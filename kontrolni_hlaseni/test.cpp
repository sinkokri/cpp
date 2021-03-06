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
    string        name = "";
    string        loweredName = "";
    string        addr = "";
    string        loweredAddr = "";
    string        taxID = "";
    unsigned int  invoiceSum = 0;
public:
    Company ( const string & name, const string & addr, const string & taxID ) :
    name ( name ),
    addr ( addr ),
    taxID( taxID )
    {
        this -> loweredName = getLower( name );
        this -> loweredAddr = getLower( addr );
    };
    Company ( const string & name, const string & addr ) :
    name ( name ),
    addr ( addr )
    {
        this -> loweredName = getLower( name );
        this -> loweredAddr = getLower( addr );
    }
    Company ( const string & taxID ) :
    taxID( taxID )
    {}

    ~ Company                                   ( void ) {};
    unsigned int    getInvoice                  ( void ) const { return this -> invoiceSum; };
    string          getName                     ( void ) const { return this -> name; };
    string          getAddr                     ( void ) const { return this -> addr; };
    string          getTaxId                    ( void ) const { return this -> taxID; };
    string          getLoweredName              ( void ) const { return this -> loweredName; };
    string          getLoweredAddr              ( void ) const { return this -> loweredAddr; };
    void            addInvoice                  ( unsigned int      amount ) { this -> invoiceSum += amount; };
    static string   getLower                    ( const string      & str );
    bool            isSameCompany               ( const string      & name,
                                                  const string      & addr ) const;
    bool            isSameCompany               ( const string      & taxID ) const;

    static bool     companyComparator           ( const Company     & left ,
                                                  const Company     & right );
    static bool     taxComparator               ( const Company & left,
                                                  const Company & right );
};
//-----------------------------------------------------
bool Company::isSameCompany ( const string & taxID ) const
{
    return this->taxID == taxID;
}
//-----------------------------------------------------
bool Company::isSameCompany ( const string & name, const string & addr ) const
{
    return this -> loweredName == getLower( name ) and
           this -> loweredAddr == getLower( addr );
}
//-----------------------------------------------------
bool Company::companyComparator ( const Company & left, const Company & right  )
{
    if (left.loweredName != right.loweredName)
        return left.loweredName < right.loweredName;
    return left.loweredAddr < right.loweredAddr;
}
//-----------------------------------------------------
bool Company::taxComparator ( const Company & left, const Company & right  )
{
    return left. taxID < right . taxID;
}
//-----------------------------------------------------
string Company::getLower ( const string & str )
{
    string tmp = str;
    for_each(tmp.begin(), tmp.end(), []( char & c ){
        c = tolower(c);
    });
    return tmp;
}
//-----------------------------------------------------
class CVATRegister
{
private:
    vector <Company>       dbPerNameAddr;
    vector <Company>       dbPerTaxId;
    vector <unsigned int>  invoices;
    unsigned int           invoiceCount = 0;

public:
    CVATRegister                                          ( ) {}
    ~CVATRegister                                         ( void ) {};
    bool          newCompany                              ( const string    & name,
                                                            const string    & addr,
                                                            const string    & taxID );
    bool          cancelCompany                           ( const string    & name,
                                                            const string    & addr );
    bool          cancelCompany                           ( const string    & taxID );
    bool          invoice                                 ( const string    & taxID,
                                                            unsigned int      amount );
    bool          invoice                                 ( const string    & name,
                                                            const string    & addr,
                                                            unsigned int      amount );
    bool          audit                                   ( const string    & name,
                                                            const string    & addr,
                                                            unsigned int    & sumIncome ) const;
    bool          audit                                   ( const string    & taxID,
                                                            unsigned int    & sumIncome ) const;
    bool          firstCompany                            ( string          & name,
                                                            string          & addr ) const;
    bool          nextCompany                             ( string          & name,
                                                            string          & addr ) const;
    unsigned int  medianInvoice                           ( void ) const;
    vector<Company>::iterator findCompanyPerNameAddr      ( const Company    & company );
    vector<Company>::iterator findCompanyPerTaxId         ( const Company    & company );
    vector<Company>::const_iterator findCompanyPerNameAddr( const Company & company ) const;
    vector<Company>::const_iterator findCompanyPerTaxId   ( const Company & company ) const;
};
//-----------------------------------------------------
vector<Company>::iterator CVATRegister::findCompanyPerNameAddr( const Company & company )
{
    return   std::lower_bound(
            dbPerNameAddr.begin(),
            dbPerNameAddr.end(),
            company,
            Company::companyComparator);
}
//-----------------------------------------------------
vector<Company>::const_iterator CVATRegister::findCompanyPerNameAddr( const Company & company ) const
{
    return std::lower_bound(
            dbPerNameAddr.begin(),
            dbPerNameAddr.end(),
            company,
            Company::companyComparator);
}
//-----------------------------------------------------
vector<Company>::iterator CVATRegister::findCompanyPerTaxId( const Company & company )
{
    return std::lower_bound(
            dbPerTaxId.begin(),
            dbPerTaxId.end(),
            company,
            Company::taxComparator);
}
//-----------------------------------------------------
vector<Company>::const_iterator CVATRegister::findCompanyPerTaxId( const Company & company ) const
{
    return std::lower_bound(
            dbPerTaxId.begin(),
            dbPerTaxId.end(),
            company,
            Company::taxComparator);
}
//-----------------------------------------------------
bool CVATRegister::newCompany ( const string & name,
                                const string & addr,
                                const string & taxID )
{
    Company company ( name, addr, taxID );
    auto posNameAddr = findCompanyPerNameAddr ( company );
    auto posTaxId = findCompanyPerTaxId ( company );
    if ( ( posNameAddr == dbPerNameAddr . end () || !( *posNameAddr ).isSameCompany( name, addr ) )
     && ( posTaxId == dbPerNameAddr . end () || !( *posNameAddr ).isSameCompany( taxID )))
    {
        dbPerNameAddr.insert(posNameAddr,
                            company);
        dbPerTaxId.insert(posTaxId,
                          company);
        return true;
    }
    return false;
}
//-----------------------------------------------------
bool CVATRegister::cancelCompany ( const string & name, const string & addr )
{
    Company tempPerNameAddr ( name, addr );
    auto posPerNameAddr = findCompanyPerNameAddr ( tempPerNameAddr  );
    if ( posPerNameAddr != dbPerNameAddr . end () && ( * posPerNameAddr ).isSameCompany( name, addr )  )
    {
        Company tempPerTaxId ( posPerNameAddr->getTaxId() );
        auto posPerTaxId = findCompanyPerTaxId ( tempPerTaxId  );
        dbPerNameAddr.erase(posPerNameAddr);
        dbPerTaxId.erase(posPerTaxId);
        return true;
    }
    return false;
}
//-----------------------------------------------------
bool CVATRegister::cancelCompany ( const string & taxID )
{
    Company temp ( taxID );
    auto posPerTaxId = findCompanyPerTaxId( temp );
    if (  posPerTaxId != dbPerTaxId . end () && ( * posPerTaxId ).isSameCompany( taxID ) )
    {
        Company tempPerNameAddr ( posPerTaxId->getLoweredName(), posPerTaxId->getLoweredAddr() );
        auto posPerNameAddr = findCompanyPerNameAddr ( tempPerNameAddr );
        dbPerNameAddr.erase(posPerNameAddr);
        dbPerTaxId.erase(posPerTaxId);

        return true;
    }
    return false;
}
//-----------------------------------------------------
bool CVATRegister::invoice( const string & taxID, unsigned int amount )
{
    Company temp ( taxID );
    auto posPerTaxId = findCompanyPerTaxId ( temp );
    if (  posPerTaxId != dbPerTaxId . end () && ( * posPerTaxId ).isSameCompany( taxID ) )
    {
        Company tempPerNameAddr ( posPerTaxId->getLoweredName(), posPerTaxId->getLoweredAddr() );
        auto posPerNameAddr = findCompanyPerNameAddr ( tempPerNameAddr );
        ( * posPerNameAddr ) . addInvoice( amount );
        ( * posPerTaxId ) . addInvoice( amount );
        invoiceCount += 1;
        invoices.insert( lower_bound( invoices.begin(),
                                              invoices.end(),
                                              amount ),
                         amount );
        return true;
    }
    return false;
}
//-----------------------------------------------------
bool CVATRegister::invoice ( const string & name, const string & addr, unsigned int amount )
{
    Company temp ( name, addr );
    auto posPerNameAddr = findCompanyPerNameAddr ( temp );
    if (  posPerNameAddr != dbPerNameAddr . end () && ( * posPerNameAddr ).isSameCompany( name, addr ) )
    {
        Company tempPerTaxId ( posPerNameAddr->getTaxId() );
        auto posPerTaxId = findCompanyPerTaxId( tempPerTaxId );
        ( * posPerTaxId ) . addInvoice( amount );
        ( * posPerNameAddr ) . addInvoice( amount );
        invoiceCount += 1;
        invoices.insert(lower_bound(invoices.begin(),
                                    invoices.end(),
                                    amount),
                        amount);
        return true;
    }
    return false;
}
//-----------------------------------------------------
unsigned int CVATRegister::medianInvoice ( void ) const
{
    return invoiceCount == 0 ? 0 : invoices.at(invoiceCount / 2);
}
//-----------------------------------------------------
bool CVATRegister::audit ( const string & name, const string & addr, unsigned int & sumIncome ) const
{
    Company temp ( name, addr );
    auto posPerNameAddr = findCompanyPerNameAddr ( temp );
    if (  posPerNameAddr != dbPerNameAddr . end () && ( * posPerNameAddr ).isSameCompany( name, addr ))
    {
        sumIncome = ( (* posPerNameAddr ) ) . getInvoice();
        return true;
    }
    return false;
}
//-----------------------------------------------------
bool CVATRegister::audit ( const string & taxID, unsigned int & sumIncome ) const
{
    Company temp ( taxID );
    auto posPerTaxId = findCompanyPerTaxId( temp );
    if (  posPerTaxId != dbPerTaxId . end () && ( * posPerTaxId ).isSameCompany( taxID ))
    {
        sumIncome = ( (* posPerTaxId ) ) . getInvoice();
        return true;
    }
    return false;
}
//-----------------------------------------------------
bool CVATRegister::firstCompany ( string & name, string & addr ) const
{
    if ( dbPerNameAddr . empty() )
        return false;
    name = this -> dbPerNameAddr .at( 0 ) . getName();
    addr = this -> dbPerNameAddr .at( 0 ) . getAddr();
    return true;
}
//-----------------------------------------------------
bool CVATRegister::nextCompany ( string & name, string & addr ) const
{
    if ( dbPerNameAddr . empty() )
        return false;
    Company temp ( name, addr );
    auto pos = findCompanyPerNameAddr( temp );
    if (  pos != dbPerNameAddr . end () && ( * pos ).isSameCompany( name, addr ))
    {
        if ( ++ pos == dbPerNameAddr . end())
            return false;
        name =  ( * pos ) . getName();
        addr =  ( * pos ) . getAddr();
        return true;
    }
    return false;
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
