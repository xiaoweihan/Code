
#import sys lib
import sys
import glob

#add thrift py file to PYTHONPATH
sys.path.append(r'D:\pythonlib')
#print (sys.path)
from Interface import PythonServer
from Interface import ttypes

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer


class PythonServerHandler:
    def __init__(self):
        self.log = {}

    def QueryPersonInfo(self,strID):
        print 'strID = %s' % strID
        sex = ttypes.SEX()
        Info = ttypes.personinfo(22,'hanxiaowei',sex.MALE,'1234567',['a','b','c','d'])
        return Info
handler = PythonServerHandler()
processor = PythonServer.Processor(handler)
transport = TSocket.TServerSocket(port=9090)
tfactory = TTransport.TBufferedTransportFactory()
pfactory = TBinaryProtocol.TBinaryProtocolFactory()

server = TServer.TSimpleServer(processor, transport, tfactory, pfactory)

# You could do one of these for a multithreaded server
# server = TServer.TThreadedServer(processor, transport, tfactory, pfactory)
# server = TServer.TThreadPoolServer(processor, transport, tfactory, pfactory)

print 'Starting the server...'
server.serve()
print 'done.'




