import curses

class OutputMenu:

    def __init__(self, stdscr):
        max_y, max_x = stdscr.getmaxyx()
        self.win = curses.newwin(max_y, int(max_x / 2), 0, int(max_x / 2))
        self.win.box()
        self.win.refresh()

    def show_data(self, data):
        self.win.clear()
        self.win.box()
        self.win.addstr(2, 2, f'Temperatura:                              {data.get("temperature")} °C')
        self.win.addstr(3, 2, f'Umidade:                                  {data.get("humidity")}%')
        self.win.addstr(4, 2, f'Sensor de Presença 01 (Sala):             {data.get("living_room")}')
        self.win.addstr(5, 2, f'Sensor de Presença 02 (Cozinha):          {data.get("kitchen")}')
        self.win.addstr(6, 2, f'Sensor Abertura 01 (Porta Cozinha):       {data.get("kitchen_door")}')
        self.win.addstr(7, 2, f'Sensor Abertura 02 (Janela Cozinha):      {data.get("kitchen_window")}')
        self.win.addstr(8, 2, f'Sensor Abertura 03 (Porta Sala):          {data.get("living_room_door")}')
        self.win.addstr(9, 2, f'Sensor Abertura 04 (Janela Sala):         {data.get("living_room_window")}')
        self.win.addstr(10, 2, f'Sensor Abertura 05 (Janela Quarto 01):    {data.get("bedroom_window_01")}')
        self.win.addstr(11, 2, f'Sensor Abertura 05 (Janela Quarto 02):    {data.get("bedroom_window_02")}')
        self.win.addstr(12, 2, f'Alarme:                                   {data.get("activate_alarm")}')
        self.win.refresh()
