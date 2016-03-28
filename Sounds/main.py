import time, beats, pygame

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

#initiate
def test():
        beats.loadNoteSounds()
        noteList = (
                "C3", "D3", "E3", "F3", "G3", "A3", "B3",
                "C4", "D4", "E4", "F4", "G4", "A4", "B4",
                "C5"
        )
        for _ in range(5):
                for index in range(len(sonata)):
                        if sonata[index] != "":
                                beats.playBeat(sonata[index])
                        #beats.playBeat(' '.join(noteList))
                        time.sleep(0.2)

pygame.mixer.pre_init(44100, -16, 2, 512)
pygame.mixer.init()
pygame.mixer.set_num_channels(30)
printInitialInfo()
timeStamp()

if TESTFLAG:
        test()
else:
        #implement some serial communication and call beats.playBeat 32 times...
        beats.loadNoteSounds()
        beats.playBeat("A3 B3 C4")
        beats.playBeat("C3 D3 C5")
        beats.playBeat("C3")
        
pygame.mixer.quit()
