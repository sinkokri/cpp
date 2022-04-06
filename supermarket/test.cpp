#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

//===========================================================================================
class CDate
{
private:
    int initYear;
    int initMonth;
    int initDay;
    int year;
    int month;
    int day;
    int totalDays = 0 ;
    int maxDays;
    bool leap;
    vector<int> monthLengths = { 0, 31, 28, 31,
                                 30, 31, 30, 31,
                                 31, 30, 31, 30, 31 };

  public:
    CDate               ( int year, int month, int day );
    bool operator >     ( const CDate & other ) const;
    bool operator <     ( const CDate & other ) const;
    void toDays         ( void );
    void previousMonth  ( void );
    void previousYear   ( void );
    void resetMaxDays   ( void );
    void checkLeapYear  ( void );
};
//===========================================================================================
CDate::CDate (int year, int month, int day)
{
    this -> year = year;
    this -> initYear = year;

    checkLeapYear();

    this -> maxDays = this -> monthLengths.at( month );

    if ( month < 1    ||
         month > 12   ||
         day   < 1    ||
         day   > this -> maxDays )
        throw exception();

    this -> month = month;
    this -> initMonth = month;
    this -> day = day;
    this -> initDay = day;
}
//===========================================================================================
bool CDate::operator < ( const CDate & other ) const
{
    CDate first = ( * this );
    first . toDays();
    CDate second = other;
    second . toDays();

    return first . totalDays < second . totalDays;
}
//===========================================================================================
bool CDate::operator > ( const CDate & other ) const
{
    CDate first = ( * this );
    first . toDays();
    CDate second = other;
    second . toDays();

    return first . totalDays > second . totalDays;
}
//===========================================================================================
void CDate::toDays ( void )
{
    int count = 0;
    count += this -> day;
    while ( this -> year > 2000 )
    {
        previousMonth();
        count += monthLengths.at(this -> month);
    }
    while ( this -> month > 1)
    {
        previousMonth();
        count += monthLengths.at(this -> month);
    }
    this -> totalDays = count;
}
//===========================================================================================
void CDate::previousMonth( void )
{
    if ( this-> month == 1 )
    {
        this-> month = 12;
        previousYear();
    }
    else this-> month -= 1;
    resetMaxDays();
}
//===========================================================================================
void CDate::resetMaxDays( void )
{
    this -> maxDays = this -> monthLengths.at ( this -> month);
}
//===========================================================================================
void CDate::previousYear( void )
{
    this -> year -= 1;
    checkLeapYear();
}
//===========================================================================================
void CDate::checkLeapYear( void )
{
    if (this -> year % 4 == 0)
    {
        if (this -> year % 100 == 0)
        {
            if (this -> year % 400 == 0)
                this -> leap = true;
            else this -> leap = false;
        }
        else this -> leap = true;
    }
    else this -> leap = false;

    // directly resetting Feb days if it's Leap
    if ( this -> leap ) this -> monthLengths.at( 2 ) = 29;
    else this -> monthLengths.at( 2 ) = 28;
}
//===========================================================================================
class Product
{
public:
    string name;
    CDate expiryDate;
    Product                 ( string name, CDate expiryDate)
    : name ( name ), expiryDate ( expiryDate ) {}

    bool operator ==        ( const Product & other ) const;
    static bool comparator  ( const Product& lhs, const Product& rhs );
};
//===========================================================================================
bool Product::operator == ( const Product & other ) const
{
    return ( * this ) . name == other . name;
}
//===========================================================================================
bool Product::comparator (const Product & lhs, const Product & rhs)
{
    return lhs.name < rhs.name;
}
//===========================================================================================

struct productCompare
{
    bool operator () (const Product &lhs, const Product &rhs) const {
        if (lhs.name != rhs.name)
            return lhs.name < rhs.name;
        return lhs.expiryDate > rhs.expiryDate;
    }
};
//===========================================================================================
class CSupermarket
{
private:
    map <Product, int, productCompare> warehouse;
    set <string> productsList;
            
  public:
    CSupermarket () {};

    CSupermarket &          store           ( const string & name, const CDate & expiryDate, int count );
    void                    sell            ( list<pair<string,int> > & shoppingList );
    list<pair<string,int> > expired         ( const CDate date );
    static bool             comparator      ( const pair <string,int>& first, const pair <string,int>& second );
};
//===========================================================================================
CSupermarket & CSupermarket::store ( const string & name, const CDate & expiryDate, int count )
{
    Product product ( name, expiryDate );
    if ( warehouse . count( product ))
        warehouse [ product ] += count ;
    else
        warehouse [ product ] = count ;
    productsList .insert(name );
    return ( * this );
}
//===========================================================================================
bool CSupermarket::comparator (const pair <string,int>& first, const pair <string,int>& second)
{
    return  first .second > second. second ;
}
//===========================================================================================
list < pair <string,int> > CSupermarket::expired ( const CDate date )
{
    list <pair <string,int>> bucket;
    for ( const auto & item: warehouse )
    {
        if ( item . first . expiryDate < date )
        {
            pair <string, int> pair (item . first .name, item . second);

            if ( bucket .back() .first  == item . first. name)
            {
                int amount = bucket .back() . second + item.second;
                bucket . pop_back();
                bucket . emplace_back(item . first .name, amount );
            }
             else {
                bucket . push_back( pair );
             }
        }
    }
    if ( ! bucket . empty() )
        bucket . sort( comparator );
    return bucket;
}
//===========================================================================================
//===========================================================================================
#ifndef __PROGTEST__
int main ( void )
{
  CSupermarket s;
  s . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
    . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
    . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
    . store ( "bread", CDate ( 2016, 4, 25 ), 100 )
    . store ( "okey", CDate ( 2016, 7, 18 ), 5 );
//
//  list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
//  assert ( l0 . size () == 4 );
//  assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );
//
//  list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
//  s . sell ( l1 );
//  assert ( l1 . size () == 2 );
//  assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );
//
//  list<pair<string,int> > l2 = s . expired ( CDate ( 2016, 4, 30 ) );
//  assert ( l2 . size () == 1 );
//  assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );
//
//  list<pair<string,int> > l3 = s . expired ( CDate ( 2016, 5, 20 ) );
//  assert ( l3 . size () == 1 );
//  assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );
//
//  list<pair<string,int> > l4 { { "bread", 105 } };
//  s . sell ( l4 );
//  assert ( l4 . size () == 0 );
//  assert ( ( l4 == list<pair<string,int> > {  } ) );
//
//  list<pair<string,int> > l5 = s . expired ( CDate ( 2017, 1, 1 ) );
//  assert ( l5 . size () == 3 );
//  assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );
//
//  s . store ( "Coke", CDate ( 2016, 12, 31 ), 10 );
//
//  list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
//  s . sell ( l6 );
//  assert ( l6 . size () == 3 );
//  assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );
//
//  list<pair<string,int> > l7 = s . expired ( CDate ( 2017, 1, 1 ) );
//  assert ( l7 . size () == 4 );
//  assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );
//
//  s . store ( "cake", CDate ( 2016, 11, 1 ), 5 );
//
//  list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
//  s . sell ( l8 );
//  assert ( l8 . size () == 2 );
//  assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );
//
//  list<pair<string,int> > l9 = s . expired ( CDate ( 2017, 1, 1 ) );
//  assert ( l9 . size () == 5 );
//  assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );
//
//  list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
//  s . sell ( l10 );
//  assert ( l10 . size () == 2 );
//  assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );
//
//  list<pair<string,int> > l11 = s . expired ( CDate ( 2017, 1, 1 ) );
//  assert ( l11 . size () == 4 );
//  assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );
//
//  list<pair<string,int> > l12 { { "Cake", 4 } };
//  s . sell ( l12 );
//  assert ( l12 . size () == 0 );
//  assert ( ( l12 == list<pair<string,int> > {  } ) );
//
//  list<pair<string,int> > l13 = s . expired ( CDate ( 2017, 1, 1 ) );
//  assert ( l13 . size () == 4 );
//  assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );
//
//  list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
//  s . sell ( l14 );
//  assert ( l14 . size () == 1 );
//  assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );
//
//  s . store ( "ccccb", CDate ( 2019, 3, 11 ), 100 )
//    . store ( "ccccd", CDate ( 2019, 6, 9 ), 100 )
//    . store ( "dcccc", CDate ( 2019, 2, 14 ), 100 );
//
//  list<pair<string,int> > l15 { { "ccccc", 10 } };
//  s . sell ( l15 );
//  assert ( l15 . size () == 1 );
//  assert ( ( l15 == list<pair<string,int> > { { "ccccc", 10 } } ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
