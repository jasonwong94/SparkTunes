import pygame, time

#sandro- i'm such a noob in python....

#list of filenames of notes that the user can toggle

easterEggFile = "easterEgg_short.ogg"

noteList = {
        "C3":"c3.ogg", "D3":"d3.ogg", "E3":"e3.ogg", "F3":"f3.ogg", "G3":"g3.ogg", "A3":"a3.ogg", "B3":"b3.ogg",
        "C4":"c4.ogg", "D4":"d4.ogg", "E4":"e4.ogg", "F4":"f4.ogg", "G4":"g4.ogg", "A4":"a4.ogg", "B4":"b4.ogg",
        "C5":"c5.ogg"
}

noteSounds = {}

def loadNoteSounds():
        for note, filename in noteList.iteritems():
                noteSounds[note] = pygame.mixer.Sound(filename)

#list of notes to play

# MSB = C5, LSB = C3
# this function stores whatever sound files needs to be played in soundList
def whichNoteToPlay(notes):
        soundList = []
        parsedNotes = notes.split(" ")
        #print parsedNotes
        for note in parsedNotes:
                #print noteList[note]
                if note in noteSounds:
                        soundList.append( noteSounds[note] )
        return soundList

# plays the list of sounds              
def playNotes(soundList):
        for sound in soundList:
                #print repr(sound.play())
                if pygame.mixer.find_channel() is None:
                        sound.stop()
                sound.play()
                #sound.fadeout(900)

def playEasterEgg():
        soundList = []
        soundList.append(pygame.mixer.Sound(easterEggFile))

def playBeat(rawNotes):
        #print "Playing something"
        rawNotes = rawNotes.rstrip()
        if len(rawNotes) == 0:
                print "No notes detected- rest"
                return
        elif rawNotes == "Easter Egg":
                print "Playing Easter Egg"
                soundList = playEasterEgg()
        else:
                print "Playing note(s)"
                soundList = whichNoteToPlay(rawNotes)
        playNotes(soundList)
