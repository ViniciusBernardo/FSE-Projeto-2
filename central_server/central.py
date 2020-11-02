import socket
import time
import json

HOST = '192.168.0.53'  # Standard loopback interface address (localhost)
PORT = 8042        # Port to listen on (non-privileged ports are > 1023)

def initiate_communication():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        conn, addr = s.accept()
        while True:
            data = conn.recv(1024)
            if data:
                data = json.loads(data)
                print("RECIEVED: ", data)

if __name__ == "__main__":
    initiate_communication()
