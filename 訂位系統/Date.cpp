// Date.cpp
// Member-function definitions for class Date.
#include <iostream>
#include <iomanip>
#include <ctime>
using namespace std;
#include "Date.h"

extern bool leapYear( int year );

Date::Date()
{
   year = 2000;
   month = 1;
   day = 1;
}

Date::Date( int y, int m, int d )
{
   setYear( y );
   setMonth( m );
   setDay( d );
}

void Date::setYear( int y )
{
   year = ( y >= 2000 ) ? y : 2000; // sets year
} // end function setYear

void Date::setMonth( int m )
{
   month = ( m >= 1 && m <= 12 ) ? m : 1; // sets month
} // end function setMonth

void Date::setDay( int d )
{
   int days[ 13 ] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
   if( month == 2 && leapYear( year ) )
      day = ( d >= 1 && d <= 29 ) ? d : 1;
   else
      day = ( d >= 1 && d <= days[ month ] ) ? d : 1;
} // end function setDay

int Date::getYear() const
{
   return year;
}

int Date::getMonth() const
{
   return month;
}

int Date::getDay() const
{
   return day;
}

bool Date::operator==( const Date &date2 )
{
   return ( year == date2.year && month == date2.month && day == date2.day );
}

bool Date::operator<( const Date &date2 )
{
   if( year < date2.year )
      return true;
   if( year > date2.year )
      return false;

   if( month < date2.month )
      return true;
   if( month > date2.month )
      return false;

   if( day < date2.day )
      return true;

   return false;
}

bool Date::operator<=( const Date &date2 )
{
   return ( *this < date2 || *this == date2 );
}

bool Date::operator>=( const Date &date2 )
{
   return !( *this < date2 );
}

// return *this - date2 provided that *this > date2
int Date::operator-( const Date &date2 )
{
   int days[ 13 ] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
   if( leapYear( year ) )
      days[ 2 ] = 29;

   if( year == date2.year )
      if( month == date2.month )
         return day - date2.day;
      else // month > date2.month
      {
         int numDays = days[ date2.month ] - date2.day;
         for( int i = date2.month + 1; i < month; i++ )
            numDays += days[ i ];
         return numDays + day;
      }
   else // year > date2.year
   {
      int numDays = days[ date2.month ] - date2.day;
      for( int i = date2.month + 1; i < 13; i++ )
         numDays += days[ i ];
      for( int i = 1; i < month; i++ )
         numDays += days[ i ];
      return numDays + day;
   }
}