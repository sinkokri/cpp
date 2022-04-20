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
    int year;
    int month;
    int day;

public:
    CDate               ( int year, int month, int day )
            : year ( year ), month ( month ), day ( day )
    {};
    bool operator <     ( const CDate & other ) const;
};
//===========================================================================================
bool CDate::operator < ( const CDate & other ) const
{
    CDate first = ( * this );

    if ( first . year  != other . year )
        return first . year < other . year;
    if ( first . month != other . month )
        return first . month < other . month;
    return  first . day < other . day;
}
//===========================================================================================
class CSupermarket
{
private:
    unordered_map<string, map<CDate, int>> warehouse;

public:
    CSupermarket () {};

    CSupermarket &          store        ( const string & name, const CDate & expiryDate, int count );
    void                    sell         ( list<pair<string,int> > & shoppingList );
    list <pair<string,int>> expired      ( const CDate & date ) const;
    static bool             comparator   ( const pair <string,int>& first, const pair <string,int>& second );
    bool                    itemMatch    ( string & value );
};
//===========================================================================================
CSupermarket & CSupermarket::store ( const string & name, const CDate & expiryDate, int count )
{
    if ( warehouse . count ( name ) )
    {
        if ( warehouse [ name ] .count( expiryDate ) )
            warehouse [ name ] [ expiryDate ] += count;
        else warehouse [ name ] [ expiryDate ] = count;
    }
    else
        warehouse [ name ] [ expiryDate ] = count;

    return ( * this );
}
//===========================================================================================
bool CSupermarket::comparator ( const pair <string,int> & first, const pair <string,int> & second )
{
    return  first . second > second . second ;
}
//===========================================================================================
list < pair <string,int> > CSupermarket::expired ( const CDate & date ) const
{
    list<pair<string, int>> basket;
    for (  auto & product : warehouse )
    {
        auto item = product . second . begin();
        while ( item -> first < date and item !=  product . second . end() )
        {
            pair<string, int> pair ( product . first , item -> second );

            if ( ! basket.empty() )
            {
                if ( basket . back() . first == product . first )
                    basket . back() . second += item -> second;

                else basket . push_back( pair );
            }
            else basket . push_back ( pair );
            std::advance ( item, 1 );
        }
    }

    if ( !basket.empty() )
        basket . sort ( comparator );

    return basket;
}
//===========================================================================================
bool CSupermarket::itemMatch ( string & value  )
{
     if ( warehouse . empty() )
         return false;

     bool moreDiffs = false;
     string match;

     for ( auto str : warehouse )
     {
         if ( str . first . size() != value.size() )
             continue;

         bool oneStringDiff = false;

         for ( size_t i = 0; i < str . first . size(); i++ )
         {
             if ( str . first [i] != value [i] )
             {
                 // If first mismatch - ok
                 if ( ! oneStringDiff )
                     oneStringDiff = true;
                     // Second mismatch
                 else
                 {
                     oneStringDiff = false;
                     break;
                 }
             }
         }
         // found first word with one char mismatch -> ok, but need to check the rest
         if ( oneStringDiff and not moreDiffs )
         {
             moreDiffs = true;
             match = str . first;
             continue;
         }

         // already more words with one char mismatch exist
         if ( oneStringDiff and moreDiffs )
             return false;
     }

     if ( ! match .empty() )
     {
         value = match;
         return true;
     }

     return false;
}
//===========================================================================================
template <typename T>
struct addressEquals
{
    addressEquals ( T * ptr ) : ptr_( ptr ) {}
    bool operator () ( pair<string, int> & element ) const
    {
        return &element == ptr_;
    }
    T * ptr_;
};
//===========================================================================================
void CSupermarket::sell ( list<pair<string,int> > & shoppingList )
{
    vector<pair<string, int> *> toProcess;
    for ( auto itemToSell = shoppingList . begin(); itemToSell != shoppingList . end(); ++ itemToSell)
    {
        // if product to sell is in the product list --
        // either by exact match or by one char mismatch --
        // add it ( already changed if the name was incorrect) to processing list of pointers
        if ( warehouse . count ( itemToSell -> first ) or itemMatch ( itemToSell -> first ))
            toProcess . push_back( & (*itemToSell)  );
    }

    for ( auto itemToProcess = toProcess . begin(); itemToProcess != toProcess . end(); ++ itemToProcess  )
    {
        string name = ( * itemToProcess ) -> first;
        if ( warehouse . count ( name ) )
        {
            auto item = warehouse [ name ] .begin();
            while ( item !=  warehouse [ name ] . end())
            {
                // if the WH item has count equal or less than in the shopping list -
                // remove it from WH and keep the item in shopping list with corrected count
                if ( item -> second <= ( * itemToProcess ) -> second )
                {
                    ( * itemToProcess ) -> second -= item -> second;
                    auto tmp = std::next(item, 1);
                    warehouse [ name ] . erase( item );
                    if ( warehouse [ name ] .empty() )
                    {
                        warehouse .erase( name );
                        break;
                    }
                    item = tmp;
                    if ( ( * itemToProcess ) -> second == 0 )
                    {
                        shoppingList . remove_if( addressEquals<pair<string, int>> (* itemToProcess ) );
                        break;
                    }
                }
                else
                    // if the WH has more items than needed to sell -
                    // deduct the count from WH and remove item from shopping list
                {
                    item -> second -= ( * itemToProcess ) -> second;
                    std::advance( item, 1 );
                    shoppingList . remove_if( addressEquals<pair<string, int>> (* itemToProcess ) );
                    break;
                }
            }
        }
    }
}

//===========================================================================================
#ifndef __PROGTEST__
int main ( void ) {
    CSupermarket s;
    s . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
            . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
            . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
            . store ( "bread", CDate ( 2016, 4, 25 ), 100 )
            . store ( "okey", CDate ( 2016, 7, 18 ), 5 );

    list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
    assert ( l0 . size () == 4 );
    assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );

    list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
    s . sell ( l1 );
    assert ( l1 . size () == 2 );
    assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );

    list<pair<string,int> > l2 = s . expired ( CDate ( 2016, 4, 30 ) );
    assert ( l2 . size () == 1 );
    assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );

    list<pair<string,int> > l3 = s . expired ( CDate ( 2016, 5, 20 ) );
    assert ( l3 . size () == 1 );
    assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );

    list<pair<string,int> > l4 { { "bread", 105 } };
    s . sell ( l4 );
    assert ( l4 . size () == 0 );
    assert ( ( l4 == list<pair<string,int> > {  } ) );

    list<pair<string,int> > l5 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l5 . size () == 3 );
    assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );

    s . store ( "Coke", CDate ( 2016, 12, 31 ), 10 );

    list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
    s . sell ( l6 );
    assert ( l6 . size () == 3 );
    assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );

    list<pair<string,int> > l7 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l7 . size () == 4 );
    assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );

    s . store ( "cake", CDate ( 2016, 11, 1 ), 5 );

    list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
    s . sell ( l8 );
    assert ( l8 . size () == 2 );
    assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );

    list<pair<string,int> > l9 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l9 . size () == 5 );
    assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );

    list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
    s . sell ( l10 );
    assert ( l10 . size () == 2 );
    assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );

    list<pair<string,int> > l11 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l11 . size () == 4 );
    assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );

    list<pair<string,int> > l12 { { "Cake", 4 } };
    s . sell ( l12 );
    assert ( l12 . size () == 0 );
    assert ( ( l12 == list<pair<string,int> > {  } ) );

    list<pair<string,int> > l13 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l13 . size () == 4 );
    assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );

    list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
    s . sell ( l14 );
    assert ( l14 . size () == 1 );
    assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );

    s . store ( "ccccb", CDate ( 2019, 3, 11 ), 100 )
            . store ( "ccccd", CDate ( 2019, 6, 9 ), 100 )
            . store ( "dcccc", CDate ( 2019, 2, 14 ), 100 );

    list<pair<string,int> > l15 { { "ccccc", 10 } };
    s . sell ( l15 );
    assert ( l15 . size () == 1 );
    assert ( ( l15 == list<pair<string,int> > { { "ccccc", 10 } } ) );
//=======================================
    s . store ( "okey", CDate ( 2029, 3, 11 ), 10 );
    list<pair<string,int> > l16 { { "okey", 5 }, {"okey", 15}, {"okeY", 4} };
    s . sell ( l16 );
    assert ( l16 . size () == 2 );
    assert ( ( l16 == list<pair<string,int> > { { "okey", 5 },{ "okey", 4 } } ) );

    list<pair<string,int> > l17 { { "cccca", 99 } };
    s . sell ( l17 );
    assert ( l17 . size () == 1 );
    assert ( ( l17 == list<pair<string,int> > { { "cccca", 99 } } ) );

    s . store ( "coffee", CDate ( 2000, 1, 1 ), 10 )
            . store ( "coffee", CDate ( 2001, 1, 1 ), 10 );
    list<pair<string,int> > l { { "coffee", 10 } };
    s . sell ( l );


    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */