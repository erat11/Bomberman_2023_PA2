#include "Player.h"

class AI : public Player
{
	public:
		AI () : Player () {}
		AI ( int x, int y, int hp, char r, const string & name ) : Player ( x, y, hp, r, name ) 
		{ 
			this->isBot = 1; 
			updateSpeed = 300;
			counter = 0;
			moveCommand = false;
			mover = -1;
			wait = 0;
			mapRep = 'B';
		}
		void update() override
		{
			counter++;
			if ( counter == updateSpeed + wait )
			{
				wait = 0;
				counter = 0;
				moveCommand = true;
			}
		}
		bool getMove() const override { return moveCommand; }
		void resetMove () override { moveCommand = false; }
		void generateMove ( vector<vector<GameObject*>> & gameMap ) override
		{
			if ( buffs.size() && !getBuffActive() ) mover = 4;
			else if ( moveQ.size() )
			{
				mover = moveQ.front();
				moveQ.pop();
			}
			else if ( bfs ( gameMap, 2 ) ) 
			{
				if ( calculateDistance() > 2 )
				{
					moveToGoal( gameMap, nextMove.first, nextMove.second );
					if ( mover == -1 ) moveToGoal( gameMap, targerPos.first, targerPos.second );
				}
				else 
				{
					mover = 5;
					runAway();
				}
			}
			else bombWalls ( gameMap );
		}
		AI * clone () const { return new AI ( *this ); }
		void freeze () override { wait = 10000; }
	private:
		pair<int,int> nextMove;
		queue<int> moveQ;
		int updateSpeed, counter, wait;
		pair<int,int> targerPos;
		bool moveCommand;
		bool bfs( vector<vector<GameObject*>> & gameMap, int target ) 
		{
			int numRows = gameMap.size();
			int numCols = gameMap[0].size();
			vector<vector<pair<bool, pair<int, int>>>> visited(numRows, vector<pair<bool, pair<int, int>>>
				(numCols, make_pair(false, make_pair(attributes.mapPos.first, attributes.mapPos.second))));
			// Define the possible neighbor directions (up, down, left, right)
			int dr[] = {0, -1, 1, 0};
			int dc[] = {-1, 0, 0, 1};

			queue<pair<int,int>> q;
			q.push( attributes.mapPos );
			visited[attributes.mapPos.first][attributes.mapPos.second].first = true;
			p = 0;
			while ( !q.empty() ) 
			{
				p++;
				pair<int,int> current = q.front();
				q.pop();
				int row = current.first, col = current.second;
				for (int i = 0; i < 4; i++) 
				{
					int newRow = row + dr[i];
					int newCol = col + dc[i];
					if ( newRow > 0 && newRow < numRows && newCol > 0 && newCol < numCols ) 
					{
						if ( gameMap[newRow][newCol]->getType() == target && !visited[newRow][newCol].first )
						{
							targerPos = make_pair( newRow, newCol );
							int i = newRow, j = newCol;
							while ( true )
							{
								nextMove = make_pair ( i, j );
								int tmpI = visited[i][j].second.first, tmpJ = visited[i][j].second.second;
								i = tmpI;
								j = tmpJ;
								if ( ( i == attributes.mapPos.first && j == attributes.mapPos.second ) ) { break; }
							}
							return true;
						}
						char c = gameMap[newRow][newCol]->getMapRep();
						if ( ( c == ' ' || c == '?' ) && !visited[newRow][newCol].first ) 
						{
							q.push( make_pair(newRow, newCol) );
							visited[newRow][newCol].first = true;
							visited[newRow][newCol].second = make_pair ( row, col );
						}
					}
				}
			}
			return false;
		}
		void moveToGoal( vector<vector<GameObject*>> & gameMap, int xx, int yy )
		{
			// 0 = NORTH, 1 = SOUTH, 2 = EAST, 3 = WEST
			int x = attributes.mapPos.first, y = attributes.mapPos.second;
			char l = gameMap[x][y - 1]->getMapRep(), r = gameMap[x][y + 1]->getMapRep(), u = gameMap[x - 1][y]->getMapRep(), d = gameMap[x + 1][y]->getMapRep();
			if ( yy < y && ( l == ' ' || l == '?' ) ) { mover = 3; return; }
			if ( yy > y && ( r == ' ' || r == '?' ) ) { mover = 2; return; }
			if ( xx < x && ( u == ' ' || u == '?' ) ) { mover = 0; return; }
			if ( xx > x && ( d == ' ' || d == '?' ) ) { mover = 1; return; }
		}
		void runAway ( )
		{
			int a, b;
			switch( move.direction() )
			{
				case(0): 
				{ 
					a = 1;
					if ( rand() % 2 ) b = 2;
					else b = 3;
					break;
				}
				case(1): 
				{ 
					a = 0;
					if ( rand() % 2 ) b = 2; 
					else b = 3;
					break;
				}
				case(2): 
				{ 
					a = 2;
					if ( rand() % 2 ) b = 0; 
					else b = 1;
					break;
				}
				case(3): 
				{ 
					a = 3;
					if ( rand() % 2 ) b = 0; 
					else b = 1;
					break;
				}
			}

			for (int i = 0; i < 4; ++i) moveQ.push( a );
			for (int i = 0; i < 3; ++i) moveQ.push( b );
			moveQ.push( 6 );
		}
		void bombWalls ( vector<vector<GameObject*>> & gameMap )
		{
			p = calculateDistance();
			if ( bfs ( gameMap, 8 ) ) moveToGoal( gameMap, nextMove.first, nextMove.second );
			else
			{
				bfs ( gameMap, 6 );
				if ( p > 3 ) moveToGoal( gameMap, nextMove.first, nextMove.second );
				else 
				{
					mover = 5;
					runAway();
				}
			}
			
		}
		int calculateDistance () { return abs ( attributes.mapPos.first - targerPos.first ) + abs ( attributes.mapPos.second - targerPos.second ); }
};