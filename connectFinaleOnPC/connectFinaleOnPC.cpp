#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
class PlayField
{
private:
	const static unsigned NumOfColumns = 10;
	const static unsigned NumOfRows = 6;
	int WhereToPut;

	int WhichRow;

public:
	//variables
	std::string Rows[NumOfRows];
	std::vector<int> EnteredNums;
	std::vector<int> xCoordinates;
	std::vector<int> oCoordinates;

	//functions
	void PrintField();
	void TestMove(); //player enters num and it gets tested
	void PutInCoordinateSystem(int LoopNum);
	void PutInField(int LoopNum);
	bool TestForEndGame(int LoopNum);
	bool CoordinateFinder(std::vector<int> CoorSys, int testWith);

	void MainLoop();

	//default constructor
	PlayField()
	{
		for (int i = 0; i < NumOfRows; i++)
		{
			Rows[i] = "|          |";
		}
		PrintField();
	}
};

int main()
{
	PlayField* game = new PlayField;
	game->MainLoop();

	return 0;
}

//Func definitions
void PlayField::PrintField()
{
	std::cout << "------------\n";
	for (int i = (NumOfRows - 1); i >= 0; i--)
	{

		std::cout << Rows[i] << std::endl;
	}
	std::cout << "------------\n";
	std::cout << " 0123456789 \n";
}

void PlayField::TestMove()
{
	WhereToPut = 0;

	std::string EnteredColumnNum; //Num entered by user, this is getting processed
	int UsableColumnNum; //processed EnteredColumnNum

	std::cout << "Enter where to put your symbol. It has to be an integer number between 0 and 9\n";

	while (true)
	{
		EnteredColumnNum = "";
		UsableColumnNum = 0;

		std::getline(std::cin, EnteredColumnNum);
		std::stringstream processingEntered(EnteredColumnNum);

		if (processingEntered >> UsableColumnNum &&
		  !(processingEntered >> EnteredColumnNum) 
		   && UsableColumnNum >= 0 &&
			  UsableColumnNum <= (NumOfColumns-1))
		{
			WhichRow = std::count(EnteredNums.begin(),
				EnteredNums.end(), UsableColumnNum);
			if (WhichRow < NumOfRows)
			{
				EnteredNums.push_back(UsableColumnNum);
				WhereToPut = UsableColumnNum;
				break;
			}
			std::cout << "This column is full. Please enter the number of another column\n";
		}
		else
		{
			std::cin.clear();
			std::cout << "Please enter a num between 0 and 9\n";
		}
	}
}

void PlayField::PutInCoordinateSystem(int LoopNum)
{
	if (LoopNum % 2)
		oCoordinates.push_back((WhichRow * NumOfColumns) + WhereToPut);
	else
		xCoordinates.push_back((WhichRow * NumOfColumns) + WhereToPut);
}

void PlayField::PutInField(int LoopNum)
{
	if (LoopNum % 2)
		Rows[WhichRow].replace(WhereToPut + 1, 1, "O");
	else
		Rows[WhichRow].replace(WhereToPut + 1, 1, "X");
}

void PlayField::MainLoop()
{
	int LoopNum = 0;

	while (LoopNum < 61)
	{
		TestMove();
		PutInField(LoopNum);
		PutInCoordinateSystem(LoopNum);
		PrintField();
		if (!TestForEndGame(LoopNum))
			break;
		LoopNum++;
	}
	if (LoopNum == 61)
		std::cout << "Draw\n";
	else if (LoopNum % 2)
		std::cout << "2nd player 'O' won\n";
	else
		std::cout << "1st player 'X' won\n";
}

bool PlayField::TestForEndGame(int LoopNum)
{
	if (LoopNum < 6)
		return true;

	if (LoopNum % 2)
	{
		std::sort(oCoordinates.begin(), oCoordinates.end());
		if (CoordinateFinder(oCoordinates, 1))
			return false;
		if (CoordinateFinder(oCoordinates, NumOfColumns))
			return false;
		if (CoordinateFinder(oCoordinates, (NumOfColumns + 1)))
			return false;
		if (CoordinateFinder(oCoordinates, (NumOfColumns - 1)))
			return false;
	}
	else
	{
		std::sort(xCoordinates.begin(), xCoordinates.end());
		if (CoordinateFinder(xCoordinates, 1))
			return false;
		if (CoordinateFinder(xCoordinates, NumOfColumns))
			return false;
		if (CoordinateFinder(xCoordinates, (NumOfColumns + 1)))
			return false;
		if (CoordinateFinder(xCoordinates, (NumOfColumns - 1)))
			return false;
	}
	return true;
}

bool PlayField::CoordinateFinder(std::vector<int> CoorSys, int testWith)
{
	for (int i = 0; i < CoorSys.size(); i++)
	{
		/*
		int numToTest = CoorSys[i];

		for (int j = 0; j < 4; j++)
		{
			if (std::find(CoorSys.begin(), CoorSys.end(), (numToTest + testWith)) != CoorSys.end())
				numToTest = numToTest + testWith;
			else if (j == 3)
				return true;
			else
				break;
		}
	}
	*/

		int Test = CoorSys[i];
		std::vector<int> WinningNums;
		WinningNums.push_back(Test);
		for (int j = 1; j < 4; j++)
		{
			if (!(Test%10) && (testWith == (NumOfColumns - 1))) 
			{/*
				 "------------"
				 "|          |"
				 "|          |"
				 "|          |"
				 "|       x  |"
				 "|        x |"
				 "|x        x|"
				 "------------"					 
			*/}
			else if (!(Test%9)&&(testWith== (NumOfColumns + 1))) 
			{/*
				 "------------"
				 "|          |"
				 "|  x       |"
				 "| x        |"
				 "|x         |"
				 "|          |"
				 "|         x|"
				 "------------"
			 */}
			else if (Test && !(Test%9)&&(testWith== 1))
			{/*
				"------------"
				"|          |"
				"|          |"
				"|          |"
				"|          |"
				"|xxx       |"
				"|         x|"
				"------------"
			 */}
			else if (std::find(CoorSys.begin(), CoorSys.end(), (Test + (testWith * j))) != CoorSys.end())
			{
				WinningNums.push_back(Test + (testWith * j));
				if (WinningNums.size() == 4)
				{
					return true;
				}
			}
			else
				break;
		}
	}
	return false;
}