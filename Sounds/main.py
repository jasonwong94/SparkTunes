import time, beats, pygame

#sandro- i'm such a noob in python....

#some logging file for stat collections, just like Ready Set Step..
timeStampFile = open("timeStamp.txt", "a+")

TESTFLAG = False

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

#initiate
def test():
	noteList = (
		"C3", "D3", "E3", "F3", "G3", "A3", "B3",
		"C4", "D4", "E4", "F4", "G4", "A4", "B4",
		"C5"
	)
	for index in range(len(noteList)):
		beats.playBeat(noteList[index])

pygame.mixer.init()
printInitialInfo()
timeStamp()

if TESTFLAG:
	test()
else:
	#implement some serial communication and call beats.playBeat 32 times...
	beats.playBeat("A3 B3 C4")
	beats.playBeat("C3 D3 C5")
	beats.playBeat("B4")

pygame.mixer.quit()