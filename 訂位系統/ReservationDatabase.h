// ReservationDatabase.h
// ReservationDatabase class definition. Represents reservation database.
#ifndef RESERVATION_DATABASE_H
#define RESERVATION_DATABASE_H

#include <vector>
#include "Reservation.h" // Reservation class definition

class ReservationDatabase
{
public:
   // call loadReservations and removes outdated reservation informations
   ReservationDatabase();

   // callsaveReservations()
   ~ReservationDatabase();

   bool empty();

   int numReservations( string IDNumber );
   void displayReservations( string IDNumber );
   void pushBack( Reservation );
private:
   vector< Reservation > reservations; // vector of the reservations

   void loadReservations();
   void saveReservations();
}; // end class ReservationDatabase

#endif // RESERVATION_DATABASE_H