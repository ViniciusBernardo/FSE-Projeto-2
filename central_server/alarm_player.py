import logging
from subprocess import Popen, PIPE, DEVNULL
logger = logging.getLogger(__name__)

class Player:
    def __init__(self):
        self.audio = 'security-alarm-sound.mp3'
        self.process = None
        self.last_activate_alarm = 0

    def decide_play_alarm(self, activate_alarm):
        if activate_alarm and activate_alarm != self.last_activate_alarm:
            self.start()
            self.last_activate_alarm = activate_alarm
        elif not activate_alarm and activate_alarm != self.last_activate_alarm:
            self.stop()
            self.last_activate_alarm = activate_alarm

    def start(self):
        self.stop()
        self.process = Popen(['omxplayer', '--loop', self.audio],stdin=PIPE,stdout=None,stderr=None,bufsize=0)

    def stop(self):
        p = self.process
        if self.process:
           try:
               self.process.stdin.write(bytes("p", 'utf-8'))
               self.process.terminate()
               self.process.wait() # -> move into background thread if necessary
           except EnvironmentError as e:
               logger.error("can't stop %s: %s", self.audio, e)
           else:
               self.process = None
