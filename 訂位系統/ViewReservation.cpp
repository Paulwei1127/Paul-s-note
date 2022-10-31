// ViewReservation.cpp
// Member-function definitions for class ViewReservation.
#include <iostream>
#include <string>
using namespace std;
#include "ViewReservation.h" // ViewReservation class definition

// extern bool legalID( const string &id );

// ViewReservation constructor initializes data members
ViewReservation::ViewReservation( ReservationDatabase &theReservationDatabase,
                                  AvailableRoomsDatabase &theAvailableRoomsDatabase )
   : reservationDatabase( theReservationDatabase ),
     availableRoomsDatabase( theAvailableRoomsDatabase )
{
} // end ViewReservation constructor

void ViewReservation::execute()
{
   if( reservationDatabase.empty() )
   {
      cout << "\nNo reservations!\n";
      return;
   }

   string IDNumber;
   cout << "\nPlease enter your ID number: ";
   getline( cin, IDNumber, '\n' );

   if( reservationDatabase.numReservations( IDNumber ) > 0 )
      reservationDatabase.displayReservations( IDNumber );
   else
      cout << "\nNo reservations with this ID number!\n";
}