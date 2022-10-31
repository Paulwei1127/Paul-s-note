// ReservationDatabase.cpp
// Member-function definitions for class ReservationDatabase.
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace::std;
#include "ReservationDatabase.h" // ReservationDatabase class definition

extern Date computeCurrentDate();

// call loadReservations and removes outdated reservation informations
ReservationDatabase::ReservationDatabase()
{
	loadReservations();

   if( reservations.size() != 0 )
   {
      Date currentDate = computeCurrentDate();
      vector< Reservation >::iterator it = reservations.begin();
      while( it != reservations.end() )
         if( it->getCheckInDate() < currentDate )
            it = reservations.erase( it );
         else
            ++it;
   }
}

ReservationDatabase::~ReservationDatabase()
{
   saveReservations();
}

bool ReservationDatabase::empty()
{
   return reservations.size() == 0;
}

void ReservationDatabase::pushBack( Reservation newReservation )
{
   reservations.push_back( newReservation );
}

int ReservationDatabase::numReservations( string IDNumber )
{
   int count = 0;
   for( vector< Reservation >::iterator it = reservations.begin(); it < reservations.end(); it++ )
      if( it->equalID( IDNumber, it->getIDNumber() ) )
         count++;

   return count;
}

void ReservationDatabase::displayReservations( string IDNumber )
{
	cout << "\n    Name   Fare       Mobile          Room type   Number of rooms   Check in date   Check out date\n";

   for( vector< Reservation >::iterator it = reservations.begin(); it < reservations.end(); it++ )
      if( it->equalID( IDNumber, it->getIDNumber() ) )
         it->displayReservation();
}

void ReservationDatabase::loadReservations()
{
	ifstream inReservationFile( "Reservations.dat", ios::binary );

	if( !inReservationFile )
	{
		cout << "File could not be opened!\n";
      system( "pause" );
		exit(1);
	}

   Reservation buffer;
	while ( inReservationFile.read( reinterpret_cast< char * >( &buffer ), sizeof( buffer ) ) )
      reservations.push_back( buffer );

	inReservationFile.close();
}

void ReservationDatabase::saveReservations()
{
	ofstream outReservationFile( "Reservations.dat", ios::binary );

	if( !outReservationFile )
	{
		cout << "File could not be opened!\n";
      system( "pause" );
		exit(1);
	}

   for( vector< Reservation >::iterator it = reservations.begin(); it < reservations.end(); it++ )
		outReservationFile.write( reinterpret_cast< const char * >( &(*it) ), sizeof( Reservation ) );

	outReservationFile.close();
}