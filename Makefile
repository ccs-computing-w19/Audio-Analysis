
all: convert.cpp dependencies
	g++ convert.cpp src-library/*.o -I include -o convert

dependencies: src-library/*.cpp
	cd src-library; g++ -c *.cpp -I ../include; cd ..

clean:
	rm -rf *.o
	rm -f convert
