CXXFLAGS = -Wall -Wextra -std=c++11

LDFLAGS= -fsanitize=address -lncurses

SRCS = src/main.cpp src/Game.cpp src/Interface.cpp src/GameObject.cpp src/Moveset.cpp src/Buff.cpp src/GameMap.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = game

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)