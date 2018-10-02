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
	char getSquare(int x, int y) {
		return field[x][y];
	}

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

	void insert_ship(int x, int y, int length, char ship, int aligment, char surrounding) {

		if (aligment == 1)
			for (int i = x; i < x + length; i++) {
				field[i][y] = ship;

				if (!(y + 1 >= BOARD_SIZE) && (field[i][y + 1] != ship))
					field[i][y + 1] = surrounding;
				if (!(y - 1 < 0) && (field[i][y - 1] != ship))
					field[i][y - 1] = surrounding;
				if (!(i + 1 >= BOARD_SIZE) && (field[i + 1][y] != ship))
					field[i + 1][y] = surrounding;
				if (!(i - 1 < 0) && (field[i - 1][y] != ship))
					field[i - 1][y] = surrounding;

				if (!(y + 1 >= BOARD_SIZE) && !(i + 1 >= BOARD_SIZE))
					field[i + 1][y + 1] = surrounding;
				if (!(y - 1 < 0) && !(i + 1 >= BOARD_SIZE))
					field[i + 1][y - 1] = surrounding;
				if (!(y + 1 >= BOARD_SIZE) && !(i - 1 < 0))
					field[i - 1][y + 1] = surrounding;
				if (!(y - 1 < 0) && !(i - 1 < 0))
					field[i - 1][y - 1] = surrounding;
			}
		else
			for (int i = y; i < y + length; i++) {
				field[x][i] = ship;

				if (!(x + 1 >= BOARD_SIZE) && (field[x + 1][i] != ship))
					field[x + 1][i] = surrounding;
				if (!(x - 1 < 0) && (field[x - 1][i] != ship))
					field[x - 1][i] = surrounding;
				if (!(i + 1 >= BOARD_SIZE) && field[x][i + 1] != ship)
					field[x][i + 1] = surrounding;
				if (!(i - 1 < 0) && field[x][i - 1] != ship)
					field[x][i - 1] = surrounding;

				
				if (!(x + 1 >= BOARD_SIZE) && !(i + 1 >= BOARD_SIZE))
					field[x + 1][i + 1] = surrounding;
				if (!(x - 1 < 0) && !(i + 1 >= BOARD_SIZE))
					field[x + 1][i - 1] = surrounding;
				if (!(x + 1 >= BOARD_SIZE) && !(i - 1 < 0))
					field[x - 1][i + 1] = surrounding;
				if (!(x - 1 < 0) && !(i - 1 < 0))
					field[x - 1][i - 1] = surrounding;
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
	ship(std::string name, int size, int count) {
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

void set_up_board(game_field & player, game_field & computer);
void players_turn(game_field & target, std::string name);
int constrain(int number, int min, int max);

int main() {

	game_field player('#');
	game_field computer('#');

	set_up_board(player, computer);


	system("pause");
	return 0;
}

void set_up_board(game_field & player, game_field & computer) {

	std::ifstream file("Zaidejas.txt");

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

		std::cout << "Zaidejo eile:\n";

		system("cls");
		std::cout << "Zaidejo lenta:\n";
		player.print_with_ships();

		std::cout << "laivai\n";
		for (int i = 0; i < ships.size(); i++) {
			std::cout << i + 1 << ". " << ships[i].getName() << ". Dydis: " << ships[i].getSize() << ". Liko: " << ships[i].getCount() << ".\n";
		}

		std::cout << "\nPasirinkite laiva:\n";
		int laivas;
		std::cin >> laivas;
		laivas = constrain(laivas, 1, ships.size());

		std::cout << "\nKaip ji pastatyti:\n";
		std::cout << "1. Horizontaliai (laivas judes i desine)\n2. Vertikaliai (laivas judes zemyn)\n";
		int lygiuote;
		std::cin >> lygiuote;
		lygiuote = constrain(lygiuote, 1, 2);

		int x;
		int y;

		bool validPos = false;
		while (!validPos) {

			std::cout << "\nx pozicija: ";
			std::cin >> x;
			x--;

			std::cout << "y pozicija: ";
			std::cin >> y;
			y--;

			if (lygiuote == 1) {
				for (int i = x; i < x + ships[laivas - 1].getSize(); i++) {
					if (player.getSquare(i, y) == 'O' || player.getSquare(i, y) == '*') {
						validPos = false;
						break;
					}
					else validPos = true;
				}
			}	
			else {
				for (int i = y; i < y + ships[laivas - 1].getSize(); i++) {
					if (player.getSquare(x, i) == 'O' || player.getSquare(x, i) == '*') {
						validPos = false;
						break;
					}
					else validPos = true;

				}
			}

			if (!validPos) {
				std::cout << "\nNegalima pozicija\n\n";
				system("pause");
				system("cls");

				player.print_with_ships();
			}

		}

		system("cls");
		player.insert_ship(x, y, ships[laivas - 1].getSize(), 'O', lygiuote, '*');

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

	//hit miss ship
	bool hit = target.hit(x, y, 'X', '*', '0');

	system("cls");
	hit ? std::cout << "Hit!\n" : std::cout << "Miss\n";
	system("pause");

	if (hit && !target.game_over('0'))
		players_turn(target, name);
}

int constrain(int number, int min, int max) {
	
	if (number < min)
		return min;
	else if (number > max)
		return max;
	else return number;

}