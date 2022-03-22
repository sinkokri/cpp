#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class Company
{
  private:
      string        name;
      string        loweredName;
      string        addr;
      string        loweredAddr;
      string        taxID;
      unsigned int  invoiceSum;
  public:
    Company ( const string & name, const string & addr, const string & taxID )
        {
            this -> name = name;
            this -> loweredName = getLower( name );
            this -> addr = addr;
            this -> loweredAddr = getLower( addr );
            this -> taxID = taxID;
            this -> invoiceSum = 0;
        };

    ~ Company                                   ( void ) {};
    unsigned int    getInvoice                  ( void ) const;
    string          getName                     ( void ) const;
    string          getAddr                     ( void ) const;
    static string   getLower                    ( const string      & str );
    bool            isSameCompany               ( const Company     & x ) const;
    bool            isSameCompany               ( const string      & name,
                                                  const string      & addr ) const;
    bool            isSameCompany               ( const string      & taxID ) const;
    void            addInvoice                  ( unsigned int      amount );
    static bool     comparator                  ( const Company     & left ,
                                                  const Company     & right );
};

string Company::getAddr ( void ) const
{
    return this -> addr;
}

string Company::getName ( void ) const
{
    return this -> name;
}

unsigned int Company::getInvoice () const
{
    return invoiceSum;
}

void Company::addInvoice ( unsigned int amount )
{
    invoiceSum += amount;
}

bool Company::isSameCompany ( const string & taxID ) const
{
    return this->taxID == taxID;
}

bool Company::isSameCompany ( const Company & x ) const
{
    return  ( this -> loweredName == x . loweredName and
              this -> loweredAddr == x . loweredAddr ) or
            taxID   == x . taxID;
}

bool Company::isSameCompany ( const string & name, const string & addr ) const
{
    return this -> loweredName == getLower( name ) and
           this -> loweredAddr == getLower( addr );
}

bool Company::comparator ( const Company & left, const Company & right  )
{
    int i = right . loweredName .compare( left . loweredName );
    if  ( i == 0 )
    {
        int j = right . loweredAddr .compare( left . loweredAddr );
        if ( j == 0 or j > 0 )
            return true;
        else return false;
    }
    if ( i > 0 )
        return true;

    return false;
}

string Company::getLower ( const string & str )
{
    string tmp = str;
    for_each(tmp.begin(), tmp.end(), []( char & c ){
        c = tolower(c);
    });
    return tmp;
}

class CVATRegister
{
  private:
    vector <Company>       db;
    vector <unsigned int>  invoices;
    unsigned int           invoiceCount;

  public:
                  CVATRegister                  ( void ) { invoiceCount = 0; };
                  ~CVATRegister                 ( void ) {};
    bool          newCompany                    ( const string    & name,
                                                  const string    & addr,
                                                  const string    & taxID );
    bool          cancelCompany                 ( const string    & name,
                                                  const string    & addr );
    bool          cancelCompany                 ( const string    & taxID );
    bool          invoice                       ( const string    & taxID,
                                                  unsigned int      amount );
    bool          invoice                       ( const string    & name,
                                                  const string    & addr,
                                                  unsigned int      amount );
    bool          audit                         ( const string    & name,
                                                  const string    & addr,
                                                  unsigned int    & sumIncome ) const;
    bool          audit                         ( const string    & taxID,
                                                  unsigned int    & sumIncome ) const;
    bool          firstCompany                  ( string          & name,
                                                  string          & addr ) const;
    bool          nextCompany                   ( string          & name,
                                                  string          & addr ) const;
    unsigned int  medianInvoice                 ( void ) const;
    bool          companyExists                          ( const Company   & company) const;
    vector<Company>::const_iterator get ( const string    & name,
                                                  const string    & address ) const;
};
//-----------------------------------------------------
bool CVATRegister::companyExists( const Company & company ) const
{
    if ( auto pos = find_if(db.begin(), db.end(), [&] ( const Company & x )
        {
            return x . isSameCompany( company );
        } ); pos != db . end ())
        return true;
    return false;
}

bool CVATRegister::companyExists( const string & name, const string & address ) const
{
    if ( auto pos = find_if(db.begin(), db.end(), [&] ( const Company & x )
        {
            return x . isSameCompany( name , addr );
        } ); pos != db . end ())
        return true;
    return false;
}

bool CVATRegister::newCompany ( const string    & name,
                               const string    & addr,
                               const string    & taxID )
{
    Company company (name, addr, taxID);
    if (!companyExists(company))
    {
        db.push_back(company);
        return true;
    }
    return false;
}

bool CVATRegister::cancelCompany ( const string & name, const string & addr )
{
    string loweredName = Company::toLower(const_cast<string &>(name));
    string loweredAddr = Company::toLower(const_cast<string &>(addr));
    if ( auto pos = find_if(db.begin(), db.end(), [&] ( const Company & x )
        {
            return x . isSameCompany(  name , addr );
        } ); pos != db . end ())
    {
        *pos = db . back ();
        db . pop_back ();

        return true;
    }
    return false;
}

bool CVATRegister::cancelCompany ( const string & taxID )
{
    if ( auto pos = find_if(db.begin(), db.end(), [&] ( const Company & x )
        {
            return x . isSameCompany( name , addr );
        } ); pos != db . end ())
    {
        *pos = db . back ();
        db . pop_back ();

        return true;
    }
    return false;
}

bool CVATRegister::invoice( const string & taxID, unsigned int amount )
{
    if ( auto pos = find_if(db.begin(), db.end(), [&] ( const Company & x )
        {
            return x . isSameCompany( name , addr );
        } ); pos != db . end ())
    {
        invoiceCount += 1;
        invoices.insert(lower_bound(invoices.begin(), invoices.end(), amount), amount);
        return true;
    }
    return false;
}

bool CVATRegister::invoice ( const string & name, const string & addr, unsigned int amount )
{
    string loweredName = Company::toLower(const_cast<string &>(name));
    string loweredAddr = Company::toLower(const_cast<string &>(addr));

    if ( auto pos = find_if(db.begin(), db.end(), [&] ( const Company & x )
        {
            return x . isSameCompany( taxID );
        } ); pos != db . end ())
    {
        invoiceCount += 1;
        invoices.insert(lower_bound(invoices.begin(), invoices.end(), amount), amount);
        return true;
    }
    return false;

}

unsigned int CVATRegister::medianInvoice ( void ) const
{
        return invoiceCount == 0 ? 0 : invoices.at(invoiceCount/2);
}

#ifndef __PROGTEST__
int               main           ( void )
{
  string name, addr;
  unsigned int sumIncome;

  CVATRegister b1;
  assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
  assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
//  assert ( b1 . invoice ( "666/666", 2000 ) );
//  assert ( b1 . medianInvoice () == 2000 );
//  assert ( b1 . invoice ( "666/666/666", 3000 ) );
//  assert ( b1 . medianInvoice () == 3000 );
//  assert ( b1 . invoice ( "123456", 4000 ) );
//  assert ( b1 . medianInvoice () == 3000 );
//  assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
//  assert ( b1 . medianInvoice () == 4000 );
//  assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
//  assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
//  assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
//  assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
//  assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
//  assert ( ! b1 . nextCompany ( name, addr ) );
//  assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
//  assert ( b1 . medianInvoice () == 4000 );
//  assert ( b1 . cancelCompany ( "666/666" ) );
//  assert ( b1 . medianInvoice () == 4000 );
//  assert ( b1 . invoice ( "123456", 100 ) );
//  assert ( b1 . medianInvoice () == 3000 );
//  assert ( b1 . invoice ( "123456", 300 ) );
//  assert ( b1 . medianInvoice () == 3000 );
//  assert ( b1 . invoice ( "123456", 200 ) );
//  assert ( b1 . medianInvoice () == 2000 );
//  assert ( b1 . invoice ( "123456", 230 ) );
//  assert ( b1 . medianInvoice () == 2000 );
//  assert ( b1 . invoice ( "123456", 830 ) );
//  assert ( b1 . medianInvoice () == 830 );
//  assert ( b1 . invoice ( "123456", 1830 ) );
//  assert ( b1 . medianInvoice () == 1830 );
//  assert ( b1 . invoice ( "123456", 2830 ) );
//  assert ( b1 . medianInvoice () == 1830 );
//  assert ( b1 . invoice ( "123456", 2830 ) );
//  assert ( b1 . medianInvoice () == 2000 );
//  assert ( b1 . invoice ( "123456", 3200 ) );
//  assert ( b1 . medianInvoice () == 2000 );
//  assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
//  assert ( ! b1 . nextCompany ( name, addr ) );
//  assert ( b1 . cancelCompany ( "123456" ) );
//  assert ( ! b1 . firstCompany ( name, addr ) );
//
//  CVATRegister b2;
//  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
//  assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
//  assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
//  assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
//  assert ( b2 . medianInvoice () == 0 );
//  assert ( b2 . invoice ( "ABCDEF", 1000 ) );
//  assert ( b2 . medianInvoice () == 1000 );
//  assert ( b2 . invoice ( "abcdef", 2000 ) );
//  assert ( b2 . medianInvoice () == 2000 );
//  assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
//  assert ( b2 . medianInvoice () == 2000 );
//  assert ( ! b2 . invoice ( "1234567", 100 ) );
//  assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
//  assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
//  assert ( ! b2 . audit ( "1234567", sumIncome ) );
//  assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
//  assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
//  assert ( ! b2 . cancelCompany ( "1234567" ) );
//  assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
//  assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
//  assert ( b2 . cancelCompany ( "abcdef" ) );
//  assert ( b2 . medianInvoice () == 2000 );
//  assert ( ! b2 . cancelCompany ( "abcdef" ) );
//  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
//  assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
//  assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
