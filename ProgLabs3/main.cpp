//int main()
//{
//	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
//	sf::CircleShape shape(100.f);
//	shape.setFillColor(sf::Color::Green);
//
//	while (window.isOpen())
//	{
//		sf::Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == sf::Event::Closed)
//				window.close();
//		}
//
//		window.clear();
//		window.draw(shape);
//		window.display();
//	}
//
//	return 0;
//}

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <Windows.h>


#pragma region Structs

struct creature
{
	unsigned int rowBit = 0;
	unsigned int colBit = 0;
	unsigned int rowIndex = -1;
	unsigned int colIndex = -1;
	bool safe = true;
};
#pragma endregion

#pragma region Functions

std::vector<std::string> readFromFile(std::string);
std::vector<creature> areCreaturesSafe(std::string);
unsigned int getBit(int);

#pragma endregion


int main()
{
	std::string fileName = "../Data/creatures.txt";
	std::vector<std::string> inputs = {};
	try
	{
		inputs = readFromFile(fileName);
	}
	catch (...)
	{
		std::cout << "There was a problem reading the file.... Please check the file";
		system("pause");
	}
	int const WIDTH = 720;
	int const HEIGHT = 480;
	try
	{
		for each (std::string input in inputs)
		{
			std::vector<creature> creatures = areCreaturesSafe(input);
			if (creatures.size() != 8)
			{
				throw new std::exception("There has to be only 8 creatures!");
			}
			sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), input.c_str());
			sf::RectangleShape cells[8][8];
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					cells[i][j].setSize(sf::Vector2f(50, 50));
					cells[i][j].setFillColor(sf::Color::White);
					cells[i][j].setPosition(120 + (i * 60), (j * 60) + 5);
				}
			}
			for each (creature creat in creatures)
			{
				cells[creat.colIndex][creat.rowIndex].setFillColor(sf::Color::Green);
				if (!creat.safe)
				{
					cells[creat.colIndex][creat.rowIndex].setOutlineColor(sf::Color::Red);
					cells[creat.colIndex][creat.rowIndex].setOutlineThickness(5);
				}
			}
			while (window.isOpen())
			{
				sf::Event event;
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();
					if (event.type == sf::Event::KeyPressed)
						window.close();
				}

				window.clear();
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						window.draw(cells[i][j]);
					}
				}
				window.display();
			}
		}
	}
	catch (...)
	{
		std::cout << "There was a problem.... Please check the inputs" << std::endl;
		system("pause");
	}
	return 0;
}

std::vector<std::string> readFromFile(std::string fileName)
{
	std::string line;
	std::vector<std::string> listOfInputs = {};
	std::ifstream myfile(fileName);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			listOfInputs.push_back(line);
		}
		myfile.close();
		return listOfInputs;
	}
	return listOfInputs;
}
std::vector<creature> areCreaturesSafe(std::string input)
{
	int map[8][8] = { 0 };
	std::vector<creature> creatures;
	for (int i = 0; i < input.length(); i += 2)
	{
		creature tmp;
		tmp.rowIndex = (input[i] - 1) - 48;
		tmp.colIndex = (input[i + 1] - 1) - 48;
		map[tmp.rowIndex][tmp.colIndex] = 1;
		tmp.rowBit = getBit(tmp.colIndex);
		tmp.colBit = getBit(tmp.rowIndex);
		creatures.push_back(tmp);
	}
	for (int i = 0; i < creatures.size(); i++)
	{
		if (creatures[i].safe)
		{
			//Rows And Columns
			for (int j = i + 1; j < creatures.size(); j++)
			{
				int checkRow = creatures[i].rowBit & creatures[j].rowBit;
				int checkCol = creatures[i].colBit & creatures[j].colBit;
				if (checkRow != 0 || checkCol != 0)
				{
					creatures[i].safe = false;
					creatures[j].safe = false;
				}
			}
			//Diagonals
			if (creatures[i].safe)
			{
				bool isDoneCheckingDiagonals = false;
				int topRow = creatures[i].rowIndex, botRow = creatures[i].rowIndex;
				int rightCol = creatures[i].colIndex, leftCol = creatures[i].colIndex;
				while (!isDoneCheckingDiagonals)
				{
					topRow--;
					rightCol++;
					botRow++;
					leftCol--;
					if (topRow >= 0)
					{
						if (rightCol <= 7)
						{
							if (map[topRow][rightCol] == 1)
							{
								creatures[i].safe = false;
								break;
							}
						}
						if (leftCol >= 0)
						{
							if (map[topRow][leftCol] == 1)
							{
								creatures[i].safe = false;
								break;
							}
						}
					}
					if (botRow <= 7)
					{
						if (rightCol <= 7)
						{
							if (map[botRow][rightCol] == 1)
							{
								creatures[i].safe = false;
								break;
							}
						}
						if (leftCol >= 0)
						{
							if (map[botRow][leftCol] == 1)
							{
								creatures[i].safe = false;
								break;
							}
						}
					}
					if (topRow < 0 && botRow > 7)
					{
						isDoneCheckingDiagonals = true;
					}
				}
			}
		}
		//WORKING CONDITION
		//int rowIndex = creatures[i].rowIndex;
		//int colIndex = creatures[i].colIndex;
		//while (rowIndex < 7 && colIndex < 7)
		//{
		//	rowIndex++;
		//	colIndex++;
		//	if (map[rowIndex][colIndex] == 1)
		//	{
		//		creatures[i].safe = false;
		//	}
		//}
		//rowIndex = creatures[i].rowIndex;
		//colIndex = creatures[i].colIndex;
		//while (rowIndex > 0 && colIndex > 0)
		//{
		//	rowIndex--;
		//	colIndex--;
		//	if (map[rowIndex][colIndex] == 1)
		//	{
		//		creatures[i].safe = false;
		//	}
		//}
		//rowIndex = creatures[i].rowIndex;
		//colIndex = creatures[i].colIndex;
		//while (rowIndex > 0 && colIndex < 7)
		//{
		//	rowIndex--;
		//	colIndex++;
		//	if (map[rowIndex][colIndex] == 1)
		//	{
		//		creatures[i].safe = false;
		//	}
		//}
		//rowIndex = creatures[i].rowIndex;
		//colIndex = creatures[i].colIndex;
		//while (rowIndex < 7 && colIndex > 0)
		//{
		//	rowIndex++;
		//	colIndex--;
		//	if (map[rowIndex][colIndex] == 1)
		//	{
		//		creatures[i].safe = false;
		//	}
		//}
	}
	return creatures;
}
unsigned int getBit(int index)
{
	switch (index)
	{
	case 0:
		return 128;
	case 1:
		return 64;
	case 2:
		return 32;
	case 3:
		return 16;
	case 4:
		return 8;
	case 5:
		return 4;
	case 6:
		return 2;
	case 7:
		return 1;
	default:
		//TODO error
		break;
	}
}
