
all: convert.cpp dependencies
	g++ convert.cpp src-library/*.o -I include -o convert

dependencies: modifier src-library/*.cpp
	cd src-library; g++ -c *.cpp -I ../include; cd ..

modifier: src-library/MidiModifier.cpp
	cd src-library; g++ -c MidiModifier.cpp -I ../include; cd ..

clean:
	rm -rf *.o
	rm -f convert
