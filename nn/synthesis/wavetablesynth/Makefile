all: TW

TW: main.cpp WavetableOsc.cpp
	g++ -std=c++11 -o testwav mainEnum.cpp WavetableOsc.cpp

beta: main.cpp WavetableOsc.cpp
		g++ -std=c++11 -o testwavbeta mainBeta.cpp WavetableOsc.cpp

TWold: main.cpp WavetableOsc.cpp
		g++ -std=c++11 -o testwav main.cpp WavetableOsc.cpp

betaTst:
	./testwavbeta

clean:
	rm -f TW TW2 beta
