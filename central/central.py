import socket
import time
import os
import json


class Central:

    _first_floor = '../config/configuracao_andar_terreo.json'
    _second_floor = '../config/configuracao_andar_1.json'

    def __init__(self):
        # self._CENTRAL_HOST = "192.168.0.53"
        self._CENTRAL_HOST = "127.0.0.1"
        self._CENTRAL_PORT = 8080
        # self._CENTRAL_PORT = 10007
        self.create_central()

    def get_config(self, filePath):
        file = open(filePath, 'r')
        json_data = json.load(file)
        print(
            f"Configurando cliente:\n\tIP: {json_data['ip']}\n\tPorta: {json_data['porta']}")
        return json_data

    def connect_disributed(self):
        try:
            while(True):
                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                    s.connect((self.HOST, self.PORT))
                    s.sendall(b'Hello, world')
                    data = s.recv(1024)
                    print(data)
        except:
            print(
                f'Tentando se conectar com \n\tIP: {self.HOST}\n\tPorta: {self.PORT}')
            time.sleep(1)
            os.system('cls' if os.name == 'nt' else 'clear')

    def create_central(self):
        while(True):
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.bind((self._CENTRAL_HOST, self._CENTRAL_PORT))
                s.listen()
                conn, addr = s.accept()
                with conn:
                    print('Connected by', addr)
                    while True:
                        data = conn.recv(1024)
                        if not data:
                            break
                        conn.sendall(data)


central_cliente = Central()
# central_cliente.connect_socket()
