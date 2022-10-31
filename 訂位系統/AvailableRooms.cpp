// AvailableRooms.cpp
// Member-function definitions for class AvailableRooms.
#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;

#include "AvailableRooms.h" // AvailableRooms class definition

AvailableRooms::AvailableRooms()
{
   for( int i = 0; i < 6; i++ )
      availableRooms[ i ] = totalNumRooms[ i ];
}

Date AvailableRooms::getDate()
{
   return date;
}

void AvailableRooms::initilizeAvailableRooms( int year, int month, int day )
{
   date = Date( year, month, day );
   for( int i = 0; i < 6; i++ )
      availableRooms[ i ] = totalNumRooms[ i ];
}

int AvailableRooms::getAvailableRooms( int roomType )
{
   return availableRooms[ roomType ];
}

void AvailableRooms::decreaseAvailableRooms( int roomType, int numRooms )
{
   availableRooms[ roomType ] -= numRooms;
}

void AvailableRooms::displayAvailableRooms()
{
   cout << date.getYear() << "-"
      << setw( 2 ) << setfill( '0' ) << date.getMonth() << "-"
      << setw( 2 ) << date.getDay()
      << setw( 4 ) << setfill( ' ' ) << availableRooms[ 1 ]
      << setw( 2 ) << availableRooms[ 2 ]
      << setw( 2 ) << availableRooms[ 3 ]
      << setw( 2 ) << availableRooms[ 4 ]
      << setw( 2 ) << availableRooms[ 5 ] << "\n";
}

void AvailableRooms::random()
{
   for( int i = 1; i < 6; i++ )
      availableRooms[ i ] = rand() % ( totalNumRooms[ i ] + 1 );
}