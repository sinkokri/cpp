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
    bool operator >     ( const CDate & other ) const;
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
bool CDate::operator > ( const CDate & other ) const
{
    CDate first = ( * this );

    if ( first . year  != other . year )
        return first . year > other . year;
    if ( first . month != other . month )
        return first . month > other . month;
    return  first . day > other . day;
}
//===========================================================================================
class Product
{
public:
    string name;
    CDate expiryDate;
    Product ( const string & name, const CDate & expiryDate)
            : name ( name ), expiryDate ( expiryDate )
    {}
};
//===========================================================================================
struct productCompareStoreLatest
{
    bool operator () ( const Product & lhs, const Product & rhs ) const
    {
        if ( lhs . name != rhs . name)
            return lhs . name < rhs . name;
        return lhs . expiryDate > rhs . expiryDate;
    }
};
//===========================================================================================
struct productCompareStoreNewest
{
    bool operator () ( const Product & lhs, const Product & rhs ) const
    {
        if ( lhs . name != rhs . name )
            return lhs . name < rhs . name;
        return lhs . expiryDate < rhs . expiryDate;
    }
};
//===========================================================================================
class CSupermarket
{
private:
    map <Product, int, productCompareStoreLatest> warehouseSortedByLatest;
    map <Product, int, productCompareStoreNewest> warehouseSortedByNewest;
    set <string> productsList;

public:
    CSupermarket () {};

    CSupermarket &          store           ( const string & name, const CDate & expiryDate, int count );
    void                    sell            ( list<pair<string,int> > & shoppingList );
    list<pair<string,int> > expired         ( const CDate & date );
    static bool             comparator      ( const pair <string,int>& first, const pair <string,int>& second );
    bool itemMatch                          ( string & value );
};
//===========================================================================================
CSupermarket & CSupermarket::store ( const string & name, const CDate & expiryDate, int count )
{
    Product product ( name, expiryDate );
    if ( warehouseSortedByLatest . count ( product ) )
    {
        warehouseSortedByLatest [ product ] += count ;
        warehouseSortedByNewest [ product ] += count ;
    }
    else
    {
        warehouseSortedByLatest [ product ] = count ;
        warehouseSortedByNewest [ product ] = count ;
    }

    productsList . insert (name );
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
}//===========================================================================================
//bool operator < (const Product & lhs, const Product & rhs)
//{
//    return lhs.name < rhs.name;
//}

bool CSupermarket::itemMatch ( string & value  )
{
    if ( productsList.empty() )
        return false;

    bool moreDiffs = false;
    string match;

    for ( auto str : productsList )
    {
        if ( str . size() != value.size() )
            continue;

        bool oneStringDiff = false;

        for ( int i = 0; i < str.size(); i++ )
        {
            if ( str [i] != value [i] )
            {
                // If first mismatch - ok
                if ( !oneStringDiff )
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
            match = str;
            continue;
        }

        // already more words with one char mismatch exist
        if ( oneStringDiff and moreDiffs )
            return false;
    }

    if ( not match .empty() )
    {
        value = match;
        return true;
    }

    return false;
}

template <typename T>struct addressEquals
{
    addressEquals(T * ptr) : ptr_(ptr) {}
    bool operator()(pair<string, int> & element) { return &element == ptr_; }
    T * ptr_;
};
//===========================================================================================
void CSupermarket::sell ( list<pair<string,int> > & shoppingList )
{
//    std::list<pair<string,int>>::const_iterator iterItem;
//    while ( i != shoppingList.end() )
//    auto pos = shoppingList.begin();
//    for ( auto & itemToBuy: shoppingList )

//    typedef vector <pair<string,int>> tListTest;
//    typedef vector<tListTest::iterator> toProcess;
    vector<pair<string, int> *> toProcess;
    for ( auto itemToBuy = shoppingList . begin(); itemToBuy != shoppingList . end(); ++ itemToBuy )
    {
        // if product is not in the product list --
        // either by exact match or by one char mismatch --
        // add it ( already changed if the name was incorrect) to processing list
        if ( productsList . count ( itemToBuy -> first ) or itemMatch ( itemToBuy -> first ) )
            toProcess . push_back( &(*itemToBuy));
    }

    for ( auto itemToProcess = toProcess . begin(); itemToProcess != toProcess . end(); ++ itemToProcess )
    {
        string name = ( * itemToProcess ) -> first;
        // find the item in warehouse - first found is the latest expired
        auto itemInWarehouse = warehouse . lower_bound( Product( name,
                                                                 CDate(2100, 12, 31)) );
        while ( itemInWarehouse != warehouse.end() and itemInWarehouse -> first . name == name )
        {
            // if the WH item has count equal or less than in the shopping list -
            // remove it from WH and keep the item in shopping list with corrected count
            int warehouseItemCount = itemInWarehouse -> second;
            if ( warehouseItemCount <= ( * itemToProcess ) -> second )
            {
                ( * itemToProcess ) -> second -= warehouseItemCount;

                if ( ( * itemToProcess ) -> second == 0 )
                {
                    shoppingList . remove_if( addressEquals<pair<string, int>> (*itemToProcess) );
                }

                    warehouse . erase( itemInWarehouse );

                    // checking if there are other matched items left
                    itemInWarehouse = warehouse . lower_bound( Product( name,
                                                                        CDate(2100, 12, 31)) );
                    if ( itemInWarehouse == warehouse.end() or itemInWarehouse -> first . name != name )
                        productsList . erase ( name );
            }
                // if the WH has more items than needed -
                // deduct the count from WH and remove item from shopping list
            else
            {
                itemInWarehouse -> second -= ( * itemToProcess ) -> second;
                shoppingList . remove_if( addressEquals<pair<string, int>> (*itemToProcess) );
                break;
            }
        }
    }
}

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
