// AvailableRoomsDatabase.h
// AvailableRoomsDatabase class definition.
#ifndef AVAILABLE_ROOMS_DATABASE_H
#define AVAILABLE_ROOMS_DATABASE_H

#include <vector>
#include "AvailableRooms.h" // AvailableRooms class definition

class AvailableRoomsDatabase
{
public:
   AvailableRoomsDatabase();
   ~AvailableRoomsDatabase();

   void displayAvailableRooms( Date checkInDate, Date checkOutDate );

   int findMinNumRooms( int roomType, const Date &checkInDate, const Date &checkOutDate );

   void decreaseAvailableRooms( int roomType, int numRooms,
                                const Date &checkInDate, const Date &checkOutDate );
private:
   AvailableRooms availableRooms[ 184 ]; // array of the available rooms for 6 months
   int numAvailableDays; // the number of days that are open for reservation

   void initilizeAvailableRooms( const Date &firstDate, int first, int numMonths );

   // remove outdated data from availableRooms, and
   // add new data into availableRooms if necessary
   void adjustAvailableRooms( const Date &currentDate );

   // return the index i in [ dateIndex, numAvailableDays ) such that availableRooms[ i ].date == date
   int getDateIndex( const Date &date, int dateIndex );

   void loadAvailableRooms();
   void saveAvailableRooms();
}; // end class AvailableRoomsDatabase

#endif // AVAILABLE_ROOMS_DATABASE_H