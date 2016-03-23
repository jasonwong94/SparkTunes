import pygame, time

#sandro- i'm such a noob in python....

timeStampFile = open("timeStamp.txt", "a+")

#list of filenames of notes that the user can toggle
noteList = (
	"c3", "d3", "e3", "f3", "g3", "a3", "b3",
	"c4", "d4", "e4", "f4", "g4", "a4", "b4",
	"c5"
)

#list of notes to play
soundList = []

# MSB = C5, LSB = C3
# this function stores whatever sound files needs to be played in soundList
def whichNoteToPlay(note):
	for index in range(len(noteList)):
		if note >> index & 0x0001:
			soundList.append( pygame.mixer.Sound(noteList[index] + ".ogg") )

# plays the list of sounds		
def playNotes():
	for index in range(len(soundList)):
		soundList[index].play()

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
timeStamp();
print "Playing something"

whichNoteToPlay(0x0001)
playNotes()
time.sleep(1)


while pygame.mixer.music.get_busy():
	print "Song is playing"
	time.sleep(0.5)

pygame.mixer.quit()

	