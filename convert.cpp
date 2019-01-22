#include "MidiFile.h"

using namespace std;
using namespace smf;

int main() {
	MidiFile midifile;
	midifile.read("test.mid");	
	return 0;
}
