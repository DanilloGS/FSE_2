import socket
import time
import os

while(True):
    try:
        while(True):
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect(("127.0.0.1", 8080))
                s.sendall(b'Hello, world')
                data = s.recv(1024)
                print(data)
    except:
        print(
            f'Tentando se conectar com \n\tIP: {"127.0.0.1"}\n\tPorta: {8080}')
        time.sleep(1)
        os.system('cls' if os.name == 'nt' else 'clear')