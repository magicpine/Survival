#include <fstream>
#include <iostream>
#include <string>
#include <list>

#include <SFML/Graphics.hpp>

#pragma region Structs
struct creature
{
	unsigned int rowIndex = -1;
	unsigned int colIndex = -1;
	bool safe = true;
};
#pragma endregion
#pragma region Functions
std::list<std::string> readFromFile(std::string);
void checkToSeeIfCreaturesAreSafe(std::string, creature[], int);
#pragma endregion
int main()
{
	std::string fileName = "../Data/creatures.txt";
	std::list<std::string> inputs = {};
	try
	{
		inputs = readFromFile(fileName);
	}
	catch (...)
	{
		std::cout << "There was a problem reading the file.... Please check the file";
		system("pause");
	}
	int const WIDTH = 480;
	int const HEIGHT = 480;
	int const NUMBER_OF_CREATURES = 8;
	try
	{
		for each (std::string input in inputs)
		{
			if (input.length() != 16)
			{
				std::cout << "One of the lines is not 16 units long" << std::endl;
			}
			else
			{
				creature creatures[NUMBER_OF_CREATURES] = {};
				checkToSeeIfCreaturesAreSafe(input, creatures, NUMBER_OF_CREATURES);
				sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), input.c_str());
				sf::RectangleShape cells[NUMBER_OF_CREATURES][NUMBER_OF_CREATURES];
				for (int i = 0; i < NUMBER_OF_CREATURES; i++)
				{
					for (int j = 0; j < NUMBER_OF_CREATURES; j++)
					{
						cells[i][j].setSize(sf::Vector2f(50, 50));
						cells[i][j].setFillColor(sf::Color::White);
						cells[i][j].setPosition((i * 60) + 5, (j * 60) + 5);
					}
				}
				bool areAllCreaturesSafe = true;
				for each (creature creat in creatures)
				{
					if (creat.safe)
					{
						cells[creat.colIndex][creat.rowIndex].setFillColor(sf::Color::Green);
					}
					else
					{
						cells[creat.colIndex][creat.rowIndex].setFillColor(sf::Color::Red);
						areAllCreaturesSafe = false;
					}
				}
				if (areAllCreaturesSafe)
				{
					std::cout << "All the Creatures are safe! In input string: "<< input << std::endl;
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
					for (int i = 0; i < NUMBER_OF_CREATURES; i++)
					{
						for (int j = 0; j < NUMBER_OF_CREATURES; j++)
						{
							window.draw(cells[i][j]);
						}
					}
					window.display();
				}
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

std::list<std::string> readFromFile(std::string fileName)
{
	std::string line;
	std::list<std::string> listOfInputs = {};
	std::ifstream myfile(fileName);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			listOfInputs.push_back(line);
		}
		myfile.close();
	}
	return listOfInputs;
}
void checkToSeeIfCreaturesAreSafe(std::string input, creature creatures[], int arraySize)
{
	int map[8][8] = { 0 };
	for (int i = 0; i < arraySize; i++)
	{
		std::string tmp = input.substr(0, 2);
		input = input.erase(0, 2);
		int realNumberValueModifer = 49; //the ASCII VALUE minus 1 equals the real number value
		creatures[i].rowIndex = (tmp[0]) - realNumberValueModifer;
		creatures[i].colIndex = (tmp[1]) - realNumberValueModifer;
		map[creatures[i].rowIndex][creatures[i].colIndex] = 1;
	}
	for (int i = 0; i < arraySize; i++)
	{
		if (creatures[i].safe)
		{
			//Rows And Columns
			for (int j = i + 1; j < arraySize; j++)
			{
				if (creatures[i].rowIndex == creatures[j].rowIndex || creatures[i].colIndex == creatures[j].colIndex)
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
	}
}