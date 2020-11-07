import csv
import datetime


class CSVWriter:

    filename = "system_log.csv"

    def __init__(self):
        self.csv_file = open(self.filename, 'w')
        fields = [
            "date",
            "time",
            "temperature",
            "humidity",
            "living_room",
            "kitchen",
            "kitchen_door",
            "kitchen_window",
            "living_room_door",
            "living_room_window",
            "bedroom_window_01",
            "bedroom_window_02",
            "activate_alarm",
            "lamp_01",
            "lamp_02",
            "lamp_03",
            "lamp_04",
            "ac_01",
            "ac_02",
            "desired_temperature"
        ]
        self.writer = csv.DictWriter(self.csv_file, fieldnames=fields)
        self.writer.writeheader()


    def write_row(self, data):
        now = datetime.datetime.now()
        data['date'] = f'{now.day}-{now.month}-{now.year}'
        data['time'] = f'{now.hour}:{now.minute}:{now.second}'

        self.writer.writerows([data])

    def save(self):
        self.csv_file.close()
