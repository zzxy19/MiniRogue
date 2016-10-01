//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// Dungeon.cpp
//


#include "Dungeon.h"
#include "utilities.h"
#include "allMonster.h"
#include "Player.h"
#include "gameObject.h"
#include "Actor.h"
#include "Golden.h"
#include "helper.h"
#include <cstdlib>
#include <iostream>
using namespace std;


////////////////////////////////////////
// Constructor and destructor
////////////////////////////////////////
Dungeon::Dungeon(int level)
	// NOTE: dungeon constructor does NOT create new Actors or new Objects.
	// It only creates the array that holds the pointers to such new objects.
	// Pointers in the member arrays are left unitialized until functions such as initializeDungeon, createObject, createMonster, are called to give values to them.
{
	if (0 <= level && level <= 4)
		m_level = level;
	else
		exit(5);
	m_maxMonster = 2 + randInt(5 * (level + 1));
	// There can be at most m_maxMonster + 3 objects on the dungeon floor.
	// Initially there can be at most 3 objects on the floor, and each monste can only drop 1 object at most.
	m_maxObject = m_maxMonster + 3;
	m_maxRoom = 20;
	m_nMonster = 0;
	m_nObject = 0;
	// create the dynamically allocated arrays
	m_monster = new Actor*[m_maxMonster];
	m_object = new gameObject*[m_maxObject];
	m_room = new Room*[m_maxRoom];
}
	

Dungeon::~Dungeon()
	// NOTE: dungeon destructor does NOT delete Player. Player is left for ~Game to delete.
	// It deletes everything else except golden idol (which it only deletes if at level 4).
{
	for (int i = 0; i < m_nMonster; i++)
		delete m_monster[i];
	for (int i = 0; i < m_nObject; i++)
		delete m_object[i];
	for (int i = 0; i < m_nRoom; i++)
		delete m_room[i];
	delete [] m_object;
	delete [] m_monster;
	delete [] m_room;
	if (m_level == 4)
	{
		delete m_golden;
	}
}


////////////////////////////////////////
// Accessors
////////////////////////////////////////
int Dungeon::level() const
{
	return m_level;
}


bool Dungeon::isMovable(int r, int c) const
{
	char target = m_maze[r][c];
	if (target == ' ' || target == '?' || target == ')' || target == '>' || target == '&')
		return true;
	else
		return false;
}


bool Dungeon::nextLevel(int r, int c) const
{
	if (r == m_stairR && c == m_stairC)
		return true;
	else
		return false;
}


Player* Dungeon::player() const
{
	return m_player;
}


Room** Dungeon::room() const
{
	return m_room;
}


int Dungeon::nRoom() const
{
	return m_nRoom;
}


//////////////////////////////////////////
// Game command executor
//////////////////////////////////////////
void Dungeon::setPlayer(Player* player)
{
	m_player = player;
}


string Dungeon::monster_move(int distance)
{
	string message;
	for (int i = 0; i < m_nMonster; i++)
	{
		// if the monster to move is asleep, reduce its sleepiness by 1, and skip its move
		if (m_monster[i]->sleep())
		{
			m_monster[i]->wake();
			continue;
		}
		// if the monster is a dragon, call its selfheal function that, 1 out of 10 times, restore its HP by 1
		if (m_monster[i]->icon() == 'D')
			m_monster[i]->selfheal();

		int r = m_monster[i]->row();
		int c = m_monster[i]->col();
		int R = m_player->row();
		int C = m_player->col();
		// if monster can attack the player, go ahead and attack
		if (canAttack(i))
			message += m_monster[i]->attack(m_player);
		// otherwise, cannot attack. If the monster is dumb:
		else if (!m_monster[i]->isSmart())
		{
			// if can smell player, do either one of these 4 actions and continue to next monster, or do nothing, finish the loop and carry on to next monster
			if (dumbSmell(i))
			{
				// if player is below, go down
				if (R > r)
				{
					if (isMovable(r + 1, c))
					{
						m_monster[i]->move(r + 1, c);
						continue;
					}
				}
				// else if player is above, go up
				else if (R < r)
				{
					if (isMovable(r - 1, c))
					{
						m_monster[i]->move(r - 1, c);
						continue;
					}
				}
				// if player is at right, go right
				if (C > c)
				{
					if (isMovable(r, c + 1))
					{
						m_monster[i]->move(r, c + 1);						
						continue;
					}
				}
				// else if the player is at left, go left
				else if (C < c)
				{
					if (isMovable(r, c - 1))
					{
						m_monster[i]->move(r, c - 1);
						continue;
					}
				}						
			}
		}
		// otherwise, the monster is smart:
		else if (dumbSmell(i))
		{
			int nextr, nextc;
			int minimum = distance + 1;
			if (smartSmell(R, C, r, c, 0, minimum, nextr, nextc))
				m_monster[i]->move(nextr, nextc);
		}
		updateMaze();
	}
	return message;
}


string Dungeon::direction_command(char direction)
{
	string result;
	// calculate the coordinate Player is trying to move to
	int R = m_player->row();
	int C = m_player->col();
	switch (direction)
	{
	case 'h':
		C--;
		break;

	case 'j':
		R++;
		break;

	case 'k':
		R--;
		break;

	case 'l':
		C++;
		break;

	default:
		exit(777);
	}
	// check what is in that location
	char destination = m_maze[R][C];
	if (isMovable(R, C)) // can move to the location
	{
		m_player->move(R, C);
	}
	else if (destination == '#') // wall, cannot move
		;
	else // cannot move but is not a wall, so has to be a monster
	{
		bool dead = false;
		for (int i = 0; i < m_nMonster; i++) // search the monster list to find the monster at this location
		{
			if (m_monster[i]->row() == R && m_monster[i]->col() == C)
			{
				// Attack. If the monster is killed, call removeMonster[i]
				result = m_player->attack(m_monster[i]);
				dead = m_monster[i]->isDead();
				if (dead)
				{
					dropObject(R, C, m_monster[i]->icon());
					removeMonster(i);
				}
			}
		}
	}
	return result;
}


string Dungeon::pickup_command(bool &success)
// Return true if the thing picked up is the golden idol.
{
	string result;
	int R = m_player->row();
	int C = m_player->col();
	// if it is the golden idol, win game!
	if (m_level == 4)
	{
		if (R == m_golden->row() && C == m_golden->col())
		{
			result += "You pick up " + m_golden->name() + "\n";
			result += "Congratulations, you won!\n";
			success = true;
			return result;
		}
	}
	// else if there is an object at this location, try to move it to inventory and remove it from dungeon
	for (int i = 0; i < m_nObject; i++)
	{
		if (m_object[i]->row() == R && m_object[i]->col() == C)
		{
			gameObject* temp = m_object[i];
			// if inventory can still hold, put it into inventory, and remove object
			if (m_player->addToInventory(temp))
			{
				result += "You pick up " + temp->name() + "\n";
				removeObject(i);
			}
			// otherwise, inventory is full, print out bag full and return false
			else
			{
				result += "Your knapsack is full; you can't pick that up.\n";
			}
			success = false;
			return result;
		}
	}
	return "";
}


string Dungeon::inventory_command(char command)
{
	string result;
	clearScreen();
	m_player->displayInventory();
	char temp = getCharacter();
	int index = (int)(temp - 97); // convert the characters to the corresponding integer index
	if (command == 'w')
		result = m_player->wieldWeapon(index);
	else if (command == 'r')
	{
		bool tele = false;
		result = m_player->readScroll(index, tele); // set tele to true only if the scroll is teleportation
		if (tele)
			teleport_command();
	}
	return result;
}


void Dungeon::cheat_command()
{
	m_player->cheat();
}


void Dungeon::teleport_command()
{
	int r, c;
	// ask Room to come up with a random movable location
	do
	{
		int index = randInt(m_nRoom);
		m_room[index]->randPoint(r, c);
	} while (isMovable(r, c));
	// move to that location
	m_player->move(r, c);
}


int Dungeon::downstair_command()
	// Return current if the Player is standing on the staircase.
	// Otherwise, returns -1.
{
	if (nextLevel(m_player->row(), m_player->col()))
		return m_level;
	else
		return -1;
}


bool Dungeon::canAttack(int pos)
{
	if (pos < 0 || pos >= m_nMonster)
		return false;
	Actor* temp = m_monster[pos];
	int distance = absolute(temp->row() - m_player->row()) + absolute(temp->col() - m_player->col());
	if (distance == 1)
		return true;
	else
		return false;
}


bool Dungeon::dumbSmell(int pos)
{
	if (pos < 0 || pos >= m_nMonster)
		return false;
	Actor* temp = m_monster[pos];
	int distance = absolute(temp->row() - m_player->row()) + absolute(temp->col() - m_player->col());
	if (distance <= temp->smellDistance())
		return true;
	else
		return false;
}


bool Dungeon::smartSmell(int sr, int sc, int er, int ec, int count, int &minimum, int &nextr, int &nextc)
	// Initial value of count should be 0; initial value minimum should be set to (smellDistance + 1);
	// (sr,sc) is the current searching position.
	// This function should have player's position as starting position, and monster's position as end position.
{
	// If the steps it takes to get here is already larger than the current minimal path, just quit searching on this path.
	if (count + 1 >= minimum)
		return false;
	
	// We make sure that we visit a location only if the path that gets us to this location is the shortest path known, by the following method:
	// Each time we visit a location, we inspect the character at this location.
	// If the character is smaller (in the sense that the integer representing this character is smaller) than or equal to current count,
	// we know that there is a previous path that gets to this location with fewer steps; therefore we can quit on our current path.
	// Else if the location is not movable (i.e. is occupied by another monster or wall), we also quit on our path.
	// Otherwise, we set the character to the one represented by the integer of current count.
	if ((int)m_maze[sr][sc] <= count)
		return false;
	else if (isNotMovable(sr, sc) && count != 0) // since when count == 0, the initial position will have the player and is always unmovable
		return false;
	m_maze[sr][sc] = (char)count;

	// Now, let's visit this location (sr,sc)
	// If you are standing right next to the target: 
	// update minimum to (count+1), which is the currently known shortest path to get to (er,ec),
	// update the closest position to target to the current location, and return true.
	if ((sr - 1 == er) && sc == ec)
	{
		minimum = count + 1;
		nextr = sr;
		nextc = sc;
		return true;
	}
	else if (sr == er && (sc + 1 == ec))
	{
		minimum = count + 1;
		nextr = sr;
		nextc = sc;
		return true;
	}
	else if ((sr + 1 == er) && sc == ec)
	{
		minimum = count + 1;
		nextr = sr;
		nextc = sc;
		return true;
	}
	else if (sr == er && (sc - 1 == ec))
	{
		minimum = count + 1;
		nextr = sr;
		nextc = sc;
		return true;
	}
	
	// Otherwise, you are not close to target. Call recursively on the four adjacent spots.
	// check coordinate at the North of current point
	bool result1 = smartSmell(sr - 1, sc, er, ec, count + 1, minimum, nextr, nextc);
	// check coordinate at the East of current point
	bool result2 = smartSmell(sr, sc + 1, er, ec, count + 1, minimum, nextr, nextc);
	// check coordinate at the Sorth of current point
	bool result3 = smartSmell(sr + 1, sc, er, ec, count + 1, minimum, nextr, nextc);
	// check coordinate at the West of current point
	bool result4 = smartSmell(sr, sc - 1, er, ec, count + 1, minimum, nextr, nextc);

	// If any of the four recursions found the target, return true; otherwise, return false.
	return result1 || result2 || result3 || result4;
}


bool Dungeon::isNotMovable(int r, int c) const
{
	char target = m_maze[r][c];
	if (target == '#' || target == 'B' || target == 'S' || target == 'D' || target == 'G' || target == '@')
		return true;
	else
		return false;
}



//////////////////////////////////////////
// Dungeon populator
//////////////////////////////////////////
bool Dungeon::initializeDungeon()
{
	buildWall();
	makeConnectedRoom();
	if (m_level != 4)
		createStair();
	else
		createGolden();
	return true;
}


Room* Dungeon::createRoom(int r, int c, int h, int w)
	// This function creates a pseudo-random room based on the fed parameters.
{
	if (r < 0 || c < 0 || h <= 0 || w <= 0)
		return nullptr;
	// Generate an upperleft corner at location around (r,c)
	// and generate a room at with that upperleft corner, whose height and width are smaller than but close to h and w
	int R = r + randInt(3);
	int C = c + randInt(4);
	int H = 4 + randInt(h - 4);
	int W = 4 + randInt(w - 4);
	// If the parameters generated would result in a room that touches/crosses the border of dungeon, return nullpointer
	if (R + H >= 17 || C + W >= 69)
		return nullptr;
	// Otherwise, generate the new room and return a pointer to it
	Room* ptr = new Room(R, C, H, W);
	return ptr;
}


void Dungeon::makeConnectedRoom()
	// This function makes use of createRoom function and creates a series of pseudo-random rooms that are connected.
{
	// Imagine the entire dungeon is divided into 3 vertical sections;
	// The first section contains column number 0-20, the second 21-50, the third 51-69;
	// The following generators will generate a series of pseudo-random rooms;
	// Each section will contain equally likely 1 or 2 rooms
	if (trueWithProbability(0.5)) // one room in section 1
	{
		m_room[m_nRoom] = createRoom(2, 2, 12, 15);
		if (m_room[m_nRoom] != nullptr)
			m_nRoom++;
	}
	else // two rooms in section 1
	{
		m_room[m_nRoom] = createRoom(1, 1, 6, 15);
		if (m_room[m_nRoom] != nullptr)
			m_nRoom++;
		m_room[m_nRoom] = createRoom(11, 1, 6, 15);
		if (m_room[m_nRoom] != nullptr)
			m_nRoom++;
	}
	if (trueWithProbability(0.5)) // one room in section 2
	{
		m_room[m_nRoom] = createRoom(2, 22, 12, 24);
		if (m_room[m_nRoom] != nullptr)
			m_nRoom++;
	}
	else // two rooms in section 2
	{
		m_room[m_nRoom] = createRoom(1, 21, 6, 24);
		if (m_room[m_nRoom] != nullptr)
			m_nRoom++;
		m_room[m_nRoom] = createRoom(11, 21, 6, 24);
		if (m_room[m_nRoom] != nullptr)
			m_nRoom++;
	}
	if (trueWithProbability(0.5)) // one room in section 3
	{
		m_room[m_nRoom] = createRoom(2, 52, 12, 15);
		if (m_room[m_nRoom] != nullptr)
			m_nRoom++;
	}
	else // two rooms in section 3
	{
		m_room[m_nRoom] = createRoom(1, 51, 6, 15);
		if (m_room[m_nRoom] != nullptr)
			m_nRoom++;
		m_room[m_nRoom] = createRoom(11, 51, 6, 15);
		if (m_room[m_nRoom] != nullptr)
			m_nRoom++;
	}
	// For every set of two rooms, pick two random points, one in each room;
	// Set up (1 or 2) corridor(s), which is a room with height/width 1, that connects these two random points.
	// This guarantees that every two rooms are connected, and hence all the rooms in the dungeon are connected.
	int roomnumber = m_nRoom;
	for (int i = 0; i < roomnumber - 1; i++)
	{
		int r1, c1;
		m_room[i]->randPoint(r1, c1);
		int r2, c2;
		m_room[i + 1]->randPoint(r2, c2);
		// A point that can extend a horizontal/vertical line to both these two points is (r2, c1).
		// If we create a corridor from both these points to (r2, c1), we connect the two rooms.
		if (r1 == r2)
		{
			m_room[m_nRoom] = new Room(r1, min(c1, c2), 1, absolute(c1 - c2) + 1);
			m_nRoom++;
		}
		else if (c1 == c2)
		{
			m_room[m_nRoom] = new Room(min(r1, r2), c1, absolute(r1 - r2) + 1, 1);
			m_nRoom++;
		}
		else
		{
			// corridor from (r1,c1) to (r2,c1)
			m_room[m_nRoom] = new Room(min(r1, r2), c1, absolute(r1 - r2) + 1, 1);
			m_nRoom++;
			// corridor from (r2,c1) to (r2,c2)
			m_room[m_nRoom] = new Room(r2, min(c1, c2), 1, absolute(c1 - c2) + 1);
			m_nRoom++;
		}
	}
	// update the character m_maze[18][70] to reflect all the rooms in array m_room[].
	for (int k = 0; k < m_nRoom; k++)
		buildRoom(k);
}


bool Dungeon::createStair()
{
	int index = randInt(m_nRoom);
	m_room[index]->randPoint(m_stairR, m_stairC);
	m_maze[m_stairR][m_stairC] = '>';
	return true;
}


bool Dungeon::createGolden()
{
	int r, c;
	int index = randInt(m_nRoom);
	m_room[index]->randPoint(r, c);
	m_golden = new Golden(r, c);
	m_maze[r][c] = m_golden->icon();
	return true;
}


bool Dungeon::createObject()
{
	int n = trueWithProbability(0.5) + 2;
	for (int i = 0; i < n; i++)
	{
		int r, c;
		int index = randInt(m_nRoom);
		m_room[index]->randPoint(r, c);
		if (trueWithProbability(1.0 / 7))
			addObject(r, c, 'A');
		else if (trueWithProbability(1.0 / 7))
			addObject(r, c, 'D');
		else if (trueWithProbability(1.0 / 7))
			addObject(r, c, 'H');
		else if (trueWithProbability(1.0 / 7))
			addObject(r, c, 'S');
		else if (trueWithProbability(1.0 / 7))
			addObject(r, c, 'l');
		else if (trueWithProbability(1.0 / 7))
			addObject(r, c, 's');
		else 
			addObject(r, c, 'm');
	}
	return true;
}


bool Dungeon::createMonster()
{
	for (int i = 0; i < m_maxMonster; i++)
	{
		// generate a random coordinate (r,c) in a random room
		int r, c;
		int index = randInt(m_nRoom);
		m_room[index]->randPoint(r, c);
		int prob = randInt(4);
		switch (prob)
		{
		case 0:
			if (m_level >= 2)
			{
				addMonster(r, c, 'B');
				break;
			}
			
		case 1:
			addMonster(r, c, 'G');
			break;
			// if level is deeper than 1, case 0 will create Bogeyman;
			// otherwise, case 0 will fall through and create Goblin;

		case 2:
			if (m_level >= 3)
			{
				addMonster(r, c, 'D');
				break;
			}

		case 3:
			addMonster(r, c, 'S');
			break;
			// if level is deeper than 2, case 2 will create Dragon;
			// otherwise, case 2 will fall through and create Snakewoman;

		default:
			exit(999);
		}
	}
	return true;
}


bool Dungeon::createPlayer()
{
	int r, c;
	// get a valid coordinate for Player to be placed at
	do
	{
		// generate a random coordinate (r,c) in a random room
		int index = randInt(m_nRoom);
		m_room[index]->randPoint(r, c);
	} while (!isMovable(r, c));
	m_player = new Player(r, c, this);
	m_maze[r][c] = m_player->icon();
	return true;
}


////////////////////////////////////////
// Monsters and objects operator
////////////////////////////////////////
bool Dungeon::addMonster(int r, int c, char id) 
{
	if (m_nMonster >= m_maxMonster)
		return false;
	if (!isMovable(r, c))
		return false;
	switch (id)
	{
	case 'B':
		m_monster[m_nMonster] = new Bogeyman(r, c, this);
		break;

	case 'D':
		m_monster[m_nMonster] = new Dragon(r, c, this);
		break;

	case 'G':
		m_monster[m_nMonster] = new Goblin(r, c, this);
		break;

	case 'S':
		m_monster[m_nMonster] = new Snakewoman(r, c, this);
		break;
		
	default:
		exit(6);
	}
	m_maze[r][c] = m_monster[m_nMonster]->icon();
	m_nMonster++;
	return true;
}


bool Dungeon::removeMonster(int pos) 
{
	if (pos < 0 || pos >= m_nMonster)
		return false;
	// delete the dynamically allocated monster
	delete m_monster[pos];
	for (int i = pos; i < m_nMonster - 1; i++)
	{
		m_monster[i] = m_monster[i + 1];
	}
	m_nMonster--;
	return true;
}


bool Dungeon::removeMonster(int r, int c)
{
	bool result = false;
	for (int i = 0; i < m_nMonster; i++)
	{
		if (m_monster[i]->row() == r && m_monster[i]->col() == c)
			result = removeMonster(i);
		break;
	}
	return result;
}


bool Dungeon::addObject(int r, int c, char id)
{
	if (m_nObject >= m_maxObject)
		return false;
	char destination = m_maze[r][c];
	if (destination == '#' || destination == '>' || destination == '&' || destination == ')' || destination == '?')
		return false;
	switch (id) 
		// Uppercase indicates scrolls, while lowercase indicates weapons.
		// '&' indicates the golden idol.
	{
	case 'A':
	case 'D':
	case 'H':
	case 'S':
	case 'T':
		m_object[m_nObject] = new Scroll(r, c, id);
		break;

	case 'a':
		m_object[m_nObject] = new M_axe(r, c);
		break;

	case 'f':
		m_object[m_nObject] = new M_fang(r, c);
		break;

	case 'l':
		m_object[m_nObject] = new L_sword(r, c);
		break;

	case 'm':
		m_object[m_nObject] = new Mace(r, c);
		break;

	case 's':
		m_object[m_nObject] = new S_sword(r, c);
		break;

	default:
		exit(7);
	}
	m_maze[r][c] = m_object[m_nObject]->icon();
	m_nObject++;
	return true;
}


bool Dungeon::removeObject(int pos) 
	// NOTE: this function will not delete the object, but merely remove it from the m_object list!!!
	// Calling this function should be immediately followed by adding the object to player inventory to avoid memory leak.
{
	if (pos < 0 || pos >= m_nObject)
		return false;
	for (int i = pos; i < m_nObject - 1; i++)
	{
		m_object[i] = m_object[i + 1];
	}
	m_nObject--;
	return true;
}


bool Dungeon::removeObject(int r, int c)
	// NOTE: this function will not delete the object, but merely remove it from the m_object list!!! 
	// Calling this function should be immediately followed by adding the object to player inventory to avoid memory leak.
{
	bool result = false;
	for (int i = 0; i < m_nObject; i++)
	{
		if (m_object[i]->row() == r && m_object[i]->col() == c)
			result = removeObject(i);
		break;
	}
	return result;
}


bool Dungeon::dropObject(int r, int c, char icon)
{
	// if there is an object at the position, return false
	for (int i = 0; i < m_nObject; i++)
	{
		if (m_object[i]->row() == r && m_object[i]->col() == c)
			return false;
	}
	// if there is a golden idol or a stair at the position, return false
	if (m_level == 4)
	{
		if (m_golden->row() == r && m_golden->col() == c)
			return false;
	}
	else
	{
		if (m_stairR == r && m_stairC == c)
			return false;
	}
	// Determine what object, if any, to drop in the dungeon.
	// Then call addObject to add the object to the dungeon.
	int result;
	switch (icon)
	{
	case 'B':
		if (trueWithProbability(0.1))
		{
			return addObject(r, c, 'a');
		}
		return false;

	case 'S':
		if (trueWithProbability(1.0 / 3))
		{
			return addObject(r, c, 'f');
		}
		return false;

	case 'D':
		result = randInt(5);
		switch (result)
		{
		case 0:
			return addObject(r, c, 'A');

		case 1:
			return addObject(r, c, 'D');

		case 2:
			return addObject(r, c, 'H');

		case 3:
			return addObject(r, c, 'S');

		default:
			return addObject(r, c, 'T');
		}

	case 'G':
		if (trueWithProbability(1.0 / 3))
		{
			if (trueWithProbability(0.5))
				return addObject(r, c, 'a');
			else
				return addObject(r, c, 'f');
		}
		return false;

	default:
		exit(1123);
	}
}


////////////////////////////////////////
// Maze Operators
////////////////////////////////////////
void Dungeon::updateMaze()
	// This function update m_maze[18][70] to reflect the most current layout of the dungeon, since m_maze might be outdated.
{
	// show all rooms
	for (int i = 0; i < m_nRoom; i++)
	{
		buildRoom(i);
	}
	// update stair or golden idol
	if (m_level != 4)
		m_maze[m_stairR][m_stairC] = '>';
	else
		m_maze[m_golden->row()][m_golden->col()] = m_golden->icon();
	// update all objects
	for (int i = 0; i < m_nObject; i++)
	{
		int r = m_object[i]->row();
		int c = m_object[i]->col();
		m_maze[r][c] = m_object[i]->icon();
	}
	// update all monsters
	for (int i = 0; i < m_nMonster; i++)
	{
		int r = m_monster[i]->row();
		int c = m_monster[i]->col();
		m_maze[r][c] = m_monster[i]->icon();
	}
	// put in the player
	m_maze[m_player->row()][m_player->col()] = m_player->icon();
}


void Dungeon::displayMaze() const
{
	for (int i = 0; i < 18; i++)
	{
		for (int k = 0; k < 70; k++)
		{
			cout << m_maze[i][k];
		}
		cout << endl;
	}
}


////////////////////////////////////////
// Build-up Operators
////////////////////////////////////////
void Dungeon::buildWall()
	// Fill the m_maze with walls.
{
	for (int i = 0; i < 18; i++)
	{
		for (int k = 0; k < 70; k++)
		{
			m_maze[i][k] = '#';
		}
	}
}


void Dungeon::buildRoom(int index)
	// Update m_maze[18][70] to reflect m_room[i] by changing all characters within m_room[i] to " ".
{
	int h = m_room[index]->height();
	int w = m_room[index]->width();
	int r = m_room[index]->row();
	int c = m_room[index]->col();
	for (int i = r; i < r + h; i++)
	{
		for (int k = c; k < c + w; k++)
		{
			m_maze[i][k] = ' ';
		}
	}
}


////////////////////////////////////////
// Build-up Operators
////////////////////////////////////////
void Dungeon::displayStatus() const
{
	cout << "Dungeon Level: " << m_level << ", Hit points: " << m_player->hp() << ", Armor: " << m_player->armor() << ", Strength: " << m_player->strength() << ", Dexterity: " << m_player->dex() << endl << endl;
}

