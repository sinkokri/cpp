#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */
//===========================================================================================
class Base
{
public:
    Base                         ( void ) = default;
    Base                         ( string  type ) : m_Type ( move( type )) {};
    Base                         ( const Base & other ) = default;
    virtual          ~Base       ( void )  = default;

    virtual Base &   operator =  ( Base & other ) { if ( & other == this ) return * this;
                                                  m_Size = other . m_Size;
                                                  m_Type.swap( other . m_Type );
                                                  return ( * this ); }

    virtual size_t   getSize     ( void )               const { return m_Size; }
    virtual string   getType     ( void )               const { return m_Type; }

    virtual bool     operator == ( const Base & other ) const { return other.getType() == ( * this ) . getType(); }
    virtual bool     operator != ( const Base & other ) const { return other.getType() != ( * this ) . getType(); }

    virtual void     print       ( ostream & os )       const { os << getType(); }
    virtual Base *   clone       ( void )               const { return new Base ( * this ); }
    virtual void     clear       ( void )                     { delete this; }
    friend ostream & operator << ( ostream & os,
                                   const Base & x );
protected:
    size_t m_Size;
    string m_Type;
};
//===========================================================================================
ostream & operator << ( ostream & os, const Base & x)
{
    x . print ( os );
    return os;
}
//===========================================================================================
class CDataTypeInt: public Base
{
public:
    CDataTypeInt ( void ) : Base ( "int" ) { m_Size = 4; }
//-------------------------------------------------------------------------------------------
    virtual  ~CDataTypeInt ( void ) {};
//-------------------------------------------------------------------------------------------
    CDataTypeInt  ( const CDataTypeInt & other ) : Base ( other ) {};
//-------------------------------------------------------------------------------------------
    CDataTypeInt &  operator =  ( const CDataTypeInt & other )
    {
        if ( & other == this ) return * this;
        m_Size = other . m_Size;
        m_Type = other . m_Type;
        return ( * this );
    }
//-------------------------------------------------------------------------------------------
    virtual Base * clone ( void ) const { return new CDataTypeInt ( * this ); }
};
//===========================================================================================
class CDataTypeDouble: public Base
{
public:
    CDataTypeDouble ( void ) : Base ( "double" ) { m_Size = 8; }
//-------------------------------------------------------------------------------------------
    virtual  ~CDataTypeDouble( void ) {};
//-------------------------------------------------------------------------------------------
    CDataTypeDouble  ( const CDataTypeDouble & other ) : Base(other) {};
//-------------------------------------------------------------------------------------------
    CDataTypeDouble &  operator =  ( const CDataTypeDouble & other )
    {
        if ( & other == this ) return * this;
        m_Size = other . m_Size;
        m_Type = other . m_Type;
        return ( * this );
    }
//-------------------------------------------------------------------------------------------
    virtual Base * clone ( void ) const override { return new CDataTypeDouble ( * this ); }
};
//===========================================================================================
class CDataTypeEnum: public Base
{
public:
    CDataTypeEnum ( void ) : Base ( "enum" ) { m_Size = 4; }
//-------------------------------------------------------------------------------------------
   virtual  ~CDataTypeEnum( void ) = default;
//-------------------------------------------------------------------------------------------
    CDataTypeEnum &  operator =  ( const CDataTypeEnum & other )
    {
        if ( & other == this ) return * this;
        m_Size = other . m_Size;
        m_Type = other . m_Type;
        m_EnumFields = other . m_EnumFields ; // vector<string>
        return ( * this );
    }
//-------------------------------------------------------------------------------------------
    CDataTypeEnum ( CDataTypeEnum const &other ): Base(other)
    {
        m_EnumFields . clear();
        m_EnumFields = other . m_EnumFields;
    }
//-------------------------------------------------------------------------------------------
    virtual Base * clone ( void ) const override { return new CDataTypeEnum ( * this ); }
//-------------------------------------------------------------------------------------------
    bool operator == ( const Base & other ) const override
    {
        if ( other . getType() == ( * this ) . getType() )
        {
            auto * enumPtr = dynamic_cast <CDataTypeEnum * >( other . clone() );
            bool result = m_EnumFields == enumPtr -> m_EnumFields;
            enumPtr -> clear();
            return result;
        }
        return false;
    }
//-------------------------------------------------------------------------------------------
    bool operator != ( const Base & other ) const override
    {
        if ( other . getType() == ( * this ) . getType() )
        {
            auto * enumPtr = dynamic_cast <CDataTypeEnum * > ( other . clone() );
            bool result = m_EnumFields != enumPtr -> m_EnumFields;
            enumPtr -> clear();
            return result;
        }
        return true;
    }
//-------------------------------------------------------------------------------------------
    CDataTypeEnum & add( const string & field  )
    {
        auto fieldExists = lower_bound (m_EnumFields . begin(), m_EnumFields . end(), field );

        if ( fieldExists != m_EnumFields . end() and ( * fieldExists ) == field )
            throw invalid_argument ("Duplicate enum value: " + field );

        m_EnumFields . push_back(  field  );
        return ( * this );
    }
//-------------------------------------------------------------------------------------------
    void print( ostream & os ) const override
    {
        Base::print( os );
        os  << endl;
        if ( ! m_EnumFields . empty() )
        {
            os << "{ "  << endl;
            for ( auto i = m_EnumFields . begin(); i != m_EnumFields . end(); ++i )
            {
                if ( (*i) == m_EnumFields . back() )
                    os << " " << (*i) << endl;
                else
                    os << " " << (*i) << "," << endl;
            }

            os << "} ";
        }
        else os << "";
    }
//-------------------------------------------------------------------------------------------
protected:
    vector <string> m_EnumFields;
};
//===========================================================================================
class CDataTypeStruct: public Base
{
public:
    CDataTypeStruct ( void ): Base ( "struct" ) {  m_Size = 0; }
//-------------------------------------------------------------------------------------------
    CDataTypeStruct &  operator =  ( const CDataTypeStruct  & other )
    {
        if ( & other == this ) return * this;
        m_StructFields . clear();
        m_Size = other . m_Size;
        // deep copy of vector
        vector<pair<string, Base * >> newVector;
        for ( size_t i = 0; i < other.m_StructFields.size(); i++ )
        {
            auto newPtr = other . m_StructFields . at(i ) . second -> clone();
            newVector . emplace_back( other . m_StructFields . at( i ) . first, newPtr );
        }
        m_StructFields = newVector;
        m_Type = other . getType();
        return ( * this );
    }
//-------------------------------------------------------------------------------------------
    CDataTypeStruct ( const CDataTypeStruct & other ): Base(other)
    {
        m_StructFields . clear();
        // deep copy of vector
        vector<pair<string, Base * >> newVector;
        for ( size_t i = 0; i < other.m_StructFields.size(); i++ )
        {
            auto newPtr = other . m_StructFields . at(i) . second -> clone();
            newVector . emplace_back( other . m_StructFields .at( i ) . first, move( newPtr ) );
        }
        m_StructFields = newVector;
    }
//-------------------------------------------------------------------------------------------
    virtual ~CDataTypeStruct  ( void )
    {
        for ( size_t i = 0; i < m_StructFields . size() ; i++ )
        {
            m_StructFields [i] . second -> clear(); // Base method
            m_StructFields [i] . first . clear(); // string clear
        }
        m_StructFields . clear();
    };
//-------------------------------------------------------------------------------------------
    virtual Base * clone ( void ) const override { return new CDataTypeStruct ( * this ); }
//-------------------------------------------------------------------------------------------
    size_t getSize( void ) const override { return m_Size; }
//-------------------------------------------------------------------------------------------
    bool operator == ( const Base & other ) const override
    {
        if ( other . getType() == ( * this ) . getType() )
        {
            auto * strPtr = dynamic_cast<CDataTypeStruct*>( other . clone() );
            if ( m_StructFields . size() == strPtr -> m_StructFields . size() )
            {
                for ( size_t i = 0; i < m_StructFields . size(); i ++ )
                {
                    if ( ( * m_StructFields . at ( i ) . second) != ( * strPtr -> m_StructFields . at( i ) . second) )
                    {
                        strPtr -> clear();
                        return false;
                    }
                }
                strPtr -> clear();
                return true;
            }
            strPtr -> clear();
            return false;
        }
        return false;
    }
//-------------------------------------------------------------------------------------------
    bool operator != ( const Base & other ) const override
    {
        if ( other . getType() == ( *this ) . getType() )
        {
            auto * strPtr = dynamic_cast<CDataTypeStruct * >( other . clone() );
            if ( m_StructFields . size() == strPtr -> m_StructFields . size() )
            {
                for ( size_t i = 0; i < m_StructFields . size(); i ++ )
                {
                    if ( ( * m_StructFields . at( i ) . second) != ( * strPtr -> m_StructFields . at( i ) . second ) )
                    {
                        strPtr -> clear();
                        return true;
                    }
                }
                strPtr -> clear();
                return false;
            }
            strPtr -> clear();
            return true;
        }
        return true;
    }
//-------------------------------------------------------------------------------------------
    CDataTypeStruct & addField ( const string & fieldName, const Base & fieldType )
    {
        for ( const auto & field: m_StructFields )
        {
            if ( field . first == fieldName )
                throw invalid_argument ( "Duplicate field: " + fieldName );
        }
        m_StructFields . emplace_back (  make_pair (  fieldName, fieldType . clone() ));
        m_Size += fieldType . getSize();
        return ( * this );
    }
//-------------------------------------------------------------------------------------------
    const Base & field ( const string & name ) const
    {
        for ( const auto & field: m_StructFields )
        {
            if ( field . first == name )
                return * field . second;
        }
        throw invalid_argument ("Unknown field: " + name );
    }
//-------------------------------------------------------------------------------------------
    void print( ostream & os ) const override
    {
        Base::print( os );
        os << endl << "{ "  << endl;
        if ( ! m_StructFields . empty() )
        {
            for ( auto & i: m_StructFields )
            {
                os << "  "  ;
                os << * i . second;
                os << " " << i . first << ";" << endl;
            }

            os << "} " << endl;
        }
        else os << "" << endl;
    }
protected:
    vector<pair<string, Base * >> m_StructFields;
};
//===========================================================================================
#ifndef __PROGTEST__
static bool        whitespaceMatch                         ( const string    & a,
                                                             const string    & b )
{
    string a1 = a;
    a1.erase(std::remove_if(a1.begin(), a1.end(), ::isspace), a1.end());
    string b1 = b;
    b1.erase(std::remove_if(b1.begin(), b1.end(), ::isspace), b1.end());

  return a1 == b1;
}
template <typename T_>
static bool        whitespaceMatch                         ( const T_        & x,
                                                             const string    & ref )
{
  ostringstream oss;
  oss << x;
  return whitespaceMatch ( oss . str (), ref );
}
int main ( void )
{
  CDataTypeStruct  a = CDataTypeStruct ().
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum ().
                          add ( "NEW" ) .
                          add ( "FIXED" ) .
                          add ( "BROKEN" ) .
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
    cout << a;
  CDataTypeStruct b = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () .
                          add ( "NEW" ) .
                          add ( "FIXED" ) .
                          add ( "BROKEN" ) .
                          add ( "READY" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
    cout << b;
  CDataTypeStruct c =  CDataTypeStruct () .
                        addField ( "m_First", CDataTypeInt () ) .
                        addField ( "m_Second", CDataTypeEnum () .
                          add ( "NEW" ) .
                          add ( "FIXED" ) .
                          add ( "BROKEN" ) .
                          add ( "DEAD" ) ).
                        addField ( "m_Third", CDataTypeDouble () );
    cout << c;
  CDataTypeStruct  d = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () .
                          add ( "NEW" ) .
                          add ( "FIXED" ) .
                          add ( "BROKEN" ) .
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeInt () );
    cout << d;
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( c, "struct\n"
    "{\n"
    "  int m_First;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Second;\n"
    "  double m_Third;\n"
    "}") );

  assert ( whitespaceMatch ( d, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  int m_Ratio;\n"
    "}") );

  assert ( a != b );
  assert ( a == c );
  assert ( a != d );
//  assert ( a . field ( "m_Status" ) == CDataTypeEnum () . add ( "NEW" ) . add ( "FIXED" ) . add ( "BROKEN" ) . add ( "DEAD" ) );
//  assert ( a . field ( "m_Status" ) != CDataTypeEnum () . add ( "NEW" ) . add ( "BROKEN" ) . add ( "FIXED" ) . add ( "DEAD" ) );
//  assert ( a != CDataTypeInt() );
//  assert ( whitespaceMatch ( a . field ( "m_Status" ), "enum\n"
//    "{\n"
//    "  NEW,\n"
//    "  FIXED,\n"
//    "  BROKEN,\n"
//    "  DEAD\n"
//    "}") );
//
//  CDataTypeStruct aOld = a;
//  b . addField ( "m_Other", CDataTypeDouble ());
//
//  a . addField ( "m_Sum", CDataTypeInt ());
//
//  assert ( a != aOld );
//  assert ( a != c );
//  assert ( aOld == c );
//  assert ( whitespaceMatch ( a, "struct\n"
//    "{\n"
//    "  int m_Length;\n"
//    "  enum\n"
//    "  {\n"
//    "    NEW,\n"
//    "    FIXED,\n"
//    "    BROKEN,\n"
//    "    DEAD\n"
//    "  } m_Status;\n"
//    "  double m_Ratio;\n"
//    "  int m_Sum;\n"
//    "}") );
//
//  assert ( whitespaceMatch ( b, "struct\n"
//    "{\n"
//    "  int m_Length;\n"
//    "  enum\n"
//    "  {\n"
//    "    NEW,\n"
//    "    FIXED,\n"
//    "    BROKEN,\n"
//    "    READY\n"
//    "  } m_Status;\n"
//    "  double m_Ratio;\n"
//    "  double m_Other;\n"
//    "}") );
//
//  c . addField ( "m_Another", a . field ( "m_Status" ));
//
//  assert ( whitespaceMatch ( c, "struct\n"
//    "{\n"
//    "  int m_First;\n"
//    "  enum\n"
//    "  {\n"
//    "    NEW,\n"
//    "    FIXED,\n"
//    "    BROKEN,\n"
//    "    DEAD\n"
//    "  } m_Second;\n"
//    "  double m_Third;\n"
//    "  enum\n"
//    "  {\n"
//    "    NEW,\n"
//    "    FIXED,\n"
//    "    BROKEN,\n"
//    "    DEAD\n"
//    "  } m_Another;\n"
//    "}") );
//
//  d . addField ( "m_Another", a . field ( "m_Ratio" ));
//
//  assert ( whitespaceMatch ( d, "struct\n"
//    "{\n"
//    "  int m_Length;\n"
//    "  enum\n"
//    "  {\n"
//    "    NEW,\n"
//    "    FIXED,\n"
//    "    BROKEN,\n"
//    "    DEAD\n"
//    "  } m_Status;\n"
//    "  int m_Ratio;\n"
//    "  double m_Another;\n"
//    "}") );
//
//  assert ( a . getSize () == 20 );
//  assert ( b . getSize () == 24 );
//  try
//  {
//    a . addField ( "m_Status", CDataTypeInt () );
//    assert ( "addField: missing exception!" == nullptr );
//  }
//  catch ( const invalid_argument & e )
//  {
//    assert ( e . what () == "Duplicate field: m_Status"sv );
//  }
//
//  try
//  {
//    cout << a . field ( "m_Fail" ) << endl;
//    assert ( "field: missing exception!" == nullptr );
//  }
//  catch ( const invalid_argument & e )
//  {
//    assert ( e . what () == "Unknown field: m_Fail"sv );
//  }
//
  try
  {
    CDataTypeEnum en;
    en . add ( "FIRST" ) .
         add ( "SECOND" ) .
         add ( "FIRST" );
    assert ( "add: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Duplicate enum value: FIRST"sv );
  }

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
