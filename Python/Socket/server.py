#coding=utf-8

import socket
import fileinput
import time
from SocketServer import TCPServer,StreamRequestHandler,ThreadingTCPServer
#抽象的服务端模型
__metaclass__=type
class AbstractServer:
    def __init__(self):
        """
        #初始化套接字
        """
        self.serverSocket = socket.socket()
        self.serverAddress = socket.gethostname()
    def setListenPort(self,serverPort):
        """
        :param serverPort: 设置监听端口号
        :return:
        """
        self.serverPort = serverPort

    def startServer(self):
        pass


#定义简单模型的服务端
__metaclass__=type
class SimpleServer(AbstractServer):
    def startServer(self):
        #绑定套接字
        self.serverSocket.bind((self.serverAddress,self.serverPort))
        self.serverSocket.listen(100)
        while True:
            peerSocket,peerAddress = self.serverSocket.accept()
            print 'peerIPAddres:[%s],peerPort[%d].'% peerAddress
            peerSocket.send('Welcome python world!')
            peerSocket.close()

#基于SocketServer的服务器模型
__metaclass__=type
class CustomTcpHandler(StreamRequestHandler):
    def handle(self):
        peerInfo = self.request.getpeername()
        print 'peerIPAddres:[%s],peerPort[%d].' % peerInfo
        self.wfile.write('Welcome python world!')
        time.sleep(10)

__metaclass__=type
class ComplexServer(AbstractServer):
    def startServer(self):
        #绑定套接字
        Server = TCPServer((self.serverAddress,self.serverPort),CustomTcpHandler)
        Server.serve_forever()

#基于多线程的Server
__metaclass__=type
class MultiThreadServer(AbstractServer):
    def startServer(self):
        #绑定套接字
        Server = ThreadingTCPServer((self.serverAddress,self.serverPort),CustomTcpHandler)
        Server.serve_forever()





if __name__ == '__main__':
    Server = MultiThreadServer()
    Server.setListenPort(10998)
    Server.startServer()








