// Date.h
// Date class definition.
#ifndef DATA_H
#define DATA_H

class Date
{
public:
   Date();
   Date( int y, int m, int d );
   void setYear( int );
   void setMonth( int );
   void setDay( int );
   int getYear() const;
   int getMonth() const;
   int getDay() const;
   bool operator==( const Date &date2 );
   bool operator<( const Date &date2 );
   bool operator<=( const Date &date2 );
   bool operator>=( const Date &date2 );
   int operator-( const Date &date2 );
private:
	int year;
	int month;
   int day;
};

#endif // DATA_H