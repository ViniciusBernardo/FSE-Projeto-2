import time
import json
import socket
import threading

HOST = '192.168.0.53'  # Standard loopback interface address (localhost)
PORT = 8042        # Port to listen on (non-privileged ports are > 1023)

def send_command():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    while sock.connect_ex(('', PORT)) != 0:
        print("Retrying to connect to distributed server in 3 seconds...")
        time.sleep(3)
    while True:
        command = input("Enter with command: ")
        sock.sendall(bytes(command, 'utf-8'))

def receive_info():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(('', PORT))
    sock.listen()
    conn, addr = sock.accept()

    while True:
        data = conn.recv(1024)
        if data:
            data = json.loads(data)
        else:
            break

if __name__ == "__main__":
    receive_thread = threading.Thread(target=receive_info)
    send_command_thread = threading.Thread(target=send_command)

    receive_thread.start()
    send_command_thread.start()
