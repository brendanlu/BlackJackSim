import io 
import multiprocessing
import pandas as pd  # this should change to cudf later
import socket
import threading

from ._utils import _strat_to_numpy_arrayfmt
from . import _wrappers as wrappers


# this will need to be tuned alongside communication.hpp
# sizes are in bytes
RECV_BUFFER = 1024
PROCESSING_BATCH_SIZE = 1048576 # 2e20 approx 1MB


def _server_func(server_ready: multiprocessing.Event, host: str, port: int):
    """
    Runs a server which receives the simulation data, and processes it on the
    fly in batches. 

    NOTE: If recv() is blocking, the GIL should be released. Many of the 
    processing functions in pandas/cudf should release the GIL (???) so 
    multithreading here should achieve a reasonable degree of concurrency (?). 
    """

    # ipv4 and tcp
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((host, port))
    server.listen(1)

    # signal that the server is ready before the simulation loop commences
    server_ready.set()

    sim_client, _ = server.socket.accept()
    buf = io.BytesIO()

    # do some processing from recv calls (this will probably be threaded)
    while True:
        # this will block and release the GIL 
        curr = sim_client.recv(RECV_BUFFER)
        if not curr:
            # recv call has come back with 0
            break
        else:
            buf.write(curr)


    # write to file or something
    pass

    # close client connection and socket
    sim_client.close()
    server.close()


def _sim_client_func(
    server_ready: multiprocessing.Event,
    host: str,
    port: int,
    niters,
    nd,
    p,
    d17,
    nA,
    strats,
    *,
    log: str = None
):
    """
    Configure a simulation engine, and run a simulation
    """

    engine = wrappers._SimEngineWrapper(nd, p, d17, nA, strats)

    if host and port:
        # do some checking of host and port input and pass to engine setter
        engine._py_set_socket_connection(host, port)

    if log:
        # check if valid log file name and pass to engine setter
        engine._py_set_log_file(log)

    server_ready.wait()
    engine.run(niters)


class Simulator:
    """
    A simulator class which calls the simulation engine in a different process,
    and processes the simulation data recieved via a socket.

    It is basically a socket server implementation.

    The processing is done in parallel, on the fly, and can be offloaded to the
    GPU to ease CPU compute strain.
    """

    def __init__(self, nd, p, d17, nA, strats, *, host="127.0.0.1", port=11111):
        self.nd = nd
        self.p = p
        self.d17 = d17
        self.nA = nA
        self.strats = strats
        self.host = host
        self.port = port

        self.buf = b""

    def run(self, niters):
        server_ready = multiprocessing.Event()

        anlys_process = multiprocessing.Process(
            target=_server_func, args=(server_ready, self.host, self.port)
        )

        sim_process = multiprocessing.Process(
            target=_sim_client_func,
            args=(
                server_ready,
                self.host,
                self.port,
                niters,
                self.nd,
                self.p,
                self.d17,
                self.nA,
                self.strats,
            ),
        )

        anlys_process.start()
        sim_process.start()

        anlys_process.join()
        sim_process.join()
