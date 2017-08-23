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
from thrift import Thrift

try:

    # Make socket
    transport = TSocket.TSocket('localhost', 9090)

    # Buffering is critical. Raw sockets are very slow
    transport = TTransport.TBufferedTransport(transport)

    # Wrap in a protocol
    protocol = TBinaryProtocol.TBinaryProtocol(transport)

    # Create a client to use the protocol encoder
    client = PythonServer.Client(protocol)

    # Connect!
    transport.open()

    Info = client.QueryPersonInfo('23')

    print Info.nAge
    print Info.strName

    for subject in Info.Subjects:
        print str(subject)
    print Info.strTel
    # Close!
    transport.close()

except Thrift.TException, tx:
    print '%s' % (tx.message)