# include <string>
using namespace std;

#ifndef MAP_H
#define MAP_H

struct Room;

// This object represents a connection between two rooms
// it contains a direction of how to enter room B from Room A
// and a boolean for if the room has been unlocked by the player
// or the progression of the game
struct Connection {
        Room* r;
        string direction;
        Connection(){}
        Connection(Room* sR, string sDir) {
                r = sR;
                direction = sDir;
        }
};

// This object represents a location that the player can be in
// it contains the room's name, the items in the room, the
// other rooms this room is connected to, and how many times the player
// has visited the room
struct Room {
        string name;
        vector<string> items;
        vector<Connection> connections;
        int progress;
        Room(){}
        Room(string cName) {
                name = cName;
                progress = 0;
        }
	bool has(string item) {
		for (string i : items) {
			if (i == item)
				return true;
		}
		return false;
	}
};

// This object controls the position of the player and
// how the rooms are connected into one map
class Map {
	private:
		vector<Room*> rooms;
		Room* curRoom; // for the current location of the player
		// makes a connection between Room A and B with direction C
	public:
		Map(); // default constructor, loads a map from string
		Room* search(string name); // returns address of specified room
		string getCurRoom();
		void setCurRoom(string);
		vector<Room*> getRooms();
		bool move(string direc); // moves a player to new room
		void printDirections(); // prints available directions
		void addItem(string); // adds item to current room
		void removeItem(string); // removes item from curRoom
		bool roomHasItem(string); // returns true if item is in curRoom
};
#endif

