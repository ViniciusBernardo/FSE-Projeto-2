import curses

class InputMenu:

    choices = [
        ("1 - Ligar/Desligar Lâmpada 01 (Cozinha)", "lamp_01"),
        ("2 - Ligar/Desligar Lâmpada 02 (Sala)", "lamp_02"),
        ("3 - Ligar/Desligar Lâmpada 03 (Quarto 01)", "lamp_03"),
        ("4 - Ligar/Desligar Lâmpada 04 (Quarto 02)", "lamp_04"),
        ("5 - Ligar/Desligar Ar-Condicionado 01 (Quarto 01)", "ac_01"),
        ("6 - Ligar/Desligar Ar-Condicionado 02 (Quarto 02)", "ac_02"),
        ("7 - Sair", "quit")
    ]

    def __init__(self, stdscr):
        self.highlight = 1
        max_y, max_x = stdscr.getmaxyx()
        self.window = curses.newwin(max_y, int(max_x / 2), 0, 0)
        self.choice = ''
        self.window.keypad(True)
        self.window.box()
        self.window.refresh()

    def get_user_input(self):
        self.choice = ''
        self.print_menu()
        while True:
            c = self.window.getch()
            if c == curses.KEY_UP:
                if self.highlight == 1:
                    self.highlight = len(self.choices)
                else:
                    self.highlight -= 1
            elif c == curses.KEY_DOWN:
                if self.highlight == len(self.choices):
                    self.highlight = 1
                else:
                    self.highlight += 1
            elif c == 10:
                self.choice = self.choices[self.highlight - 1][-1]

            self.print_menu()

            if self.choice != '':
                break

        return self.choice

    def print_menu(self):
        x = 3
        y = 4
        self.window.clear()
        self.window.addstr(1, 1, "Utilize as setas para navegar entre as opções")
        self.window.addstr(2, 1, "Pressione Enter para selecionar uma opção")
        self.window.box()
        for i, (choice, key) in enumerate(self.choices):
            if self.highlight == i + 1:
                self.window.addstr(y, x, choice, curses.A_REVERSE)
            else:
                self.window.addstr(y, x, choice, curses.A_NORMAL)
            y += 1
        self.window.refresh()

    def print_info(self, message, y=1, x=1):
        self.window.clear()
        self.window.addstr(y, x, message)
        self.window.box()
        self.window.refresh()
