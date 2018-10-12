#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <limits>
#include <chrono>
#include <thread>

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

	void print_without_ships(char hit, char empty, char miss) {
		//X - hit ship
		//# - empty space
		//* - missed shot
		std::cout << "  ";
		for (int i = 0; i < BOARD_SIZE; i++)
			std::cout << i + 1 << " ";
		std::cout << "\n";

		for (int y = 0; y < BOARD_SIZE; y++) {
			std::cout << y + 1 << " ";
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

	void setCount(int count) {
		_count = count;
	}

};

void set_up_board(game_field & player, game_field & computer);
bool isValidPos(int x, int y, int length, int lygiuote, game_field field);
int randomNum(int min, int max);
bool gameOver(std::vector<game_field> fields);
void players_turn(game_field & target);
void computers_turn(game_field & target);

int main() {

	game_field player('#');
	game_field computer('#');

	set_up_board(player, computer); 


	while (!gameOver({player, computer})) {
		players_turn(computer);
		computers_turn(player);
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
	//ships.push_back(cruiser);
	//ships.push_back(destroyer);
	//ships.push_back(submarine);


	while (!ships.empty()) {

		std::cout << "Zaidejo eile:\n";

		int laivas;
		int lygiuote;
		int x;
		int y;

		bool validPlayerInput = false;
		while (!validPlayerInput) {
			system("cls");
			std::cout << "Zaidejo lenta:\n";
			player.print_with_ships();
			std::cout << "\nKompiuterio lenta:\n";
			computer.print_with_ships();
			
			std::cout << "laivai\n";
			for (int i = 0; i < ships.size(); i++) {
				std::cout << i + 1 << ". " << ships[i].getName() << ". Dydis: " << ships[i].getSize() << ". Liko: " << ships[i].getCount() << ".\n";
			}

			std::cout << "\nPasirinkite laiva:\n";
			std::cin >> laivas;
			if (laivas < 1 || laivas > ships.size())
				validPlayerInput = false;
			else
				validPlayerInput = true;

			if (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				validPlayerInput = false;
				x = 0;
				y = 0;
				lygiuote = 0;
				laivas = 0;
			}

			std::cout << "\nKaip ji pastatyti:\n";
			std::cout << "1. Horizontaliai (laivas judes i desine)\n2. Vertikaliai (laivas judes zemyn)\n";
			std::cin >> lygiuote;
			if (lygiuote < 1 || lygiuote > 2)
				validPlayerInput = false;
			else
				validPlayerInput = true;

			std::cout << "\nx pozicija: ";
			std::cin >> x;
			x--;

			std::cout << "y pozicija: ";
			std::cin >> y;
			y--;	 

			try {
				if (!validPlayerInput || !isValidPos(x, y, ships[laivas - 1].getSize() - 1, lygiuote, player)) {
					validPlayerInput = false;
					std::cout << "\Bloga pozicija \\\\ Negalimi kiti duomenys\n\n";
					system("pause");
					throw new std::exception;
				}
			}
			catch(const std::exception&) {
				

				validPlayerInput = false;
				std::cout << "\Bloga pozicija \\\\ Negalimi kiti duomenys\n\n";
				system("pause");
			}
		}

		int computerX;
		int computerY;
		int computerAligment;

		bool validComputerInput = false;
		while (!validComputerInput) {
			computerX = randomNum(0, BOARD_SIZE - 1);
			computerY = randomNum(0, BOARD_SIZE - 1);
			computerAligment = randomNum(1, 2);

			if (isValidPos(computerX, computerY, ships[laivas - 1].getSize() - 1, computerAligment, computer))
				validComputerInput = true;
			else
				validComputerInput = false;
		}

		system("cls");
		player.insert_ship(x, y, ships[laivas - 1].getSize(), 'O', lygiuote, '*');
		computer.insert_ship(computerX, computerY, ships[laivas - 1].getSize(), 'O', computerAligment, '*');

		ships[laivas - 1].setCount(ships[laivas - 1].getCount() - 1);
		if (ships[laivas - 1].getCount() <= 0) {
			ships.erase(ships.begin() + (laivas - 1));
		}
	}

	//Remove ship borders after both players have set their board up
	player.remove_borders('*', '#');
	computer.remove_borders('*', '#');

}

bool isValidPos(int x, int y, int length, int lygiuote, game_field field) {
	
	if (lygiuote == 1) {
		if (x + length >= BOARD_SIZE || x < 0 || y < 0 || y >= BOARD_SIZE)
			return false;

		for (int i = x; i < x + length; i++) {
			if (field.getSquare(i, y) == 'O' || field.getSquare(i, y) == '*') {
				return false;
			}
			return true;
		}
	}
	else {

		if (y + length >= BOARD_SIZE || x < 0 || y < 0 || x >= BOARD_SIZE)
			return false;

		for (int i = y; i < y + length; i++) {
			if (field.getSquare(x, i) == 'O' || field.getSquare(x, i) == '*') {
				return false;
				break;
			}
			else 
				return true;

		}
	}
}

int randomNum(int min, int max) {

	srand(time(NULL));

	return rand() % (max - min + 1) + min;

}

bool gameOver(std::vector<game_field> fields) {

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

	bool hit = target.hit(x - 1, y - 1, 'X', '*', 'O');

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

void computers_turn(game_field & target) {

	system("cls");
	std::cout << "Kompiuterio eile\n\n";
	std::cout << "Zaidejo lenta:\n";
	
	target.print_without_ships('X', '#', '*');
	std::cout << "x: ";
	int x = randomNum(0, BOARD_SIZE - 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << x + 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "\n";
	std::cout << "y: ";
	int y = randomNum(0, BOARD_SIZE - 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << y + 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	bool hit = target.hit(x, y, 'X', '*', 'O');

	system("cls");
	if (hit) {
		std::cout << "Hit!\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		players_turn(target);
	}
	else {
		std::cout << "Miss!\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}