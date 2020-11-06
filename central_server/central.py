import time
import json
import socket
import curses
import threading
from output_menu import OutputMenu
from input_menu import InputMenu

CENTRAL_HOST = '192.168.0.53'
DISTRIBUTED_HOST = '192.168.0.52'
CENTRAL_PORT = 10009
DISTRIBUTED_PORT = 10109

stdscr = curses.initscr()
curses.noecho()
curses.cbreak()
stdscr.keypad(True)

def send_command():
    menu = InputMenu(stdscr)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    while sock.connect_ex((DISTRIBUTED_HOST, DISTRIBUTED_PORT)) != 0:
        menu.print_info("Tentando se conectar ao servidor distribuído...")
        time.sleep(3)
    while True:
        command = menu.get_user_input()
        if isinstance(command, str):
            sock.sendall(bytes(command, 'utf-8'))
        elif isinstance(command, dict):
            sock.sendall(bytes(json.dumps(command), 'utf-8'))

def receive_info():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((CENTRAL_HOST, CENTRAL_PORT))
    sock.listen()
    conn, addr = sock.accept()

    menu = OutputMenu(stdscr)

    while True:
        data = conn.recv(1024)
        if data:
            data = json.loads(data)
            menu.show_data(data)
        else:
            break

if __name__ == "__main__":
    receive_thread = threading.Thread(target=receive_info)
    send_command_thread = threading.Thread(target=send_command)

    receive_thread.start()
    send_command_thread.start()
