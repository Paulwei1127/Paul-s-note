#include <iostream>
#include <iomanip>
using namespace std;

#include "MakeReservation.h" // MakeReservation class definition
#include "ViewReservation.h" // ViewReservation class definition

int displayMainMenu();
Date computeCurrentDate();
bool leapYear( int year ); // if the year is a leap year, return true; otherwise, return false
int inputAnInteger( int begin, int end );

int main()
{
   cout << "Evergreen Laurel Hotel Online Reservation System\n";

   ReservationDatabase reservationDatabase;
   AvailableRoomsDatabase availableRoomsDatabase;

   MakeReservation makeReservation( reservationDatabase, availableRoomsDatabase );
   ViewReservation viewReservation( reservationDatabase, availableRoomsDatabase );

   // enumeration constants represent submenu options
   enum MenuOption { MAKE_RESERVATION = 1, VIEW_CANCEL_RESERVATION, EXIT };
   bool userExited = false; // user has not chosen to exit

   // loop while user has not chosen option to exit system
   while( !userExited )
   {
      // show main menu and get user selection
      int mainMenuSelection = displayMainMenu();

      // decide how to proceed based on user's menu selection
      switch( mainMenuSelection )
      {
      case MAKE_RESERVATION:
         makeReservation.execute();
         break;
      case VIEW_CANCEL_RESERVATION:
         viewReservation.execute();
         break;
      case EXIT: // user chose to terminate session
         cout << "\nExiting the system...\n\n";
         userExited = true; // this ATM session should end
         break;
      default: // user did not enter an integer from 1-3
         cout << "\nYou did not enter a valid selection. Try again." << endl;
         break;
      } // end switch
   } // end while

   system( "pause" );
} // end main

// display the main menu and return an input selection
int displayMainMenu()
{
   cout << "\nMain menu:" << endl;
   cout << "1 - Make Reservation" << endl;
   cout << "2 - View/Cancel Reservation" << endl;
   cout << "3 - Exit\n";

   int choice;
   do cout << "\nEnter a choice: ";
   while( ( choice = inputAnInteger( 1, 3 ) ) == -1 );

   return choice; // return user's selection
} // end function displayMainMenu

Date computeCurrentDate()
{
//   int remainingSeconds = static_cast< int >( time( 0 ) ) + 8 * 60 * 60 + 165 * 24 * 60 * 60; // 2021-12-01
//   int remainingSeconds = static_cast< int >( time( 0 ) ) + 8 * 60 * 60 + 164 * 24 * 60 * 60; // 2021-11-30
   int remainingSeconds = static_cast< int >( time( 0 ) ) + 8 * 60 * 60; // 2021/6/19

   int year = 1970;

   int secondsPerYear = 365 * 24 * 60 * 60;
   if( leapYear( year ) )
      secondsPerYear += 24 * 60 * 60;

   while( remainingSeconds >= secondsPerYear )
   {
      remainingSeconds -= secondsPerYear;
      year++;

      secondsPerYear = 365 * 24 * 60 * 60;
      if( leapYear( year ) )
         secondsPerYear += 24 * 60 * 60;
   }

   int days[ 13 ] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
   if( leapYear( year ) )
      days[ 2 ] = 29;

   int month = 1;
   while( remainingSeconds >= days[ month ] * 24 * 60 * 60 )
   {
      remainingSeconds -= days[ month ] * 24 * 60 * 60;
      month++;
   }

   int day = remainingSeconds / ( 24 * 60 * 60 ) + 1;

   return Date( year, month, day );
}

bool leapYear( int year )
{
   return ( year % 400 == 0 || ( year % 4 == 0 && year % 100 != 0 ) );
}

int inputAnInteger( int begin, int end )
{
   char string[ 80 ];
   cin.getline( string, 80, '\n' );

   if( strlen( string ) == 0 )
      exit( 0 );

   for( unsigned int i = 0; i < strlen( string ); i++ )
      if( string[ i ] < '0' || string[ i ] > '9' )
         return -1;

   int number = atoi( string );

   if( number >= begin && number <= end )
      return number;
   else
      return -1;
}