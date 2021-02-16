# include <string>
# include "Game.h"
# include "Player.h"
# include "Inventory.h"
# include <fstream>
# include <iostream>
# include <sstream>
using namespace std;

Game::Game() {
	verb = "describe";
	logic = Logic(&player,&map,&verb);
	gameLoaded = false;
}

// converts a str to all lower case by reference
// makes the if logic statements easier
void Game::lower(string &str) {
        for (int i = 0; str[i]; i++)
                str[i] = tolower(str[i]);
}

void Game::printHelp() {
	ifstream file("HELP");
	string line;
	while (getline(file,line))
		cout << line << endl;
}

// saves game by reading data to user chosen file
Game::~Game() {
	if (!gameLoaded) // if no game has been loaded there is nothing to save
		return;
	if (player.getInven()->has("gem")) // This is how to win the game
		return;
	if (player.isDead()) // If the player has died they must reset the game and progress is lost
		return;
	string fileName;
	cout << "Input a name to save game under." << endl;
	cin >> fileName;
	ofstream file(fileName);
	file << "health," << player.getHealth() << endl;
	file << "lives," << player.getLives() << endl;
	file << "inventory";
	for (string item : player.getInven()->getItems())
		file << "," << item;
	file << endl;
	file << "location," << map.getCurRoom() << endl;
	for (Room* r : map.getRooms()) {
		file << r->name << "," << r->progress;
		for (string item : r->items)
			file << ',' << item;
		file << endl;
	}
}

// loads game from file. default is data for new game
bool Game::load(string fileName) {
	ifstream file(fileName);
	if (!file.is_open())
		return false;
	string line;
	while (getline(file,line)) {
		stringstream ss(line);
		string item;
		getline(ss,item,',');
		if (item == "health") {
			getline(ss,item,',');
			player.setHealth(stoi(item));
		}
		else if (item == "lives") {
			getline(ss,item,',');
			cout << item << endl;
			player.setLives(stoi(item));
			cout << player.getLives() << endl;
		}
		else if (item == "inventory") {
			while (getline(ss,item,','))
				player.getInven()->addItem(item);
		}
		else if (item == "location") {
			getline(ss,item,',');
			map.setCurRoom(item);
		}
		else {
			Room* r = map.search(item);
			getline(ss,item,','); // gets progess member;
			r->progress = stoi(item);
			while (getline(ss,item,','))
				r->items.push_back(item);
		}
	}
	gameLoaded = true;
	return true;
}


Player Game::getPlayer() {
	return player;
}

void Game::setVerb(string cVerb) {
	verb = cVerb;
}

string Game::getVerb() {
	return verb;
}

Map Game::getMap() {
	return map;
}

// engine for dropping an item from player inventory and adding it to current room
void Game::drop() {
	string item{};
	cin >> item;
	if (player.getInven()->has(item)) {
		if (player.getInven()->dropItem(item)) {
			map.addItem(item);
			cout << item << " was removed from your inventory." << endl;
		} else {
			cout << "Your inventory is empty. There is nothing to drop." << endl;
		}
	} else {
		cout << "That item is not in your inventory." << endl;
		player.getInven()->printItems();
	}
}

// engine for grabbing an item, adding it to player inventory and removing it from current room
void Game::grab() {
	string item{};
	cin >> item;
	if (map.roomHasItem(item) and !player.getInven()->isFull()) {
		map.removeItem(item);
		player.getInven()->addItem(item);
		item[0] = toupper(item[0]); //capitalize first letter
		cout << item  << " successfully added to your inventory." << endl;
	} else if (!map.roomHasItem(item)) {
		cout << "That item is either not in this area or cannot be grabbed at this time." << endl;
	} else {
		cout << "Your inventory is full, item cannot be added." << endl;
	}
}


// Main function to control gameplay
bool Game::play() {
	if (map.getCurRoom() == "outside")
		return logic.outside();
	if (map.getCurRoom() == "forest")
		return logic.forest();
	if (map.getCurRoom() == "cave")
		return logic.cave();
	if (map.getCurRoom() == "pit")
		return logic.pit();
	if (map.getCurRoom() == "tunnel")
		return logic.tunnel();
	if (map.getCurRoom() == "spring")
		return logic.spring();
	if (map.getCurRoom() == "castle")
		return logic.castle();
	if (map.getCurRoom() == "library")
		return logic.library();
	if (map.getCurRoom() == "kitchen")
		return logic.kitchen();
	return false;
}
