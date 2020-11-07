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
        self.win.addstr(2, 2, f'Temperatura Ambiente:                     {data.get("temperature")} °C')
        self.win.addstr(3, 2, f'Temperatura Desejada:                     {data.get("desired_temperature")} °C')
        self.win.addstr(4, 2, f'Umidade:                                  {data.get("humidity")} %')
        self.win.addstr(6, 2, f'Sensor de Presença 01 (Sala):             {data.get("living_room")}')
        self.win.addstr(7, 2, f'Sensor de Presença 02 (Cozinha):          {data.get("kitchen")}')
        self.win.addstr(8, 2, f'Sensor Abertura 01 (Porta Cozinha):       {data.get("kitchen_door")}')
        self.win.addstr(9, 2, f'Sensor Abertura 02 (Janela Cozinha):      {data.get("kitchen_window")}')
        self.win.addstr(10, 2, f'Sensor Abertura 03 (Porta Sala):          {data.get("living_room_door")}')
        self.win.addstr(11, 2, f'Sensor Abertura 04 (Janela Sala):         {data.get("living_room_window")}')
        self.win.addstr(12, 2, f'Sensor Abertura 05 (Janela Quarto 01):    {data.get("bedroom_window_01")}')
        self.win.addstr(13, 2, f'Sensor Abertura 05 (Janela Quarto 02):    {data.get("bedroom_window_02")}')
        self.win.addstr(14, 2, f'Alarme:                                   {data.get("activate_alarm")}')
        self.win.addstr(16, 2, f'Lâmpada 01 (Cozinha):                     {data.get("lamp_01")}')
        self.win.addstr(17, 2, f'Lâmpada 02 (Sala):                        {data.get("lamp_02")}')
        self.win.addstr(18, 2, f'Lâmpada 03 (Quarto 01):                   {data.get("lamp_03")}')
        self.win.addstr(19, 2, f'Lâmpada 04 (Quarto 02):                   {data.get("lamp_04")}')
        self.win.addstr(20, 2, f'Ar-Condicionado 01 (Quarto 01):           {data.get("ac_01")}')
        self.win.addstr(21, 2, f'Ar-Condicionado 02 (Quarto 02):           {data.get("ac_02")}')
        self.win.refresh()
