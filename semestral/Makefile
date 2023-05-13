CXXFLAGS = -Wall -Wextra -std=c++11

# Define the SFML libraries to link against
LDFLAGS=-fsanitize=address

# Define the source files and the output executable
SRCS = src/main.cpp src/Game.cpp src/StartScreen.cpp src/Interface.cpp src/GameObject.cpp src/Moveset.cpp src/Buff.cpp
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