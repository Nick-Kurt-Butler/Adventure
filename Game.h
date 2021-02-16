# include <string>
# include "Player.h"
# include "Map.h"
# include "Logic.h"
using namespace std;

#ifndef GAME_H
#define GAME_H
class Game {
	private:
		Logic logic;
		Player player;
		Map map;
		string verb; // The action of the player
		bool gameLoaded;
	public:
		Game(); // loads map and initialized player and verb
		~Game(); // saves game to fileName.save
		Player getPlayer();
		Map getMap();
		void printHelp(); // print help.txt
		string getVerb();
		void lower(string&); //converts string to all lowercase
		void setVerb(string);
		bool load(string); // loads previous game data true if file exists
		bool play(); // this calls all private methods
		void drop(); // method to drop item
		void grab(); // method to add an item
};
#endif
