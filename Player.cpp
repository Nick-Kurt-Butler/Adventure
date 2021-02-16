# include <iostream>
# include "Inventory.h"
# include "Player.h"
using namespace std;

// default constructor, initializes private members
Player::Player() {
	inven = new Inventory;
	lives = 3;
	health = 100;
}

// constructor, initializes private members
Player::Player(int cLives, int cHealth) {
	inven = new Inventory;
	lives = cLives;
	health = cHealth;
}

Inventory* Player::getInven() {
	return inven;
}

//prints health, lives, and magic
void Player::printStats() {
	cout << "Health: " << health << endl;
	cout << "Lives Left: " << lives << endl;
}

// deals damage to player and prints how much was lost
// if damage kills player, then a life is subtracted
bool Player::damage(int num) {
	health -= num;
	cout << "\nYou have lost " << num << " points of your health." << endl;
	if (health <= 0) {
		kill();
		return true;
	}
	return false;
}

// gives health to player
void Player::heal(int num) {
	health += num;
}

// subtracts lives from player and resets health the 100
void Player::kill() {
	cout << "\nYou Died" << endl;
	lives--;
	health = 100;
}

// return true if player has no lives left
bool Player::isDead() {
	if (lives == 0)
		return true;
	return false;
}

int Player::getHealth() {
	return health;
}

void Player::setHealth(int cHealth) {
	health = cHealth;
}

int Player::getLives() {
	return lives;
}

void Player::setLives(int cLives) {
	lives = cLives;
}
