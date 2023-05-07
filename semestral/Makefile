CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

SRC = src/main.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = gameExecutable

SDL_LIB = -lSDL2
SDL_INCLUDE = -I/usr/include/SDL2

# Build target
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC) $(SDL_LIB)

# Compile each source file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SDL_INCLUDE) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJ) $(EXEC)
	rm -f src/*.o
