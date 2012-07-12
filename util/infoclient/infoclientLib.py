import socket
import sys

import socket
import threading
import SocketServer

class ThreadedTCPRequestHandler(SocketServer.BaseRequestHandler):
    def __init__(self, callback, infoclient, *args, **keys):
            self.callback = callback
            self.infoclient = infoclient
            SocketServer.BaseRequestHandler.__init__(self, *args, **keys)

    def handle(self):
        data = self.request.recv(1024).strip()
        #print "{} wrote:".format(self.client_address[0])
        #print data
        self.callback(self.infoclient, data)
        '''
        cur_thread = threading.current_thread()
        response = "{}: {}".format(cur_thread.name, data)
        self.request.sendall(response)
        '''

def handler_factory(callback, infoclient):
    def createHandler(*args, **keys):
        return ThreadedTCPRequestHandler(callback, infoclient,  *args, **keys)
    return createHandler

def processDataCallback(infoclient, data):
    infoclient.datastore.append(data)

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass

class InfoClient(object):

    def __init__(self, infoclient_host=None, infoclient_port=None,
                 server_host=None, server_port=None):
        self.host = server_host
        self.port = server_port
        self.infoclient_host = infoclient_host
        self.infoclient_port = infoclient_port

        self._is_running = None
        self._server = None
        self._datalist = []
        self.datastore = []

    def add(self, data_name, region_name):
        pair = (data_name, region_name)
        if pair in self._datalist:
            raise ValueError('%s already registered' % pair)
        message=('<?xml version="1.0" encoding="UTF-8"?><info><add name="%s" '
                 'roi="%s"></add></info>') % pair
        self._sendmessage(message)
        self._datalist.append(pair)
        return pair

    def remove(self, pair):
        if pair not in self._datalist:
            raise ValueError('%s not registered' % pair)
        message=('<?xml version="1.0" encoding="UTF-8"?><info><remove name="%s" '
                 'roi="%s"></add></info>') % pair
        self._sendmessage(message)
        self._datalist.remove(pair)

    def stop(self):
        self._server.shutdown()
        self._is_running = None
        self._server = None

    def start(self):
        self._startserver()

    def check(self):
        if not self._is_running:
            raise RuntimeError('Server is not running')
        return self.datastore

    def send(self, message):
        self._sendmessage(message)

    def acknowledge(self):
        pass

    def _startserver(self):
        if self._is_running:
            raise RuntimeError('Server already running')

        server = ThreadedTCPServer((self.host, self.port),
                                   handler_factory(processDataCallback, self))
        ip, port = server.server_address
        print "server running at %s on port %d" % (ip, port)
        # Start a thread with the server -- that thread will then start one
        # more thread for each request
        server_thread = threading.Thread(target=server.serve_forever)
        # Exit the server thread when the main thread terminates
        server_thread.daemon = True
        server_thread.start()
        self._is_running = True
        self._server = server

    def _sendmessage(self, message):
        """Send a stream of bytes to the infoclient in murfi
        """

        # Create a socket (SOCK_STREAM means a TCP socket)
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        e = None
        try:
            # Connect to server and send data
            sock.connect((self.infoclient_host, self.infoclient_port))
            sock.sendall(message + "\n")
        except Exception, e:
            pass
        finally:
            sock.close()
        if e and "Connection refused" in e:
            raise IOError(e)
        print "Sent:     {}".format(message)
