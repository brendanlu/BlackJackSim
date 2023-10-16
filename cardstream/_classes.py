from io import BytesIO
import socketserver
import threading

from ._utils import _strat_to_numpy_arrayfmt
from . import _wrappers


class _SocketDataToBufferHandler(socketserver.BaseRequestHandler):
    """
    The idea here is to accumulate a byte buffer in csv format, and once it
    becomes large enough, convert as many complete lines as possible into a
    dataframe on the GPU for data processing.

    In doing so, it is able to process the generated simulation data on the fly.
    """

    def __init__(self, request, client_address, server):
        self.buf = b""
        super(_SocketDataToBufferHandler, self).__init__(
            request, client_address, server
        )

    def handle(self):
        batch = b""
        while True:
            batch = self.request.recv()
            self.buf += batch
            
