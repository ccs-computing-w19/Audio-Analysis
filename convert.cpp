#include "MidiFile.h"
#include <iostream>

using namespace std;
using namespace smf;

int main(int argc, char** argv) {
	if(argc != 3) {
		cout << "Usage: " << argv[0] << " [midi file] [semitones to transpose]" << endl;
		return 0;
	}

	// Loads data from test.mid into a midifile object
	MidiFile midifile;
	midifile.read(argv[1]);

	int semitones = atoi(argv[2]);	

	for(int i = 0; i < midifile.getTrackCount(); i++) {
		for(int j = 0; j < midifile[i].getEventCount(); j++) {
			if(!midifile[i][j].isNote()) continue;

			if(midifile[i][j].getChannel() == 9) continue;

			int new_note = semitones + midifile[i][j].getP1();
			midifile[i][j].setP1(new_note);
		}
	}
	
	// Debugging, ensures that test.mid exists
	midifile.writeHex(cout);

	

	return 0;
}
