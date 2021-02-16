# include "Inventory.h"
# include <string>
using namespace std;

#ifndef PLAYER_H
#define PLAYER_H

// This object hold the stats and progress of the player
class Player {
	private:
		int health;
		int lives;
		// pointer so that copies are not made
		// but rather the one inventory used in the game
		Inventory* inven;
	public:
		Player();
		Player(int,int);
		Inventory* getInven();
		int getHealth();
		void setHealth(int);
		int getLives();
		void setLives(int);
		void printStats(); // prints healt and lives
		bool damage(int num); // takes off health, returns true if player is killed
		void heal(int num); // adds on health
		void kill(); // takes off one lives
		bool isDead(); // true is player has no lives left
};

#endif
