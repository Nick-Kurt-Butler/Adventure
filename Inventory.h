# include <vector>
using namespace std;

# ifndef INVENTORY_H
# define INVENTORY_H

class Inventory {
	private:
		vector<string> items;
	public:
		Inventory(){};
		bool isFull(); // returns true if inventory has 3 items
		bool isEmpty(); // returns true is inventory is empty
		bool has(string); // return true if inventory has item
		vector<string> getItems();
		bool addItem(string); // add items to vector
		bool dropItem(string); // removes items from vector
		void printItems(); // prints items in vector
};

# endif
