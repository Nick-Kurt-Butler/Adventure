# include <iostream>
# include "Player.h"
# include "Map.h"
# include "Logic.h"
using namespace std;

// constructor that give class access to change game variables
// such as player, map and verb (i.e. go, grab, etc)
Logic::Logic(Player* sPlayer, Map* sMap, string* sVerb) {
	player = sPlayer;
	map = sMap;
	verb = sVerb;
}

// Rand probability generator, takes number between 0 and 1
// for example a prob value of .4 would mean there is a 40%
// chance of the function returning true.
bool Logic::probability(double prob) {
        double randNum = (double) rand()/RAND_MAX;
        if (randNum <= prob)
                return true;
        return false;
}

// simpily changes a string to a maximum of 5 lowercase letters
void lower(string& str) {
	for (int i = 0; str[i]; i++)
		str[i] = tolower(str[i]);
	str = str.substr(0,5);
}

// returns the nouns typed after the verb by the player
// this is what makes the two words commands work (i.e. "go south")
string getNoun() {
	string item;
	cin >> item;
	lower(item);
	return item;
}

// this is a generic function to move through the map
// if a more complicate structure is needed then an
// idiosyncratic function can be made tailored to the location
void Logic::go() {
	if (*verb == "go") {
		map->move(getNoun());
	}
}

// Engine to deal damage to player.  inflicts damage that check to see
// if lives is lost, then checks to see if player lost all three lives
void Logic::damage(int num) {
	if (player->damage(num)) { // if player dies he moves outside
		map->setCurRoom("outside");
		if (!player->isDead()) { // if player loses all three lives do not print outside
			*verb = "describe";
			outside();
		}
	}
}

bool Logic::outside() {
	Room* r = map->search("outside");
	if (*verb == "describe") {
		if (r->progress == 0) {
	        	cout << "Hello, it seems you have been trapped inside a magical\n"
		     	     << "realm. I will be your eyes an ears to help you escape.\n"
			     << "For more directions type help and press enter.\n" << endl;
			r->progress = 1;
		}
		// printed regardless
		cout << "You are outside. It is a gloomy day.  In front of you\n"
		     << "to the north you see a large dark castle.  Behind you\n"
		     << "to the south you see a dark cave and to the sides and\n"
		     << "all around there is a vast forest with no end in sight." << endl;
		if (r->has("shovel"))
			cout << "You look down and see a rusty shovel at your feet." << endl;
		if (r->progress != 2) // if axe has not been pryed with prybar
			cout << "You also see an axe stuck in a stump by the forest." << endl;
		if (r->has("axe")) // If the axe has been freed and is outside
			cout << "You also see the axe you unwedged from the stump." << endl;
	}
	if (*verb == "use") {
		string item = getNoun();
		if (item == "pryba" and player->getInven()->has("prybar") and r->progress == 1) {
			cout << "Using your prybar you wedge the axe free and leave it\n"
			     << "lying by the stump." << endl;
			r->items.push_back("axe");
			r->progress = 2;
			return true;
		}
	}
	go();
	return false;
}

bool Logic::cave() {
	Room* r = map->search("cave");
	if (!player->getInven()->has("lantern") and !r->has("lantern")) {
		string choice{};
		while (choice != "2" and map->getCurRoom() == "cave") {
			cout << "In the cave you find that you cannot see through the\n"
			     << "consuming darkness.  You must leave now or risk being\n"
			     << "injured.\n\n"
			     << "Would you like to\n"
			     << "1. Look for clues\n"
			     << "2. Leave now\n"
			     << ">> ";
			cin >> choice;
			cout << endl;
			if (choice == "1") {
				cout << "As you wander deeper into the cave" << endl;
				if (probability(0.8)) {
					cout << "without any light you stumble on a rock\n"
					     << "and injure you yourself." << endl;
					damage(10);
				} else {
					cout << "you find nothing helpful." << endl;
				}
			} else if (choice == "2") {
				cout << "You quickly leave the cave to return outside." << endl;
				map->setCurRoom("outside");
			} else {
				cout << "Invalid Option. Please enter 1 or 2." << endl;
			}
		}
	} else {
		if (*verb == "describe") {
			cout << "Using your lantern you can now see the walls of the cave\n"
			     << "On one of the walls has a scribbled message written on it." << endl;
			if (r->progress == 0) // Stage 1: rope has not been cut
				cout << "Near you there is a rope near leading down into a pit.\n"
				     << "It is tied very tightly to a rock." << endl;
			else if (r->has("rope")) // If rope is in the cave
				cout << "Near you there is a rope tied loosely to a rock leading\n"
				     << "down into a pit." << endl;
			cout << "To the west there is a long and dark tunnel in the distance you.\n"
			     << "think you hear small scuttled footsteps." << endl;
		}
	}
	if (*verb == "examine") {
		string item = getNoun();
		if (item == "messa" or item == "wall") {
			cout << "You look closely at the wall and see that the message says,\n"
			     << "    \"TO STAY HERE FOREVER\"" << endl;
			return true;
		}
	}

	if (*verb == "use") {
		string item = getNoun();
		if (player->getInven()->has("knife") and item == "knife" and r->progress == 0) {
			cout << "Using your knife you cut the rope from the rock"
			     << "and tie the rope loser so as to remove it later." << endl;
			map->addItem("rope");
			r->progress++;
		}
		return true;
	}
	if (*verb == "go") {
		string direction = getNoun();
		if (!player->getInven()->has("lantern") and (direction == "down" or direction == "west")) {
			cout << "I cannot let you go farther into the cave without any light\n"
			     << "to guide you. You would certainly die." << endl;
		} else if (direction == "down") {
			cout << "You begin to climb down the rope into a large dark pit." << endl;
			if (r->progress == 0) {
				map->move(direction);
			} else if (r->has("rope")) {
				if (probability(0.1)) {// 10% chance that player falls and dies
					cout << "It seems that since the rope was tied so loosely that\n"
					     << "it came off the rock and you fell down the pit." << endl;
					player->kill();
				} else {
					map->move(direction);
				}
			} else {
				cout << "There is no way to get down off the precipice into the pit." << endl;
			}
		} else {
			map->move(direction);
		}
	}
	return false;
}


bool Logic::castle() {
	if (*verb == "describe")
	        cout << "You enter a large castle.  The front gate is ajar and the\n"
		     << "edifice appears to be untouched by a living hand for decades.\n"
		     << "You look to you left and see a library full of books. To your\n"
		     << "right you see a kitchen fill with light from many windows." << endl;
	go();
	return false;
}

bool Logic::pit() {
	Room* r = map->search("pit");
	if (*verb == "describe") {
		cout << "You are in a somewhat small pit.  The bottom is very flat and\n"
		     << "you can smell the pungent oder of damp earth." << endl;
		if (r->progress == 0)
			cout << "You notice that the dirt on one part of the pit floor is\n"
			     << "not as flat as the rest." << endl;
		if (r->progress == 1)
			cout << "A large wooden chest lies in the hole that you dug with your\n"
			     << "shovel.  There is a small key hole that appears to unlocked\n"
			     << "the chest." << endl;
		if (r->progress == 2)
			cout << "A large wooden chest lies in front of you unlocked." << endl;
		if (r->has("gem"))
			cout << "The chest now lies wide open. The inside is a soft red velvet\n"
			     << "material and inside is a large sparkling green stone." << endl;
	}
	if (*verb == "use") {
		string item = getNoun();
		if (item == "shove" and player->getInven()->has("shovel") and r->progress == 0) {
			cout << "Using your shovel you dig the soft spot in the earth. It takes\n"
			     << "some work but you uncover a large wooden chest." << endl;
			r->progress = 1;
		}
		if (item == "key" and player->getInven()->has("key") and r->progress == 1) {
			cout << "Taking out yor key you place in the wooden chest's key hole.\n"
			     << "It fits perfectly." << endl;
			r->progress = 2;
		}
		return true;
	}
	if (*verb == "open") {
		string item = getNoun();
		if (item == "chest") {
			r->progress = 3;
			cout << "The chest now lies wide open. The inside is a sofe red velvet\n"
			     << "material and inside is a large sparkling green gem." << endl;
			map->addItem("gem");
			return true;
		}
	}
	if (*verb == "close") {
		string item = getNoun();
		if (item == "chest") {
			r->progress = 2;
			cout << "A large wooden chest lies in front of you unlocked." << endl;
			return true;
			if (r->has("gem"))
				map->removeItem("gem");
		}
	}
	if (*verb == "examine") {
		string item = getNoun();
		if (item == "floor") {
			cout << "When you examine the part of the floor that is lumpy, you find\n"
			     << "that the dirt is soft but not soft enough to dig with bare hands." << endl;
			return true;
		}
	}
	if (*verb == "go") {
		string direction = getNoun();
		if (!player->getInven()->has("lantern") and direction == "up")
			cout << "I cannot let you move through the cave without any light\n"
			     << "to guide you. You would certainly die." << endl;
		else
			map->move(direction);
	}
	return false;
}

bool Logic::tunnel() {
	Room* r = map->search("tunnel");
	if (*verb == "describe") {
        	cout << "Inside the dark tunnel the footsteps are louder and you\n"
		     << "begin to feel concerned." << endl;
		if (r->progress == 0) { // this only happens once each time player enters tunnel
			cout << "suddenly a small goblin with a crossbow begins to\n"
			     << "attack you.\n" << endl;
			while (map->getCurRoom() == "tunnel") {
				cout << "What do you do?\n"
				     << "1. Run away\n"
				     << "2. Fight\n>> ";
				string choice = getNoun();
				if (choice == "run" or choice == "1") {
					cout << "You run away but are barly snagged by and arrow." << endl;
					damage(10);
					map->setCurRoom("cave");
					*verb = "describe";
					cave();
				} else if (choice == "fight" or choice == "2") {
					if (player->getInven()->has("axe")) {
						cout << "Using your axe you kill the gobin in one swift swing." << endl;
						if (probability(0.1)) { // 10% probability of damage
							cout << "Your fight allowed the goblin to shoot one arrow before his fall." << endl;
							damage(15);
						} else {
							cout << "Your quick decision to fight left you unscathed." << endl;
						}
					} else {
						cout << "Though you have no good weapon you choose to fight." << endl;
						if (probability(0.5)) {
							cout << "Your skirimish with the goblin did not go well and he\n"
							     << "fired a shot into you abdomen." << endl;
							damage(80);
						} else {
							cout << "You and the goblin struggled but in the end you came out on top\n"
							     << "and the goblin lie before you dead.  Though you did not go unscathed." << endl;
							damage(35);
						}
					}
					r->progress = 1;
					break;
				} else {
					cout << "Invalid choice.\n" << endl;
				}
			}
		}
	}
	if (*verb == "go") {
		string direction = getNoun();
		if (!player->getInven()->has("lantern") and (direction == "west" or direction == "east"))
			cout << "I cannot let you move through the cave without any light\n"
			     << "to guide you. You would certainly die." << endl;
		else
			map->move(direction);
		r->progress = 0; // so that when player enters the tunnel again the goblin attacks.
	}
	return false;
}

bool Logic::spring() {
	Room* r = map->search("spring");
	if (*verb == "describe") {
		if (r->progress == 0) {
			cout << "You enter a large cavern and hear the rushing waters of a\n"
			     << "waterfall.  You see something sparking in the distance but\n"
			     << "you cannot quite make out what it is from this distance.\n"
			     << "In your way is a powerful geyser that seems to have words\n"
			     << "on it." << endl;
		} else if (r->progress == 1) {
			cout << "You are in a large cavern with spring waters rushing everywhere.\n"
			     << "Now that the geyser is gone you see something shiny up\n"
			     << "above on a cliff.  You cannot reach or see it from here and\n"
			     << "the cliff is to dangerous to climb without tools." << endl;
		} else if (r->progress == 2) {
			cout << "You are in a large cavern and see that there are waters\n"
			     << "rushing everywhere." << endl;
		}
	}
	if (*verb == "use") {
		string item = getNoun();
		if (item == "rope" and player->getInven()->has("rope") and r->progress == 1) {
			cout << "Using you rope you are able to climb the cliff safely\n"
			     << "and see that the shiny object is a brass key." << endl;
			map->addItem("key");
			r->progress = 2;
			return true;
		}
	}
	if (*verb == "examine") {
		string item = getNoun();
		if (item == "geyse") {
			cout << "On the gyser is written, \"WHAT IS YOUR QUEST?\"\n"
			     << "What is you response?\n>> ";
			string password{};
			getline(cin,password);
			getline(cin,password);
			if (password == "TO STAY HERE FOREVER") {
				cout << "Your response seems to have done something.\n"
				     << "The gyser begins to shrink to the ground to\n"
				     << "to let you pass." << endl;
				r->progress = 1;
			} else {
				cout << "Your response appears to have done nothing." << endl;
			}
			return true;
		}
	}
	if (*verb == "go") {
		string direction = getNoun();
		if (!player->getInven()->has("lantern") and direction == "up")
			cout << "I cannot let you move through the cavern without any light\n"
			     << "to guide you. You would certainly die." << endl;
		else
			map->move(direction);
	}
	return false;
}

bool Logic::forest() {
	Room* r = map->search("forest");
	if (*verb == "describe")
		if (r->progress == 3) {
			cout << "After wandering in the forest for a while you find a prybar\n"
			     << "lying next to a tree" << endl;
			map->addItem("prybar");
			r->progress = 4;
		} else if (r->has("prybar")) {
			cout << "You are in a forest and see a prybar next to a tree." << endl;
		} else {
			cout << "You are in a forest." << endl;
		}
	if (*verb == "go" and r->progress < 3)
		r->progress++;
	go();
	return false;
}

bool Logic::kitchen() {
	Room* r = map->search("kitchen");
	if (*verb == "describe") {
        	cout << "You are in a large kitchen.  It has many utensils.  However, there\n"
		     << "All the cabinets appear to have been raided by wild animals." << endl;
		if (r->has("knife"))
			cout << "While taking in the room you trip over a small knife on the ground\n"
			     << "and almost hurt yourself." << endl;
	}
	go();
	return false;
}

bool Logic::library() {
	Room* r = map->search("library");
	if (*verb == "describe") {
	        cout << "You are in a library filled to the brim with dusty books.\n"
		     << "You look around for clues but begin to think you might have\n"
		     << "a better chance elsewhere.  There is a desk covered with papers";
		if (r->has("lantern"))
			cout << "\nand a lantern that appears to be functional";
		cout << "." << endl;
	}
	go();
	return false;
}
