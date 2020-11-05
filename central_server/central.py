import time
import json
import socket
import curses
import threading

CENTRAL_HOST = '192.168.0.53'
DISTRIBUTED_HOST = '192.168.0.52'
PORT = 8042        # Port to listen on (non-privileged ports are > 1023)

stdscr = curses.initscr()
curses.noecho()
curses.cbreak()
stdscr.keypad(True)

def send_command():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    while sock.connect_ex((DISTRIBUTED_HOST, PORT)) != 0:
        print("Retrying to connect to distributed server in 3 seconds...")
        time.sleep(3)
    while True:
        command = input("Enter with command: ")
        sock.sendall(bytes(command, 'utf-8'))

def receive_info():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((CENTRAL_HOST, PORT))
    sock.listen()
    conn, addr = sock.accept()

    while True:
        data = conn.recv(1024)
        if data:
            data = json.loads(data)
            max_y, max_x = stdscr.getmaxyx()
            win = curses.newwin(max_y, int(max_x / 2), 0, int(max_x / 2))
            win.box()
            win.addstr(2, 2, f'Temperatura:                              {data.get("temperature")}')
            win.addstr(2, 3, f'Sensor de Presença 01 (Sala):             {data.get("living_room")}')
            win.addstr(2, 4, f'Sensor de Presença 02 (Cozinha):          {data.get("kitchen")}')
            win.addstr(2, 5, f'Sensor Abertura 01 (Porta Cozinha):       {data.get("kitchen_door")}')
            win.addstr(2, 6, f'Sensor Abertura 02 (Janela Cozinha):      {data.get("kitchen_window")}')
            win.addstr(2, 7, f'Sensor Abertura 03 (Porta Sala):          {data.get("living_room_door")}')
            win.addstr(2, 8, f'Sensor Abertura 04 (Janela Sala):         {data.get("living_room_window")}')
            win.addstr(2, 9, f'Sensor Abertura 05 (Janela Quarto 01):    {data.get("bedroom_window_01")}')
            win.addstr(2, 10, f'Sensor Abertura 05 (Janela Quarto 02):    {data.get("bedroom_window_02")}')
            win.refresh()
        else:
            break

if __name__ == "__main__":
    receive_thread = threading.Thread(target=receive_info)
    send_command_thread = threading.Thread(target=send_command)

    receive_thread.start()
    send_command_thread.start()
