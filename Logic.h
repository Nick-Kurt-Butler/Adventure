# include "Player.h"
# include "Map.h"
# include <string>
using namespace std;

#ifndef LOGIC_H
#define LOGIC_H

struct Logic {
	Player* player;
	Map* map;
	string* verb;
	bool outside();
	bool cave();
	bool pit();
	bool tunnel();
	bool spring();
	bool forest();
	bool castle();
	bool kitchen();
	bool library();
	Logic(){};
	Logic(Player*, Map*, string*);
	bool probability(double);
	void damage(int);
	void go();
};

#endif
