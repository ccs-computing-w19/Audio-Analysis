#include "MidiFile.h"
#include "MidiModifier.h"
#include <iostream>
#include <string>

using namespace std;
using namespace smf;

int main(int argc, char** argv) {
	if(argc != 3) {
		cout << "Usage: " << argv[0] << " [midi file] [semitones to transpose]" << endl;
		return 0;
	}

	// Loads data from midi file passed as argument into a midifile object
	MidiFile midifile;
	midifile.read(argv[1]);

	// Transposes the midi file
	int semitones = atoi(argv[2]);	
	transpose(midifile, 3);
	
	// Prints the midi file
	midiToText(midifile);

	// Writes the transposed MIDI file 
	string transposed_file = argv[1];
	transposed_file = transposed_file.substr(0, transposed_file.length() - 4);
	midifile.write(transposed_file + "_transposed.mid");

	return 0;
}
