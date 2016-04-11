# SparkTunes

A Spark project that uses Raspberry Pi+ Arduino. The python program uses `pygame` and `serial` library for communication between the Pi and the Arduino to send the notes to be played. The Arduino uses the `Adafruit_NeoPixel` library as it uses the NeoPixel LED strips to indicate the progress/mode of the display. 

## How it works:
The switches are arranged in a keyboard-matrix circuit- there are 15 rows (notes) and 32 switches (beats). Frequently the Arduino will activate a row and reach each column, storing the results in an array. When the user hits the play button, the Arduino reads the data from the array and then sends a series of notes. The Pi parses the results and plays the sound file corresponding to the note (or the Easter Egg!)

## Some suggested ideas (and more to come):
* log when display is being played just for some wonderful statistical purpose
* hopefully display the live data somewhere
* ability to export music (i.e converted from a .midi to a .mp3) and push it to a Dropbox folder
* an easter egg!! (because Easter is approaching!)

## Credits:
Credits to Ian Mckenzie, Sandro Young, Alessandro Sciarra, Helen Meng, James Chen, Perbhushan Dawaraj and Fatema Farag for helping plan the structure fo the display. Special thanks to the Sparkers that helped build the display during our workshop!

## About Spark:
The Spark Design Club builds interactive electrical and mechanical displays around campus. Past displays include a giant time turner for Orientation Week! (F!rosh Week 1T5) a "magical" reactive LED table and a human powered car racing game.

**Website: ** [spark.skule.ca](https://spark.skule.ca) **Email:** spark[at]skule[dot]ca 
