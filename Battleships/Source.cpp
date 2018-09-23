#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>

#define BOARD_SIZE 9

class game_field {

private:
	char field[BOARD_SIZE][BOARD_SIZE];

public:
	game_field(char empty) {
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				field[i][j] = empty;
	}

	void print_with_ships() {

		std::cout << "  ";
		for (int i = 0; i < BOARD_SIZE; i++)
			std::cout << i + 1 << " ";
		std::cout << "\n";

		for (int y = 0; y < BOARD_SIZE; y++) {
			std::cout << y + 1 << " ";
			for (int x = 0; x < BOARD_SIZE; x++)
				std::cout << field[x][y] << " ";
			std::cout << "\n";
		}
	}

	void print_without_ships(char hit, char empty) {
		std::cout << "  ";
		for (int i = 0; i < BOARD_SIZE; i++)
			std::cout << i + 1 << " ";
		std::cout << "\n";

		for (int y = 0; y < BOARD_SIZE; y++) {
			std::cout << y + 1 << " ";
			for (int x = 0; x < BOARD_SIZE; x++)
				if (field[x][y] != hit)
					std::cout << empty << " ";
				else
					std::cout << hit << " ";
			std::cout << "\n";
		}
	}

	void insert_ship(int x, int y, int length, char ship, int aligment) {

		if (aligment == 1)
			for (int i = x - 1; i < x - 1 + length; i++) {
				field[i][y - 1] = ship;
			}
		else if (aligment == 2)
			for (int i = y - 1; i < y - 1 + length; i++) {
				field[x - 1][i] = ship;
			}
	}

	//X $ 0 
	bool hit(int x, int y, char hit, char miss, char ship) {

		if (field[x - 1][y - 1] == ship) {
			field[x - 1][y - 1] = hit;
			return true;
		}
		else {
			field[x - 1][y - 1] = miss;
			return false;
		}
	}

	bool game_over(char ship) {
		
		for (int y = 0; y < BOARD_SIZE; y++) {
			for (int x = 0; x < BOARD_SIZE; x++)
				if (field[x][y] == ship)
					return false;
		}

		return true;
	}
};

class ship {


private:
	std::string _name;
	int _size;
	int _count;
public:
	ship (std::string name, int size, int count) {
		_name = name;
		_size = size;
		_count = count;
	}

	std::string getName() {
		return _name;
	}

	int getSize() {
		return _size;
	}
	
	int getCount() {
		return _count;
	}

	void setName(std::string name) {
		_name = name;
	}

	void setSize(int size) {
		_size = size;
	}

	void setCount(int count) {
		_count = count;
	}

};

void set_up_board(game_field & field, std::string name);
void players_turn(game_field & target, std::string name);

int main() {

	game_field field1('#');
	game_field field2('#');
	
	set_up_board(field1, "Pirmo");
	//set_up_board(field2, "Antro");

	bool game_over = false;
	while (!game_over) {
		//Pirmo zaidejo eile 
		players_turn(field2, "Antro");
		
		//Antro zaidejo eile
		players_turn(field1, "Pirmo");
	}


	system("pause");
	return 0;
}

void set_up_board(game_field & field, std::string name) {

	std::ifstream file(name + ".txt");

	ship battleship("battleship", 4, 1);
	ship cruiser("cruiser", 3, 2);
	ship destroyer("destroyer", 2, 3);
	ship submarine("submarine", 1, 4);

	std::vector<ship> ships;
	ships.push_back(battleship);
	ships.push_back(cruiser);
	ships.push_back(destroyer);
	ships.push_back(submarine);


	while (!ships.empty()) {

		if (name == "Antro")
			std::cout << "Pirmo zaidejo eile:\n";
		else std::cout << "Antro zaidejo eile:\n";

		system("cls");
		std::cout << name << " zaidejo lenta:\n";
		field.print_with_ships();

		std::cout << "\n" << name << " zaidejo laivai\n";
		for (int i = 0; i < ships.size(); i++) {
			std::cout << i + 1 << ". " << ships[i].getName() << ". Dydis: " << ships[i].getSize() << ". Liko: " << ships[i].getCount() << ".\n";
		}

		std::cout << "\nPasirinkite laiva:\n";
		int laivas;
		//std::cin >> laivas;
		file >> laivas;

		std::cout << "\nKaip ji pastatyti:\n";
		std::cout << "1. Horizontaliai (laivas judes i desine)\n2. Vertikaliai (laivas judes zemyn)\n";
		int lygiuote;
		//std::cin >> lygiuote;
		file >> lygiuote;

		std::cout << "\nx pozicija: ";
		int x;
		//std::cin >> x;
		file >> x;

		std::cout << "y pozicija: ";
		int y;
		//std::cin >> y;
		file >> y;

		system("cls");
		field.insert_ship(x, y, ships[laivas - 1].getSize(), '0', lygiuote);

		ships[laivas - 1].setCount(ships[laivas - 1].getCount() - 1);
		if (ships[laivas - 1].getCount() <= 0) {
			ships.erase(ships.begin() + (laivas - 1));
		}
	}
}

void players_turn(game_field & target, std::string name) {

	system("cls");

	if (target.game_over('0')) {
		std::cout << "Antras zaidejas pralaimejo";
		exit(0);
	}

	std::cout << name << " lenta:\n";

	target.print_without_ships('X', '#');
	std::cout << "\n\n";
	target.print_with_ships();

	std::cout << "\nX: ";
	int x;
	std::cin >> x;

	std::cout << "Y: ";
	int y;
	std::cin >> y;


	//hit miss
	bool hit = target.hit(x, y, 'X', '$', '0');

	std::cout << hit ? "Hit!" : "Miss!";
	system("cls");

	if (hit && !target.game_over('0'))
		players_turn(target, name);


}