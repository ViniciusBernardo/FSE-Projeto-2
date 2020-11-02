import time
import json
import socket
import threading

HOST = '192.168.0.53'  # Standard loopback interface address (localhost)
PORT = 8042        # Port to listen on (non-privileged ports are > 1023)

def receive_info(conn):
    while True:
        data = conn.recv(1024)
        if data:
            data = json.loads(data)
        else:
            break

def send_command(conn):
    while True:
        command = input("Enter with command: ")
        conn.sendall(bytes(command, 'utf-8'))

if __name__ == "__main__":
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((HOST, PORT))
    sock.listen()
    conn, addr = sock.accept()

    receive_thread = threading.Thread(target=receive_info, args=(conn,))
    send_command_thread = threading.Thread(target=send_command, args=(conn,))

    receive_thread.start()
    send_command_thread.start()
