// MakeReservation.cpp
// Member-function definitions for class MakeReservation.
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

#include "MakeReservation.h" // MakeReservation class definition

extern Date computeCurrentDate();

extern bool leapYear(int year);

extern int inputAnInteger(int begin, int end);

// MakeReservation constructor initializes base-class data members
MakeReservation::MakeReservation(ReservationDatabase& theReservationDatabase,
	AvailableRoomsDatabase& theAvailableRoomsDatabase)
	: reservationDatabase(theReservationDatabase),
	availableRoomsDatabase(theAvailableRoomsDatabase)
{
} // end MakeReservation constructor

// performs transaction
void MakeReservation::execute()
{
	Date currentDate = computeCurrentDate();

	Date availableMonths[7];
	computeAvailableMonths(currentDate, availableMonths);

	int checkInCode;
	int firstDay;
	int lastDay;

	Date checkInDate;
	Date checkOutDate;

	inputCheckInDate(checkInDate, currentDate, availableMonths, checkInCode, firstDay, lastDay);
	inputCheckOutDate(checkOutDate, checkInDate, availableMonths, checkInCode, firstDay, lastDay);

	cout << "\nCheck in date: " << checkInDate.getYear() << '-'
		<< setw(2) << setfill('0') << checkInDate.getMonth() << '-'
		<< setw(2) << checkInDate.getDay() << endl;

	cout << "Check out date: " << checkOutDate.getYear() << '-'
		<< setw(2) << checkOutDate.getMonth() << '-'
		<< setw(2) << checkOutDate.getDay() << endl;

	availableRoomsDatabase.displayAvailableRooms(checkInDate, checkOutDate);

	int roomType;
	do {
		cout << "\nSelect Room Type:\n"
			<< "1. Superior Room\n" << "2. Deluxe Room\n" << "3. Deluxe Twin Room\n"
			<< "4. Superior Suite\n" << "5. Deluxe Suite\n? ";
	} while ((roomType = inputAnInteger(1, 5)) == -1);

	int minNumRooms = availableRoomsDatabase.findMinNumRooms(roomType, checkInDate, checkOutDate);

	if (minNumRooms == 0)
	{
		cout << "\nThere are no rooms available!\n";
		return;
	}

	int numRooms;
	do cout << "\nEnter the number of rooms (1 ~ " << minNumRooms << ", 0 to end): ";
	while ((numRooms = inputAnInteger(0, minNumRooms)) == -1);
	if (numRooms == 0)
		return;

	availableRoomsDatabase.decreaseAvailableRooms(roomType, numRooms, checkInDate, checkOutDate);

	cout << "\nID Number: ";
	string IDNumber;
	getline(cin, IDNumber, '\n');

	cout << "\nName: ";
	string name;
	getline(cin, name, '\n');

	cout << "\nMobile Phone: ";
	string mobile;
	getline(cin, mobile, '\n');

	newReservation.setReservation(roomType, numRooms, checkInDate, checkOutDate, IDNumber, name, mobile);

	cout << "\n    Name   Fare       Mobile          Room type   Number of rooms   Check in date   Check out date\n";
	newReservation.displayReservation();

	reservationDatabase.pushBack(newReservation);

	cout << "\nReservation successfully!" << endl;
}

void MakeReservation::computeAvailableMonths(Date currentDate, Date availableMonths[])
{
	int year = currentDate.getYear();
	int month = currentDate.getMonth();
	for (int i = 1; i <= 6; i++)
	{
		availableMonths[i].setYear(year);
		availableMonths[i].setMonth(month);

		if (month < 12)
			month++;
		else
		{
			year++;
			month = 1;
		}
	}
}

void MakeReservation::inputCheckInDate(Date& checkInDate, Date currentDate, Date availableMonths[],
	int& checkInCode, int& firstDay, int& lastDay)
{
	int days[13] = { 31,31,28,31,30,31,30,31,31,30,31,30,31 };//add
	cout << "Please Input Check In Date:" << endl;
	cout << "Month:" << endl;
	for (int i = 1; i <= 6; i++)
	{
		cout << i << ".  " << availableMonths[i].getYear() << "-"
			<< setw(2) << setfill('0') << availableMonths[i].getMonth() << endl;//modify
	}
	do {
		cout << "?";//add
		checkInCode = inputAnInteger(1, 6);
	} while (checkInCode == -1);//add
	days[2] = (leapYear(availableMonths[checkInCode].getYear()) ? 29 : 28);//add
	if (availableMonths[checkInCode].getMonth() == currentDate.getMonth())//如果跟今天同月則從今天開始
		firstDay = currentDate.getDay();
	else
		firstDay = 1;//不同月從1號開始
	lastDay = (checkInCode == 6 ? days[availableMonths[checkInCode].getMonth()] - 1 : days[availableMonths[checkInCode].getMonth()]);//不管如何都會是一個月的最後一天//modify
	int choice2;//add
	do {
		cout << "Day (" << firstDay << " ~ " << lastDay << ") :" << endl;
		choice2 = inputAnInteger(firstDay, lastDay);//modify
	} while (choice2 == -1);//add

	checkInDate.setYear(availableMonths[checkInCode].getYear());
	checkInDate.setMonth(availableMonths[checkInCode].getMonth());
	checkInDate.setDay(choice2);
}

void MakeReservation::inputCheckOutDate(Date& checkOutDate, Date checkInDate, Date availableMonths[],
	int checkInCode, int firstDay, int lastDay)
{
	int days[13] = { 31,31,28,31,30,31,30,31,31,30,31,30,31 };//add
	days[2] = (leapYear(availableMonths[checkInCode].getYear()) ? 29 : 28);//add
	int help = checkInCode;//modify
	cout << "Please Input Check Out Date" << endl;
	cout << "Month:" << endl;
	for (int i = checkInCode; i <= 6; i++)//modify
	{
		if (checkInDate.getDay() == days[availableMonths[i].getMonth()] && checkInDate.getMonth() == availableMonths[i].getMonth())//月份同且剛好月底//modify
		{
			help++;//modify
			continue;

		}
		cout << i << ".  " << availableMonths[i].getYear() << "-"
			<< setw(2) << setfill('0') << availableMonths[i].getMonth() << endl;
	}
	/*if (help == -1)
		checkInCode = inputAnInteger(2, 6);
	else*///delete
	do {//add
		cout << "?";//add
		checkInCode = inputAnInteger(help, 6);//modify
	} while (checkInCode == -1);//add
	if (availableMonths[checkInCode].getMonth() == checkInDate.getMonth())//跟今天同月，則從明天開始
		firstDay = checkInDate.getDay() + 1;
	else
		firstDay = 1;//不同月，從1號開始
	lastDay = days[availableMonths[checkInCode].getMonth()];//modify

	int choice2;//add
	do {//add
		cout << "?";//add
		cout << "Day (" << firstDay << " ~ " << lastDay << ") :" << endl;
		choice2 = inputAnInteger(firstDay, lastDay);//modify
	} while (choice2 == -1);//add
	checkOutDate.setYear(availableMonths[checkInCode].getYear());
	checkOutDate.setMonth(availableMonths[checkInCode].getMonth());
	checkOutDate.setDay(choice2);
}
