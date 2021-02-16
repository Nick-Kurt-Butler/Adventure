# include "Game.h"
# include <iostream>
# include <fstream>
using namespace std;

int main() {
	Game g;
	string clear = "clear";
	int choice = 1;
	bool play = false;
	// main menu to start to game
	while (choice != 4 and !play) {
		cout << "====The Quest====\n"
		     << "1. Start New Game\n"
		     << "2. Load Game\n"
		     << "3. Rules\n"
		     << "4. Quit\n>> ";
		cin >> choice;
		cin.ignore();
		cin.clear();
		switch (choice) {
			case 1: { // Starts a new game without loading previous data
				g.load("newGame");
				play = true;
				break;
			} case 2: { // loads previous game and starts play
				cout << "Input name of last game:" << endl;
				string fileName;
				cin >> fileName;
				ifstream file(fileName);
				if (file.is_open()) {
					g.load(fileName);
					play = true;
				} else {
					cout << "Invalid file name" << endl;
				}
				break;
			} case 3: { // Prints file HELP
				g.printHelp();
				cout << "\nPress enter to continue." << endl;
				cin.get();
				break;
			}
		}
		system(clear.c_str()); // clears screen
	}
	bool done = false;
	if (play) {
		// This is the main loop to play the game
		// the function Game::play() controls all other game logic
		g.setVerb("describe");
		g.play();
		while(!done) {
			if (g.getPlayer().getInven()->has("gem")) {
				cout << "You have obtained the great gem and "
				     << "wield the power to escape this place."
				     << endl;
				done = true;
				break;
			}
			if (g.getPlayer().isDead()) {
				cout << "\nSorry it seems you have died in your quest and failed your escape.\n" << endl;
				done = true;
				break;
			}
			// Main input print statement
			cout << ">> ";
			string choice{};
	                cin >> choice;
	                cin.clear();
			cin.ignore();
	                g.lower(choice); // makes all lowercase
			choice = choice.substr(0,5); // so that user only has to put 5 chars or more of choice
			if (choice == "descr" or choice == "show" or choice == "tell") { // for the user to redescribe the environment
				g.setVerb("describe");
				g.play();
			} else if (choice == "direc") { // prints possible directions
	                        g.getMap().printDirections();
	                }
	                else if (choice == "inven" or choice == "items") { // prints player inventory, can hold at most 3
	                        g.getPlayer().getInven()->printItems();
	                }
			else if (choice == "stats" or choice == "healt") { // print health, magic, and lives
				g.getPlayer().printStats();
			}
			else if (choice == "actio") { // prints possible actions
				cout << "The possible actions you may take are...\n"
				     << "Go\nMove\nGrab\nGet\nDrop\nUse\nExamine\nSee\nLook\nOpen\nClose\nShut" << endl;
	                }
			else if (choice == "help" or choice == "rules") { // print help page which gives instructions to player
				g.printHelp();
			}
			else if (choice == "clear") { // clears screen and prints out menu again
				system(clear.c_str());
			}
			else if (choice == "quit") { // proceeds to ~Game::Game() to save player progress
				done = true;
			}

			// remaining options are for actions in the game
			else if (choice == "go" or choice == "move") {
				g.setVerb("go");
				g.play();
				// environment is only described when a new location is reached
				g.setVerb("describe");
				g.play();
	                }
			else if (choice == "grab" or choice == "get") {
				g.grab();
			}
			else if (choice == "drop") {
				g.drop();
			}
	                else if (choice == "use") {
				g.setVerb("use");
				if (!g.play()) {
					cout << "That item is not possible to use here." << endl;
					getline(cin,choice);
				}
			}
			else if (choice == "exami" or choice == "see" or choice == "look") {
				g.setVerb("examine");
				if (!g.play()) {
					cout << "You cannot examine that." << endl;
					getline(cin,choice);
				}
			}
			else if (choice == "open") {
				g.setVerb("open");
				if (!g.play()) {
					cout << "You cannot open that." << endl;
					getline(cin,choice);
				}
			}
			else if (choice == "close" or choice == "shut") {
				g.setVerb("close");
				if (!g.play()) {
					cout << "You cannot close that." << endl;
					getline(cin,choice);
				}
			}
			else {
				cout << "Invalid Action" << endl;
			}
		}
	}
	cout << "Goodbye" << endl;
}

