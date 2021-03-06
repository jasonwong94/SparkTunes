import time, beats_nopygame, serial

#sandro- i'm such a noob in python....

#some logging file for stat collections, just like Ready Set Step..
timeStampFile = open("timeStamp.txt", "a+")

TESTFLAG = True

def printInitialInfo():
        result = pygame.mixer.get_init()
        print "Program initialized:"
        print result
        
        channels = pygame.mixer.get_num_channels()
        print "Number of channels:"
        print channels

def timeStamp():
        timeActivated = time.localtime(time.time())
        formattedTime = time.asctime(timeActivated)
        timeStampFile.write(formattedTime + "\n")
        timeStampFile.close()
        print formattedTime

sonata = ["C3 C4 E4 G4 C5",
              "",
              "",
              "",
              "G3",
              "C4 E4",
              "D4 F4",
              "D4 F4",
              "C4 E4",
              "",
              "E3 C4",
              "G3",
              "D3 B3",
              "G3",
              "C3 C4",
              "G3",
              "B3 D4",
              "G3",
              "B3",
              "G3 D4",
              "C4 E4",
              "G3",
              "C4",
              "G3 E4",
              "C4 G4",
              "G3",
              "B3 F4",
              "G3 F4",
              "C4 E4",
              "G3",
              "E3",
              "G3"]

chord = ["C3 E3 G3 C4 E4 G4 C5",
           "C3 E3 G3 C4 E4 G4 C5",
           "C3 E3 G3 C4 E4 G4 C5",
           "C3 E3 G3 C4 E4 G4 C5",
           "C3 E3 G3 C4 E4 G4 C5",
           "C3 E3 G3 C4 E4 G4 C5",
           "C3 E3 G3 C4 E4 G4 C5",
           "C3 E3 G3 C4 E4 G4 C5",
           "C3 E3 G3 C4 E4 G4 C5",
           "C3 E3 G3 C4 E4 G4 C5",
           "C3 E3 G3 C4 E4 G4 C5"]

#initiate
def test():
        beats_nopygame.loadNoteSounds()
        noteList = (
                "C3", "D3", "E3", "F3", "G3", "A3", "B3",
                "C4", "D4", "E4", "F4", "G4", "A4", "B4",
                "C5"
        )
        for _ in range(5):
                # UNCOMMENT THE LINE BELOW TO TEST SONATA
                song = sonata

                # OR, UNCOMMENT THE LINE BELOW TO TEST CHORD
                #song = chord
                for index in range(len(song)):
                        if song[index] != "":
                                beats_nopygame.playBeat(song[index])
                        #beats_nopygame.playBeat(' '.join(noteList))
                        #beats_nopygame.playBeat(noteList[index % len(noteList)])
                        #beats_nopygame.playBeat("D3 B3")
                        time.sleep(0.2)

#replace this string
port = '/dev/cu.usbmodem1421'
arduino = serial.Serial(port)
arduino.baudrate = 115200
#give some time for the serial communication to finish setting up 
time.sleep(3)
arduino.write('Ready'.encode())

# pygame.mixer.pre_init(44100, -16, 2, 512)
# pygame.mixer.init()
# pygame.mixer.set_num_channels(300)
# printInitialInfo()
timeStamp()

while True:
  input = arduino.readline()
  print input
  beats_nopygame.playBeat(input)

if TESTFLAG:
        test()
else:
        #implement some serial communication and call beats_nopygame.playBeat 32 times...
        # beats_nopygame.loadNoteSounds()
        beats_nopygame.playBeat("A3 B3 C4")
        beats_nopygame.playBeat("C3 D3 C5")
        beats_nopygame.playBeat("C3")
        
#pygame.mixer.quit()
