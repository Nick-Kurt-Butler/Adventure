# include <iostream>
# include <sstream>
# include <vector>
# include "Map.h"
using namespace std;

// loads in map from file, see format in map.txt
Map::Map() {
	// text for map, short enough to make string instead of file
	string map =
	"outside:castle,north:forest,east:forest,west:cave,south\n"
	"cave:pit,down:tunnel,west:outside,outside\n"
	"pit:cave,up\n"
	"tunnel:cave,east:spring,west\n"
	"spring:tunnel,east\n"
	"forest:outside,up:forest,east:forest,west:forest,south:forest,north:forest,down\n"
	"castle:outside,outside:library,left:kitchen,right\n"
	"kitchen:castle,back\n"
	"library:castle,back";
	stringstream file(map);
	string line;
	while (getline(file,line)) {
		stringstream ss(line);
		Room *r1;
		string connect, loc, direc;
		for (int i = 0; getline(ss,connect,':'); i++) {
			if (i == 0) {
				r1 = search(connect);
			} else {
				stringstream ss2(connect);
				getline(ss2,loc,',');
				getline(ss2,direc,',');
				Room* r2 = search(loc);
				r1->connections.push_back(Connection(r2,direc));
			}
		}
	}
	curRoom = search("outside");
}

// sets current room based on name
void Map::setCurRoom(string name) {
	curRoom = search(name);
}

// returns the name of the current room
string Map::getCurRoom() {
	return curRoom->name;
}

vector<Room*> Map::getRooms() {
	return rooms;
}

// returns address of a room in the rooms vector
// is no room is found then it creates one
Room* Map::search(string name) {
	for (Room* r : rooms) {
		if (r->name == name)
			return r;
	}
	Room* r = new Room(name);
	rooms.push_back(r);
	return r;
}

// contols player location
bool Map::move(string direc) {
	for (Connection c : curRoom->connections) {
		if (c.direction.substr(0,5) == direc.substr(0,5)) {
			curRoom = c.r;
			return true;
		}
	}
	cout << "\rYou cannot go that direction.\n" << endl;
	return false;
}

// prints viable directions for the player to use
void Map::printDirections() {
	cout << "These are the directions you can go..." << endl;
	for (Connection c : curRoom->connections)
		cout << c.direction << endl;
}

void Map::addItem(string item) {
	curRoom->items.push_back(item);
}

bool Map::roomHasItem(string item) {
	for (string s : curRoom->items) {
		if (s == item)
			return true;
	}
	return false;
}

// removes item from vector of items specific to the current room
// this is called by Game::grab()
void Map::removeItem(string item) {
	for (int i = 0; i < curRoom->items.size(); i++) {
		if (curRoom->items[i] == item)
			curRoom->items.erase(curRoom->items.begin()+i);
	}
}

