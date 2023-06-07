AI.o: src/AI.cpp src/Player.h src/Bomb.h src/GameObject.h src/Wall.cpp \
 src/Buff.h
AllBuffs.o: src/AllBuffs.cpp src/Buff.h src/GameObject.h src/Bomb.h \
 src/Wall.cpp
Bomb.o: src/Bomb.cpp src/Bomb.h src/GameObject.h src/Wall.cpp
Buff.o: src/Buff.cpp src/Buff.h src/GameObject.h src/Bomb.h src/Wall.cpp
Game.o: src/Game.cpp src/Game.h src/StartScreen.h src/Interface.h \
 src/GetNameScreen.cpp src/Player.h src/Bomb.h src/GameObject.h \
 src/Wall.cpp src/Buff.h src/GameMap.h src/AI.cpp src/AllBuffs.cpp \
 src/Leaderboard.cpp src/SettingsScreen.cpp src/MapChooseScreen.cpp
GameMap.o: src/GameMap.cpp src/GameMap.h src/Interface.h src/Player.h \
 src/Bomb.h src/GameObject.h src/Wall.cpp src/Buff.h src/AI.cpp \
 src/AllBuffs.cpp src/Leaderboard.cpp
GameObject.o: src/GameObject.cpp src/GameObject.h
GetNameScreen.o: src/GetNameScreen.cpp src/Interface.h
Interface.o: src/Interface.cpp src/Interface.h
Leaderboard.o: src/Leaderboard.cpp src/Interface.h
MapChooseScreen.o: src/MapChooseScreen.cpp src/Interface.h
Player.o: src/Player.cpp src/Player.h src/Bomb.h src/GameObject.h \
 src/Wall.cpp src/Buff.h
ScoreTable.o: src/ScoreTable.cpp src/Interface.cpp src/Interface.h
SettingsScreen.o: src/SettingsScreen.cpp src/StartScreen.h \
 src/Interface.h
StartScreen.o: src/StartScreen.cpp src/StartScreen.h src/Interface.h
Wall.o: src/Wall.cpp src/GameObject.h
main.o: src/main.cpp src/Game.h src/StartScreen.h src/Interface.h \
 src/GetNameScreen.cpp src/Player.h src/Bomb.h src/GameObject.h \
 src/Wall.cpp src/Buff.h src/GameMap.h src/AI.cpp src/AllBuffs.cpp \
 src/Leaderboard.cpp src/SettingsScreen.cpp src/MapChooseScreen.cpp
