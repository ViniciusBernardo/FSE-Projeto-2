import os
import sys
import time
import json
import signal
import socket
import curses
import threading
from output_menu import OutputMenu
from input_menu import InputMenu
from alarm_player import Player
from csv_writer import CSVWriter

CENTRAL_HOST = '192.168.0.53'
DISTRIBUTED_HOST = '192.168.0.52'
CENTRAL_PORT = 10009
DISTRIBUTED_PORT = 10109

csv_obj = CSVWriter()
stdscr = curses.initscr()
curses.noecho()
curses.cbreak()
stdscr.keypad(True)

player = Player()
socket_send = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket_receive = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

def send_command(sock):
    menu = InputMenu(stdscr)
    while sock.connect_ex((DISTRIBUTED_HOST, DISTRIBUTED_PORT)) != 0:
        menu.print_info("Tentando se conectar ao servidor distribuído...")
        time.sleep(1)
    while True:
        command = menu.get_user_input()
        sock.sendall(bytes(command, 'utf-8'))
        if command == 'quit':
            os.kill(os.getpid(), signal.SIGINT)

def receive_info(sock, player):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((CENTRAL_HOST, CENTRAL_PORT))
    sock.listen()
    conn, addr = sock.accept()

    menu = OutputMenu(stdscr)

    while True:
        data = conn.recv(1024)
        if data:
            data = json.loads(data)
            #player.decide_play_alarm(data['activate_alarm'])
            menu.show_data(data)
            csv_obj.write_row(data)
        else:
            break

def signal_handler(sig, frame):
    player.stop()
    csv_obj.save()
    socket_send.close()
    socket_receive.close()
    sys.exit(0)

if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal_handler)
    receive_thread = threading.Thread(target=receive_info, args=(socket_receive, player,))
    send_command_thread = threading.Thread(target=send_command, args=(socket_send,))

    receive_thread.start()
    send_command_thread.start()
