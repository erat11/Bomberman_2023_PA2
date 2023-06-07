CXX=g++
LD=g++
CXXFLAGS=-Wall -pedantic -fsanitize=address -lncursesw

all: vorosjoz

vorosjoz: src/main.o src/GameObject.o src/Interface.o src/Bomb.o src/Buff.o src/GameMap.o src/Player.o src/StartScreen.o
	$(LD) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f src/*.o vorosjoz 

deps: 
	$(CXX) -MM src/*cpp > Makefile.d

compile: src/main.cpp src/GameObject.cpp src/Interface.cpp src/Bomb.cpp src/Buff.cpp src/GameMap.cpp src/Player.cpp src/StartScreen.cpp
	$(CXX) $(CXXFLAGS) -o vorosjoz $^

run: vorosjoz
	./vorosjoz

doc: Doxyfile
	doxygen Doxyfile

.PHONY: doc

-include Makefile.d