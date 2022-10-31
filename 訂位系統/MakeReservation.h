// MakeReservation.h
// MakeReservation class definition.
#ifndef MAKE_RESERVATION_H
#define MAKE_RESERVATION_H

#include "ReservationDatabase.h" // ReservationDatabase class definition
#include "AvailableRoomsDatabase.h" // AvailableRoomsDatabase class definition

class MakeReservation
{
public:
   MakeReservation( ReservationDatabase &, AvailableRoomsDatabase & );
   void execute();
private:
   Reservation newReservation;
   ReservationDatabase &reservationDatabase;
   AvailableRoomsDatabase &availableRoomsDatabase;

   void computeAvailableMonths( Date currentDate, Date availableMonths[] );
   void inputCheckInDate( Date &checkInDate, Date, Date[], int &, int &, int & );
   void inputCheckOutDate( Date &checkOutDate, Date, Date[], int, int, int );
}; // end class MakeReservation

#endif // MAKE_RESERVATION_H