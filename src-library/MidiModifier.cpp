#include "MidiFile.h"
#include "MidiModifier.h"
#include <string>
#include <iostream>
#include <map>

void midiToText(smf::MidiFile& m) {
	std::string output;
	
	std::map<int, std::string> notes = {{0, "A"}, {1, "A#"}, {2, "B"}, {3, "C"}, {4, "C#"}, {5, "D"}, {6, "D#"}, {7, "E"}, {8, "F"}, {9, "F#"}, {10, "G"}, {11, {"G#"}}};

	for(int i = 0; i < m.getTrackCount(); i++) {
		for(int j = 0; j < m[i].getEventCount(); j++) {
			if(!m[i][j].isNoteOn()) continue;

			int note = m[i][j].getP1() % 12;
			output += notes[note];	
		}
	}
	std::cout << output << std::endl;
}

void transpose(smf::MidiFile& m, int semitones) {
	// Iterates through MIDI file's note events and raises their pitch
	for(int i = 0; i < m.getTrackCount(); i++) {
		for(int j = 0; j < m[i].getEventCount(); j++) {
			// Ensures the given event is a "note on" event
			if(!m[i][j].isNote()) continue;
			
			// Ignores percussion track
			if(m[i][j].getChannel() == 9) continue;

			int new_note = semitones + m[i][j].getP1();
			m[i][j].setP1(new_note);
			
		}
	}
	
}
