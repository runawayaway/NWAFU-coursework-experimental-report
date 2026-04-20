#-*- coding : utf-8-*-
# coding:unicode_escape
import asyncio
import tkinter as tk
from tkinter import messagebox
import time
from datetime import datetime
from abc import ABC, abstractmethod
import threading
import random

#定义事件基类
class Event:
    def __init__(self, event_type: str, data: Dict[str, Any]):
        self.event_type = event_type
        self.data = data
        self.timestamp = datetime.now()

#定义事件处理基类
class EventHandler(ABC):
    async def handle(self, event: Event):
        pass

#通过继承事件处理基类，并通过随机时间的暂停，表示处理过程与异步性
#邮件通知事件处理
class EmailNotificationHandler(EventHandler):
    async def handle(self, event: Event):
        print(f"[Email] begin - {time.strftime('%H:%M:%S')}")
        await asyncio.sleep(random.uniform(1, 10))
        print(f"[Email] end - {time.strftime('%H:%M:%S')}")

#数据处理事件处理
class DataHandler(EventHandler):
    async def handle(self, event: Event):
        print(f"[Data] begin - {time.strftime('%H:%M:%S')}")
        await asyncio.sleep(random.uniform(1, 10))
        print(f"[Data] end - {time.strftime('%H:%M:%S')}")

#消息通知事件处理
class MessageHandler(EventHandler):
    async def handle(self, event: Event):
        print(f"[Message] begin - {time.strftime('%H:%M:%S')}")
        await asyncio.sleep(random.uniform(1, 10))
        print(f"[Message] end - {time.strftime('%H:%M:%S')}")

#事件总线类，负责进行订阅与广播
class EventBus:
    def __init__(self):
        self._subscribers: Dict[str, List[EventHandler]] = {}
        self._loop = asyncio.new_event_loop()
        
    def subscribe(self, event_type: str, handler: EventHandler):
        if event_type not in self._subscribers:
            self._subscribers[event_type] = []
        self._subscribers[event_type].append(handler)
                  
    async def publish(self, event: Event):
        if event.event_type in self._subscribers:
            handlers = self._subscribers[event.event_type]
            tasks = [handler.handle(event) for handler in handlers]
            await asyncio.gather(*tasks, return_exceptions=True)
    #由于用户界面的部分是同步方式，所以在事件处理的异步之外，设置了同步广播方式       
    def publish_sync(self, event: Event):
        asyncio.run_coroutine_threadsafe(self.publish(event), self._loop)
        
    def start_event_loop(self):
        def run_loop():
            asyncio.set_event_loop(self._loop)
            self._loop.run_forever()
            
        thread = threading.Thread(target=run_loop, daemon=True)
        thread.start()

#对用户的输入进行初步处理
class UserService:
    def __init__(self, event_bus: EventBus):
        self.event_bus = event_bus
        
    def register_user(self, username: str, email: str, password: str) -> bool:
        # 验证用户的输入是否正确
        if not self._validate_input(username, email, password):
            return False
        
        user_data = {
            "username": username,
            "email": email,
            "password": password,
        }
        
        # 发布用户注册事件
        event = Event("user_registered", user_data)
        self.event_bus.publish_sync(event)
        
        return True
    #简易的输出检查
    def _validate_input(self, username: str, email: str, password: str) -> bool:
        if len(username) < 3:
            return False
        if "@" not in email:
            return False
        if len(password) < 6:
            return False
        return True
            
#使用tkinter设置简单的界面
class RegistrationApp:
	
    def __init__(self, user_service: UserService):
        self.user_service = user_service
        self.setup_ui()
        
    def setup_ui(self):
        self.root = tk.Tk()
        self.root.title("User Registry Test")
        self.root.geometry("400x300")
        main_frame = tk.Frame(self.root, padx=20, pady=20)
        main_frame.pack(expand=True, fill=tk.BOTH)
        
        tk.Label(main_frame, text="Username:").pack(anchor=tk.W)
        self.username_entry = tk.Entry(main_frame, width=30)
        self.username_entry.pack(pady=5, fill=tk.X)
        
        tk.Label(main_frame, text="Email:").pack(anchor=tk.W)
        self.email_entry = tk.Entry(main_frame, width=30)
        self.email_entry.pack(pady=5, fill=tk.X)

        tk.Label(main_frame, text="Password:").pack(anchor=tk.W)
        self.password_entry = tk.Entry(main_frame, width=30, show="*")
        self.password_entry.pack(pady=5, fill=tk.X)
        
        register_btn = tk.Button(main_frame, text="Registry", 
                               command=self.on_register,
                               bg="#4CAF50", fg="white",
                               font=("Arial", 12))
        register_btn.pack(pady=20, ipadx=10, ipady=5)
        
        self.status_label = tk.Label(main_frame, text="", fg="blue")
        self.status_label.pack()
        
    #设置注册按钮事件
    def on_register(self):

        username = self.username_entry.get().strip()
        email = self.email_entry.get().strip()
        password = self.password_entry.get()
        
        # 调用用户服务注册
        success = self.user_service.register_user(username, email, password)
        
        if success:
            self.status_label.config(text="success", fg="green")
        else:
            self.status_label.config(text="fail,check the input", fg="red")
            
    def run(self):
        self.root.mainloop()

class Application:
    
    def __init__(self):
        self.event_bus = EventBus()
        self.user_service = UserService(self.event_bus)
        self.app = RegistrationApp(self.user_service)
        
    def setup(self):
        #订阅事件处理
        self.event_bus.subscribe("user_registered", EmailNotificationHandler())
        self.event_bus.subscribe("user_registered", DataHandler())
        self.event_bus.subscribe("user_registered", MessageHandler())
        
        # 启动事件总线
        self.event_bus.start_event_loop()
        
    def run(self):
        self.setup()
        self.app.run()

def main():
    app = Application()
    app.run()

if __name__ == "__main__":
    main()

