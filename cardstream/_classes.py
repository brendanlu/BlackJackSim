from io import BytesIO
import socketserver

from ._utils import _strat_to_numpy_arrayfmt
from . import _wrappers


class _SocketDataToBufferHandler(socketserver.BaseRequestHandler):
    def __init__(self, request, client_address, server):
        self.byte_buffer = b""
        super(_SocketDataToBufferHandler, self).__init__(
            request, client_address, server
        )

    def handle(self):
        while True:
            self.data = self.request.recv()
