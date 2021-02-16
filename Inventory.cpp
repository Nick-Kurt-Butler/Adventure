# include <iostream>
# include <vector>
# include "Inventory.h"
using namespace std;

// returns true if inventory has item
bool Inventory::has(string item) {
	for (string s : items) {
		if (s == item)
			return true;
	}
	return false;
}

vector<string> Inventory::getItems() {
	return items;
}

bool Inventory::addItem(string item) {
	if (isFull())
		return false;
	items.push_back(item);
	return true;
}

// removes item from inventory
bool Inventory::dropItem(string item) {
	if (isEmpty())
		return false;
	for (int i = 0; i < items.size(); i++) {
		if (items[i] == item)
			items.erase(items.begin()+i);
	}
	return true;
}

// true if inventory is empty
bool Inventory::isEmpty() {
	if (items.size())
		return false;
	return true;
}

// true if inventory is full
bool Inventory::isFull() {
	if (items.size() == 3)
		return true;
	return false;
}

// prints items in inventory, maximum three
void Inventory::printItems() {
	if (items.size() == 0)
		cout << "You have no items in your inventory" << endl;
	else
		cout << "Here is a list of items in your inventory..." << endl;
	for (string item : items)
		cout << item << endl;
}
