import pygame, time

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

file1= "a3.ogg"
file2= "b4.ogg"
file3= "c5.oggs"


pygame.mixer.init()

printInitialInfo()
timeStamp();

print "Playing something"
sound1 = pygame.mixer.Sound(file1)
sound2 = pygame.mixer.Sound(file2)
sound3 = pygame.mixer.Sound(file3)
sound1.play()
sound2.play()
#sound3.play()
time.sleep(1)


while pygame.mixer.music.get_busy():
	print "Song is playing"
	time.sleep(0.5)

pygame.mixer.quit()

	