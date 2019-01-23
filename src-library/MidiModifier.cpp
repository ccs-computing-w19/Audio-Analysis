#include "MidiFile.h"
#include "MidiModifier.h"

void MidiToText(smf::MidiFile& m) {

}

void transpose(smf::MidiFile& m, int semitones) {
	// Iterates through MIDI file's note events and raises their pitch
	for(int i = 0; i < m.getTrackCount(); i++) {
		for(int j = 0; j < m[i].getEventCount(); j++) {
			if(!m[i][j].isNote()) continue;
			
			// Ignores percussion track
			if(m[i][j].getChannel() == 9) continue;

			int new_note = semitones + m[i][j].getP1();
			m[i][j].setP1(new_note);
			
		}
	}
	
}
