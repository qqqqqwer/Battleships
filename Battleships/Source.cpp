#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <limits>
#include <chrono>
#include <thread>

#define BOARD_SIZE 10

class game_field {

private:
	char field[BOARD_SIZE][BOARD_SIZE];

	char getSquare(int x, int y) {
		return field[x][y];
	}

public:
	
	game_field(char empty) {
		for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				field[i][j] = empty;
	}

	void print_with_ships() {

		std::cout << "  ";
		for (int i = 0; i < BOARD_SIZE; i++)
			std::cout << i << " ";
		std::cout << "\n";

		for (int y = 0; y < BOARD_SIZE; y++) {
			std::cout << y << " ";
			for (int x = 0; x < BOARD_SIZE; x++)
				std::cout << field[x][y] << " ";
			std::cout << "\n";
		}
	}

	void print_without_ships(char hit, char empty, char miss) {
		//X - hit ship
		//# - empty space
		//* - missed shot
		std::cout << "  ";
		for (int i = 0; i < BOARD_SIZE; i++)
			std::cout << i << " ";
		std::cout << "\n";

		for (int y = 0; y < BOARD_SIZE; y++) {
			std::cout << y << " ";
			for (int x = 0; x < BOARD_SIZE; x++)
				if (field[x][y] == hit)
					std::cout << hit << " ";
				else if (field[x][y] == miss)
					std::cout << miss << " ";
				else
					std::cout << empty << " ";
				
					

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
				if (!(i + 1 >= BOARD_SIZE) && (field[x][i + 1] != ship))
					field[x][i + 1] = surrounding;
				if (!(i - 1 < 0) && (field[x][i - 1] != ship))
					field[x][i - 1] = surrounding;

				if (!(x + 1 >= BOARD_SIZE) && !(i + 1 >= BOARD_SIZE) && (field[x + 1][i + 1] != ship))
					field[x + 1][i + 1] = surrounding;
				if (!(x - 1 < 0) && !(i + 1 >= BOARD_SIZE) && (field[x - 1][i + 1] != ship))
					field[x - 1][i + 1] = surrounding;
				if (!(x + 1 >= BOARD_SIZE) && !(i - 1 < 0) && (field[x + 1][i - 1] != ship))
					field[x + 1][i - 1] = surrounding;
				if (!(x - 1 < 0) && !(i - 1 < 0) && (field[x - 1][i - 1] != ship))
					field[x - 1][i - 1] = surrounding;
			}
	}

	//X $ 0 
	bool hit(int x, int y, char hit, char miss, char ship) {

		if (field[x][y] == ship) {
			field[x][y] = hit;
			return true;
		}
		else {
			field[x][y] = miss;
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

	void remove_borders(char border, char empty) {
		for (int y = 0; y < BOARD_SIZE; y++)
			for (int x = 0; x < BOARD_SIZE; x++)
				if (field[x][y] == border)
					field[x][y] = empty; 
	}

	//For placing ships
	bool is_valid_pos(int x, int y, int length, int lygiuote) {

		if (lygiuote == 1) {
			if (x + length > BOARD_SIZE || x < 0 || y < 0 || y > BOARD_SIZE)
				return false;

			for (int i = x; i < x + length; i++) {
				if (field[i][y] == 'O' || field[i][y] == '*') {
					return false;
				}
				return true;
			}
		}
		else {

			if (y + length >= BOARD_SIZE || x < 0 || y < 0 || x >= BOARD_SIZE)
				return false;

			for (int i = y; i < y + length; i++) {
				if (field[x][i] == 'O' || field[x][i] == '*') {
					return false;
					break;
				}
				else
					return true;

			}
		}
	}

	//For targeting ships
	bool is_valid_pos(int x, int y, char empty) {

		if (field[x][y] != empty || (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE))
			return false;
		else
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

	std::string get_name() {
		return _name;
	}

	int get_size() {
		return _size;
	}

	int get_count() {
		return _count;
	}

	void set_count(int count) {
		_count = count;
	}

};

class computers_info {
public:
	int x;
	int y;
	int startingX;
	int startingY;
	int direction; //left, right, top, bottom
	bool sunken_ship;
	bool hit;

	void reset() {
		x = -1;
		y = -1;
		startingX = -1;
		startingY = -1;
		direction = -1;
		sunken_ship = false;
		hit = false;
	}

	void print() {
		std::cout << x << " " << y << " " << startingX << " " << startingY << " " << direction << " " << sunken_ship << "\n";
	}

	computers_info() {
		reset();
	}

};

void set_up_board(game_field & player, game_field & computer);
int random_num(int min, int max);
bool game_over(std::vector<game_field> fields);
void players_turn(game_field & target);
void computers_turn(game_field & target, computers_info & info);
void get_new_target(int & x, int & y, char empty, game_field field);

int main() {

	game_field player('#');
	game_field computer('#');

	set_up_board(player, computer); 
	computers_info info;

	while (!game_over({player, computer})) {
		players_turn(computer);
		computers_turn(player, info);
	}


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

		int laivas;
		int lygiuote;
		int x;
		int y;

		bool validPlayerInput = false;
		bool validShip;
		bool validAli;
		while (!validPlayerInput) {
			system("cls");
			std::cout << "Zaidejo lenta:\n";
			player.print_with_ships();
			std::cout << "\nKompiuterio lenta:\n";
			computer.print_with_ships();
			
			std::cout << "laivai\n";
			for (int i = 0; i < ships.size(); i++) {
				std::cout << i + 1 << ". " << ships[i].get_name() << ". Dydis: " << ships[i].get_size() << ". Liko: " << ships[i].get_count() << ".\n";
			}

			try {
				std::cout << "\nPasirinkite laiva:\n";
				std::cin >> laivas;
				if (laivas < 1 || laivas > ships.size()) {
					validShip = false;
					laivas = 1;
				}
				else
					validShip = true;
				

				std::cout << "\nKaip ji pastatyti:\n";
				std::cout << "1. Horizontaliai (laivas judes i desine)\n2. Vertikaliai (laivas judes zemyn)\n";
				std::cin >> lygiuote;
				if (lygiuote < 1 || lygiuote > 2) {
					validAli = false;
					lygiuote = 0;
				}
				else
					validAli = true;

				std::cout << "\nx pozicija: ";
				std::cin >> x;

				std::cout << "y pozicija: ";
				std::cin >> y;	 

				validPlayerInput = (validShip && validAli);
				if (!validPlayerInput || !player.is_valid_pos(x, y, ships[laivas - 1].get_size() - 1, lygiuote)) {
					validPlayerInput = false;
					std::cout << "\Bloga pozicija \\\\ Negalimi kiti duomenys\n\n";
					std::cout << player.is_valid_pos(x, y, ships[laivas - 1].get_size() - 1, lygiuote);
					system("pause");
				}
			}
			catch(...) {
				validPlayerInput = false;
				std::cout << "\Bloga pozicija \\\ Negalimi kiti duomenys\n\n";
				system("pause");
			}
		}

		int computerX;
		int computerY;
		int computerAligment;

		bool validComputerInput = false;
		while (!validComputerInput) {
			computerX = random_num(0, BOARD_SIZE - 1);
			computerY = random_num(0, BOARD_SIZE - 1);
			computerAligment = random_num(1, 2);

			if (computer.is_valid_pos(computerX, computerY, ships[laivas - 1].get_size() - 1, computerAligment))
				validComputerInput = true;
			else
				validComputerInput = false;
		}

		system("cls");
		player.insert_ship(x, y, ships[laivas - 1].get_size(), 'O', lygiuote, '*');
		computer.insert_ship(computerX, computerY, ships[laivas - 1].get_size(), 'O', computerAligment, '*');

		ships[laivas - 1].set_count(ships[laivas - 1].get_count() - 1);
		if (ships[laivas - 1].get_count() <= 0) {
			ships.erase(ships.begin() + (laivas - 1));
		}
	}

	//Remove ship borders after both players have set their board up
	player.remove_borders('*', '#');
	computer.remove_borders('*', '#');

}

int random_num(int min, int max) {

	srand(time(NULL));

	return rand() % (max - min + 1) + min;
}

bool game_over(std::vector<game_field> fields) {

	for (int i = 0; i < 2; i++) 
		if (fields[i].game_over('O'))
			return true;
	return false;

}

void players_turn(game_field & target) {

	system("cls");
	std::cout << "Zaidejo eile\n\n";
	std::cout << "Kompiuterio lenta.\n";
	target.print_without_ships('X', '#', '*');
	std::cout << "\n\n";
	target.print_with_ships();

	int x;
	int y;
	std::cout << "Iveskite x: ";
	std::cin >> x;
	std::cout << "Iveskite y: ";
	std::cin >> y;

	bool hit = target.hit(x, y, 'X', '*', 'O');

	system("cls");
	if (hit) {
		std::cout << "Hit!\n";
		system("pause");
		players_turn(target);
	}
	else {
		std::cout << "Miss!\n";
		system("pause");
	}

}

void computers_turn(game_field & target, computers_info & info) {

	system("cls");
	std::cout << "Kompiuterio eile\n\n";
	std::cout << "Zaidejo lenta:\n";
	target.print_without_ships('X', '#', '*');

	system("pause");

	int x, y;
	get_new_target(x, y, '#', target);
	std::cout << "x: ";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << x + 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "\n";
	std::cout << "y: ";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << y + 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	bool hit = target.hit(x, y, 'X', '*', 'O');
		

	if (hit) {
		std::cout << "Hit!\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		computers_turn(target, info);
	}
	else {
		std::cout << "Miss!\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	
}

void get_new_target(int & x, int & y, char empty, game_field field) {
	
	do {
		x = random_num(0, BOARD_SIZE - 1);
		y = random_num(0, BOARD_SIZE - 1);
	} while (!field.is_valid_pos(x, y, empty));
	
}