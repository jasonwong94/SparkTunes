import time, beats, pygame

#sandro- i'm such a noob in python....

#some logging file for stat collections, just like Ready Set Step..
timeStampFile = open("timeStamp.txt", "a+")

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

 
pygame.mixer.init()
printInitialInfo()
timeStamp()

#implement some serial communication and call beats.playBeat 32 times...

beats.playBeat(0x0001)
beats.playBeat(0x0002)

pygame.mixer.quit()