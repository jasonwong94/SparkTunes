from mingus.midi import fluidsynth
from mingus.containers import Note, NoteContainer
from mingus.containers.instrument import MidiInstrument
import time
import os.path

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

timber = ["",
          "G4 E5",
          "D5",
          "C5",
          "E4 A4 C5",
          "A4",
          "",
          "E4 A4",
          "",
          "E5",
          "D5",
          "C5",
          "C4 G4 E5",
          "G5",
          "",
          "C4 G4",
          "",
          "E5",
          "D5",
          "C5",
          "D4 G4 D5",
          "",
          "",
          "D4 G4",
          "",
          "E5",
          "D5",
          "C5",
          "C4 F4 C5",
          "A4",
          "",
          "C4 F4"]

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

slam = [" ".join([
                "C3", "D3", "E3", "F3", "G3", "A3", "B3",
                "C4", "D4", "E4", "F4", "G4", "A4", "B4",
                "C5"])]

def play_offset(off, sleep_s=0.2, song=sonata, repeats=5, down_octaves=1, instrument=None):
        song = song * repeats
        if instrument is not None:
                for i in range(off):
                        fluidsynth.set_instrument(i+1, instrument)
        for i in range(len(song)+off):
                to_stop = i - off
                if to_stop >= 0:
                        fluidsynth.stop_NoteContainer(NoteContainer([a + "-" + str(int(b)-down_octaves) for (a, b) in song[to_stop].split()]), channel=(to_stop%off)+1)
                if i < len(song):
                        fluidsynth.play_NoteContainer(NoteContainer([a + "-" + str(int(b)-down_octaves) for (a, b) in song[i].split()]), channel=(i%off)+1, velocity=127)
                time.sleep(sleep_s)

fluidsynth.init(os.path.join("fs", "FluidR3 GM2-2.SF2"))

# UNCOMMENT THE LINE BELOW TO TEST SONATA
#play_offset(2, song=sonata, sleep_s=0.2)

# OR, UNCOMMECT THE LINE BELOW TO TEST CHORD
#play_offset(2, song=chord, sleep_s=0.2)

play_offset(2, sleep_s=0.22, song=timber, repeats=5)
