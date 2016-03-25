import pygame, time

#sandro- i'm such a noob in python....

#list of filenames of notes that the user can toggle

noteList = {
	"C3":"c3.ogg", "D3":"d3.ogg", "E3":"e3.ogg", "F3":"f3.ogg", "G3":"g3.ogg", "A3":"a3.ogg", "B3":"b3.ogg",
	"C4":"c4.ogg", "D4":"d4.ogg", "E4":"e4.ogg", "F4":"f4.ogg", "G4":"g4.ogg", "A4":"a4.ogg", "B4":"b4.ogg",
	"C5":"c5.ogg"
}

#list of notes to play

# MSB = C5, LSB = C3
# this function stores whatever sound files needs to be played in soundList
def whichNoteToPlay(notes, soundList):
	parsedNotes = notes.split(" ")
	print parsedNotes
	for index in range(len(parsedNotes)):
		note = parsedNotes[index]
		print noteList[note]
		soundList.append( pygame.mixer.Sound(noteList[note]) )

# plays the list of sounds		
def playNotes(soundList):
	for index in range(len(soundList)):
		soundList[index].play()

def playBeat(rawNotes):
	soundList = []
	print "Playing something"
	whichNoteToPlay(rawNotes, soundList)
	playNotes(soundList)
	
	#def want to play around with the numbers here..what's the magic number
	time.sleep(0.35)
	while pygame.mixer.music.get_busy():
		print "Song is playing"
	
	time.sleep(0.35)
	del soundList