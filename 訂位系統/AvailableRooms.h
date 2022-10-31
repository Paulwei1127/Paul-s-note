// AvailableRooms.h
// AvailableRooms class definition. Represents the number of available rooms of each type for one day.
#ifndef AVAILABLE_ROOMS_H
#define AVAILABLE_ROOMS_H

#include "Date.h" // Date class definition

const int totalNumRooms[ 6 ] = { 0, 9, 5, 3, 5, 4 }; // total number of rooms for each room type

class AvailableRooms
{
public:
   AvailableRooms(); // availableRooms[ 0..5 ] = totalNumRooms[ 0..5 ]

   Date getDate();

   // date = Date( year, month, day )
   // availableRooms[ 0..5 ] = totalNumRooms[ 0..5 ]
   void initilizeAvailableRooms( int year, int month, int day );

   int getAvailableRooms( int roomType ); // return availableRooms[ roomType ]

   // availableRooms[ roomType ] -= numRooms
   void decreaseAvailableRooms( int roomType, int numRooms );

   void displayAvailableRooms();

   void random();
private:
   Date date = Date();
   int availableRooms[ 6 ] = {}; // number of available rooms for each room type
}; // end class AvailableRooms

#endif // AVAILABLE_ROOMS_H