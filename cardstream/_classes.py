from io import BytesIO
import multiprocessing
import pandas as pd  # this should change to cudf later
import socket
import threading

from ._utils import _strat_to_numpy_arrayfmt
from . import _wrappers


def _processing_function(server_ready: multiprocessing.Event, *, host="127.0.0.1", port=11111):
    # ipv4 and tcp
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((host, port))
    server.listen(1)

    # signal that the server is ready before the simulation loop commences
    server_ready.set()

    simulation, _ = server.socket.accept()

    # do some processing from recv calls (this will probably be threaded)


def _simulation_function(
    server_ready: multiprocessing.Event,
    nd,
    p,
    d17,
    nA,
    strats,
    *,
    host: str = "127.0.0.1",
    port: int = 11111,
    log: str = None
):
    engine = _wrappers._SimEngineWrapper(nd, p, d17, nA, strats)

    if host and port:
        # do some checking of host and port input and pass to engine setter
        engine._py_set_socket_connection(host, port)

    if log:
        # check if valid log file name and pass to engine setter
        engine._py_set_log_file(log)

    server_ready.wait()
    engine.run()

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
        self.d17 = 17
        self.nA = nA
        self.strats = strats
        self.buf = b""

    def run(self):
        server_ready = multiprocessing.Event()

        anlys_process = multiprocessing.Process(
            target=_processing_function, 
            args=(server_ready)
        )

        sim_process = multiprocessing.Process(
            target=_simulation_function,
            args=(server_ready, self.nd, self.p, self.d17, self.nA, self.strats),
        )

        anlys_process.start()
        sim_process.start()

        anlys_process.join()
        sim_process.join()

