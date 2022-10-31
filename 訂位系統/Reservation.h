// Reservation.h
// Reservation class definition. Represents a reservation.
#ifndef RESERVATION_H
#define RESERVATION_H

#include "Date.h" // Date class definition

class Reservation
{
public:
   void setReservation( int, int, Date, Date, string, string, string );
   void setIDNumber( string );
   void setName( string );
   void setMobile( string );
   string getIDNumber();
   int getRoomType();
   int getNumRooms();
   Date getCheckInDate();
   Date getCheckOutDate();
   bool equalID( string id1, string id2 ); // return true iff id1 and id2 are equal where A == a, B == b, ...
   void displayReservation(); 
private:
   char IDNumber[ 12 ] = "";
   char name[ 12 ] = "";
   char mobile[ 12 ] = ""; // mobile phone number
   int roomType = 0; // 1: Superior Room  2: Deluxe Room  3: Deluxe Twin Room
                     // 4: Superior Suite  5: Deluxe Suite
   int numRooms = 0; // number of reserved rooms
   Date checkInDate = Date();
   Date checkOutDate = Date();
};

#endif // RESERVATION_H