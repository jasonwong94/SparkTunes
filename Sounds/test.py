import pygame, time

def printInitialInfo():
	result = pygame.mixer.get_init()
	print "Program initialized:"
	print result
	
	channels = pygame.mixer.get_num_channels()
	print "Number of channels:"
	print channels 

file1= "w20.wav"
file2= "w21.wav"
file3= "w30.wav"


pygame.mixer.init()

printInitialInfo()

print "Playing something"
sound1 = pygame.mixer.Sound(file1)
sound2 = pygame.mixer.Sound(file2)
sound3 = pygame.mixer.Sound(file3)
sound1.play()
sound2.play()
#sound3.play()
time.sleep(2)


while pygame.mixer.music.get_busy():
	print "Song is playing"
	time.sleep(1)

pygame.mixer.quit()

	