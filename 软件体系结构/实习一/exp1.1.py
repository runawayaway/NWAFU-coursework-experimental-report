#-*- coding : utf-8-*-
# coding:unicode_escape
import threading
from multiprocessing import Pipe

#定义过滤器基类，保证子类必须实现对文本的处理
class TextFilter:
    def process(self, text):
        raise NotImplementedError()

#作为演示，定义两种简单的过滤器
#定义将字母全部设置为大写的过滤器
class UpperCaseFilter(TextFilter):
    def process(self, text):
        return text.upper()

#定义将消息反转的过滤器
class ReverseFilter(TextFilter):
    def process(self, text):
        return text[::-1]

#定义客户端，负责发送测试消息
class Client(threading.Thread):
    def __init__(self, conn):
        super().__init__()
        self.conn = conn
    
    def run(self):
        messages = ["hello world", "test message"]
        for msg in messages:
            self.conn.send(msg)
            print(f"Client send: {msg}")

#定义服务器，负责接受消息与对消息进行处理
class Server(threading.Thread):
    def __init__(self, conn):
        super().__init__()
        self.conn = conn
        #定义存储过滤器的列表
        self.filters = []
    
    #定义在Server中添加过滤器的方法
    def add_filter(self, filter_obj):
        self.filters.append(filter_obj)
	
	#定义使用过滤器对消息进行处理的方法
    def process_text(self, text):
        result = text
        for filter_obj in self.filters:
             result = filter_obj.process(result)
        return result
    
    #模拟过滤器之间通过管道进行数据的输入与输出的过程
    def process_text(self, text):
        result = text
        for filter_obj in self.filters:
            result = filter_obj.process(result)
        return result
      
    #接受客户端的消息，并进行处理
    def run(self):
        while True:
            if self.conn.poll(1):
                data = self.conn.recv()
                processed = self.process_text(data)
                print(f"Server receive: {processed}")

def main():
	#使用python自带的Pipe类，创建客户端与服务器之间的管道
    client_conn, server_conn = Pipe()
    
    #分别创建服务器与客户端的实例
    server = Server(server_conn)
    client = Client(client_conn)
    
    #向服务器中添加过滤器
    server.add_filter(UpperCaseFilter())
    server.add_filter(ReverseFilter())
    
    
    #开始进程
    server.start()
    client.start()
    

    client.join()
    server.join()

if __name__ == "__main__":
    main()
