CXXFLAGS = -Wall -pedantic -std=c++17

# Define the SFML libraries to link against
LDFLAGS= -fsanitize=address -lncursesw
FILES= src/*.cpp

# Define the source files and the output executable
SRCS = src/main.cpp src/GameMap.cpp src/Player.cpp src/Wall.cpp src/AI.cpp src/Bomb.cpp src/Game.cpp src/StartScreen.cpp src/Interface.cpp src/GameObject.cpp src/Buff.cpp src/SettingsScreen.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = game

# Compile and link the program
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $(EXEC)

# Compile the source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up the object files and the executable
clean:
	rm -f $(OBJS) $(EXEC)