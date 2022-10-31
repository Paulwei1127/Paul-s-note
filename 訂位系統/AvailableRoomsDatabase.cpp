// AvailableRoomsDatabase.cpp
// Member-function definitions for class AvailableRoomsDatabase.
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;
#include "AvailableRoomsDatabase.h" // AvailableRoomsDatabase class definition

extern Date computeCurrentDate();

extern bool leapYear(int year);

extern const int totalNumRooms[6]; // total number of rooms for each room type

AvailableRoomsDatabase::AvailableRoomsDatabase()
{
	loadAvailableRooms();

	Date currentDate = computeCurrentDate();
	if (numAvailableDays == 0)
		initilizeAvailableRooms(currentDate, 0, 6);
	else
		if (availableRooms[numAvailableDays - 1].getDate() < currentDate)
			initilizeAvailableRooms(currentDate, 0, 6);
		else if (availableRooms[0].getDate() < currentDate)
			adjustAvailableRooms(currentDate);
	// It cannot happen that availableRooms[ 0 ].getDate() > currentDate
}

AvailableRoomsDatabase::~AvailableRoomsDatabase()
{
	saveAvailableRooms();
}

void AvailableRoomsDatabase::initilizeAvailableRooms(const Date& firstDate, int first, int numMonths)
{
	/*availableRooms[first].getDate() = firstDate;
	availableRooms[first].random();*///delete
	int tempm = 0;//modify
	int addpos = 1;
	int days[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	//while (tempm != numMonths)//delete


	for (int j = firstDate.getMonth(); j <= firstDate.getMonth() + numMonths; j++)//modify
	{
		int NowDay = (j == firstDate.getMonth() ? firstDate.getDay() : 1);//add
		days[2] = (leapYear(firstDate.getYear() + tempm) ? 29 : 28);//add
		for (; NowDay <= days[j % 12]; NowDay++)//modify
		{
			/*availableRooms[first + addpos].random();
			availableRooms[first + addpos].getDate().setMonth(availableRooms[first].getDate().getMonth() + j);
			availableRooms[first + addpos].getDate().setDay(availableRooms[first].getDate().getDay() + i);
			addpos++;*///delete
			availableRooms[first].initilizeAvailableRooms(firstDate.getYear() + tempm, (j == 12 ? 12 : j % 12), NowDay);//add
			first++;//add
		}
		if (j == 12)//add
			tempm++;
	}
	numAvailableDays = first;
}

// remove outdated data from availableRooms, and
// add new data into availableRooms if necessary
void AvailableRoomsDatabase::adjustAvailableRooms(const Date& currentDate)
{
	int days[13] = { 31,31,28,31,30,31,30,31,31,30,31,30,31 };//add
	int needRemoveindex = 0;//add
	int pos = 0;//0
	while (availableRooms[needRemoveindex].getDate() < currentDate) {
		needRemoveindex++;
	}//add
	numAvailableDays -= needRemoveindex;//add
	for (int i = 0; i < numAvailableDays; i++)
	{
		//if (availableRooms[i].getDate() == currentDate)
			//for (int j = 0; j < numAvailableDays; j++)//delete

		availableRooms[i] = availableRooms[i + needRemoveindex];//modify
		pos++;//0

	}
	int Firstmon = availableRooms[0].getDate().getMonth(), Endmon = availableRooms[numAvailableDays - 1].getDate().getMonth();//add here to
	int minus = (Firstmon > Endmon ? ((Endmon + 12) - Firstmon) : (Endmon - Firstmon));
	int Endyear = availableRooms[numAvailableDays - 1].getDate().getYear(), Endday = availableRooms[numAvailableDays - 1].getDate().getDay();
	if (minus < 5) {
		if (Endmon + 1 > 12) {
			Endmon %= 12;
			Endyear++;
		}//to here(add)
		int tempm = availableRooms[pos].getDate().getMonth() - availableRooms[0].getDate().getMonth();//0
		Date DAY(Endyear, Endmon + 1, 1);//modify
		initilizeAvailableRooms(DAY, numAvailableDays, 5 - minus);
		minus++;
	}
}

void AvailableRoomsDatabase::displayAvailableRooms(Date checkInDate, Date checkOutDate)
{
	cout << "\nAvailable Rooms:\n\n";
	cout << "      Date   Superior Room   Deluxe Room   Deluxe Twin Room   Superior Suite   Deluxe Suite\n";

	// checkInDateIndex is the index of checkInDate in availableRooms
	// checkOutDateIndex is the index of checkOutDate in availableRooms
	int checkInDateIndex = getDateIndex(checkInDate, 0);
	int checkOutDateIndex = getDateIndex(checkOutDate, checkInDateIndex + 1);

	for (int i = checkInDateIndex; i < checkOutDateIndex; i++)
		cout << availableRooms[i].getDate().getYear() << "-"
		<< setw(2) << setfill('0') << availableRooms[i].getDate().getMonth() << "-"
		<< setw(2) << availableRooms[i].getDate().getDay()
		<< setw(16) << setfill(' ') << availableRooms[i].getAvailableRooms(1)
		<< setw(14) << availableRooms[i].getAvailableRooms(2)
		<< setw(19) << availableRooms[i].getAvailableRooms(3)
		<< setw(17) << availableRooms[i].getAvailableRooms(4)
		<< setw(15) << availableRooms[i].getAvailableRooms(5) << "\n";
}

int AvailableRoomsDatabase::findMinNumRooms(int roomType,
	const Date& checkInDate, const Date& checkOutDate)
{
	int minNumRooms = totalNumRooms[roomType];

	int checkInDateIndex = getDateIndex(checkInDate, 0);
	int checkOutDateIndex = getDateIndex(checkOutDate, checkInDateIndex + 1);

	for (int i = checkInDateIndex; i < checkOutDateIndex; i++)
		if (minNumRooms > availableRooms[i].getAvailableRooms(roomType))
			minNumRooms = availableRooms[i].getAvailableRooms(roomType);

	return minNumRooms;
}

void AvailableRoomsDatabase::decreaseAvailableRooms(int roomType, int numRooms,
	const Date& checkInDate, const Date& checkOutDate)
{
	int checkInDateIndex = getDateIndex(checkInDate, 0);
	int checkOutDateIndex = getDateIndex(checkOutDate, checkInDateIndex + 1);

	for (int i = checkInDateIndex; i < checkOutDateIndex; i++)
		availableRooms[i].decreaseAvailableRooms(roomType, numRooms);
}

void AvailableRoomsDatabase::loadAvailableRooms()
{
	ifstream inFile("Available Rooms.dat", ios::in | ios::binary);

	if (!inFile)
	{
		cout << "Available rooms.dat could not be opened!\n";
		system("pause");
		exit(1);
	}

	numAvailableDays = 0;
	while (inFile.read(reinterpret_cast<char*>(&availableRooms[numAvailableDays]), sizeof(AvailableRooms)))
		++numAvailableDays;

	inFile.close();
}

void AvailableRoomsDatabase::saveAvailableRooms()
{
	ofstream outFile("Available Rooms.dat", ios::out | ios::binary);

	if (!outFile)
	{
		cout << "Available rooms.dat could not be opened." << endl;
		system("pause");
		exit(1);
	}

	for (int i = 0; i < numAvailableDays; i++)
		outFile.write(reinterpret_cast<const char*> (&availableRooms[i]), sizeof(AvailableRooms));

	outFile.close();
}

// return the index i in [ dateIndex, numAvailableDays ) such that availableRooms[ i ].date == date
int AvailableRoomsDatabase::getDateIndex(const Date& date, int dateIndex)
{
	for (int i = dateIndex; i < numAvailableDays; i++)
		if (availableRooms[i].getDate() == date)
			return i;

	return -1;
}