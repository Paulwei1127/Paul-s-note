// Reservation.cpp
// Member-function definitions for class Reservation.
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;
#include "Reservation.h" // Reservation class definition

void Reservation::setReservation( int inputRoomType, int inputNumRooms,
                                  Date inputCheckInDate, Date inputCheckOutDate,
                                  string inputIDNumber, string inputName, string inputMobile )
{
   roomType = inputRoomType;
   numRooms = inputNumRooms;
   checkInDate = inputCheckInDate;
   checkOutDate = inputCheckOutDate;
   setIDNumber( inputIDNumber );
   setName( inputName );
   setMobile( inputMobile );
}

void Reservation::setIDNumber( string theIDNumber )
{
   strcpy_s( IDNumber, 12, theIDNumber.c_str() );
}

void Reservation::setName( string theName )
{
   strcpy_s( name, 12, theName.c_str() );
}

void Reservation::setMobile( string theMobile )
{
   strcpy_s( mobile, 12, theMobile.c_str() );
}

string Reservation::getIDNumber()
{
   return IDNumber;
}

int Reservation::getRoomType()
{
   return roomType;
}

int Reservation::getNumRooms()
{
   return numRooms;
}

Date Reservation::getCheckInDate()
{
   return checkInDate;
}

Date Reservation::getCheckOutDate()
{
   return checkOutDate;
}

bool Reservation::equalID( string id1, string id2 )
{
   id1[ 0 ] = tolower( id1[ 0 ] );
   id2[ 0 ] = tolower( id2[ 0 ] );

   return id1 == id2;
}

void Reservation::displayReservation()
{
   int roomRate[ 6 ] = { 0, 5390, 6270, 6270, 6820, 8470 };
   char roomTypeName[ 6 ][ 20 ] = { "", "Superior Room", "Deluxe Room",
                    "Deluxe Twin Room", "Superior Suite", "Deluxe Suite" };

   cout << setw( 8 ) << setfill( ' ' ) << name
        << setw( 7 ) << numRooms * roomRate[ roomType ] * ( checkOutDate - checkInDate )
        << setw( 13 ) << mobile
        << setw( 19 ) << roomTypeName[ roomType ]
        << setw( 18 ) << numRooms
        << setw( 10 ) << checkInDate.getYear() << "-"
        << setw( 2 ) << setfill( '0' ) << checkInDate.getMonth() << "-"
        << setw( 2 ) << checkInDate.getDay()
        << setw( 11 ) << setfill( ' ' ) << checkOutDate.getYear() << "-"
        << setw( 2 ) << setfill( '0' ) << checkOutDate.getMonth() << "-"
        << setw( 2 ) << checkOutDate.getDay() << endl;
}