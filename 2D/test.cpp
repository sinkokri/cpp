#ifndef __PROGTEST__
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <set>
#include <map>
#include <list>
#include <vector>
#include <memory>
#include <cstdarg>
using namespace std;
struct CCoord
{
  public:
                             CCoord                        ( int               x = 0,
                                                             int               y = 0 )
      : m_X ( x ),
        m_Y ( y )
    {
    }
    int                      m_X;
    int                      m_Y;
};
#endif /* __PROGTEST__ */
struct coordComparator
{
    bool operator () ( const CCoord & left, const CCoord & right ) const
    {
        return ( left . m_X + left . m_Y ) < ( right . m_X + right . m_Y );
    }
};

//==============================================================================================================
class CShape
{
public:
    CShape( int id ): ID ( id ){};
private:
    int ID;
};
//==============================================================================================================
class CCircle: public CShape
{
public:
    CCircle(int id, int x, int y, int r): CShape( id ), center( x, y ), radius ( r ) { }
private:
    CCoord center;
    int radius;
};
//==============================================================================================================
class CPolygon: public CShape
{
public:
    template<typename  T >
    void add ( T & c )
    {
        vertecesSet . insert(static_cast<CCoord>(c) );
    }

    template<typename...  U, typename = std::enable_if_t<std::conjunction_v<std::is_same<U, CCoord>...>> >
    explicit CPolygon(int id, U... vals): CShape( id )
    {
//        ( add (vals), ...);
    std::initializer_list<CCoord> list {std::forward<U>(vals)...};
    std::copy(list.begin(),
              list.end(),
              std::inserter(vertecesSet, vertecesSet.begin()));
    }
    CPolygon( int id,  const initializer_list<CCoord> & list ): CShape( id )
    {
        std::copy(list.begin(),
                  list.end(),
                  std::inserter(vertecesSet, vertecesSet.begin()));
    }
    CPolygon( int id, const initializer_list<CCoord>::iterator& begin,  const initializer_list<CCoord>::iterator& end ): CShape( id )
    {
        std::copy(begin,
                  end,
                  std::inserter(vertecesSet, vertecesSet.begin()));
    }
    CPolygon( int id,  const list<CCoord>::iterator &begin,  const list<CCoord>::iterator &end ): CShape( id )
    {
        std::copy(begin,
                  end,
                  std::inserter(vertecesSet, vertecesSet.begin()));
    }

protected:
    set<CCoord, coordComparator> vertecesSet;
    size_t vertecesNum = vertecesSet . size();
};
//==============================================================================================================
class CTriangle: public CPolygon
{
public:
    CTriangle ( int id,  CCoord & a,  CCoord & b,  CCoord & c ):
            CPolygon(id, a, b, c)
            { vertecesNum = 3; }
};
//==============================================================================================================
class CRectangle: public CPolygon
{
public:
    CRectangle ( int id, int a1, int b1, int a2, int b2 ):
            CPolygon(id, CCoord(a1, b1), CCoord(a1, b1 + b2), CCoord(a2, b2), CCoord(a2, b2-b1))
    { vertecesNum = 4; }
};
//==============================================================================================================

class CScreen
{
  public:
    void                     add                           ( ... );
    std::vector<int>         test                          ( int               x,
                                                             int               y ) const;
    void                     optimize                      ( void );
    // todo
};

//==============================================================================================================


#ifndef __PROGTEST__
int                          main                          ( void )
{
  CScreen s0;
  s0 . add ( CRectangle ( 1, 10, 20, 30, 40 ) );
  s0 . add ( CRectangle ( 2, 20, 10, 40, 30 ) );
  s0 . add ( CTriangle ( 3, CCoord ( 10, 20 ), CCoord ( 20, 10 ), CCoord ( 30, 30 ) ) );
  s0 . optimize();
  assert ( s0 . test ( 0, 0 ) == (vector<int>{ }) );
  assert ( s0 . test ( 21, 21 ) == (vector<int>{ 1, 2, 3 }) );
  assert ( s0 . test ( 16, 17 ) == (vector<int>{ 3 }) );
  assert ( s0 . test ( 30, 22 ) == (vector<int>{ 1, 2 }) );
  assert ( s0 . test ( 35, 25 ) == (vector<int>{ 2 }) );

  CScreen s1;
  s1 . add ( CCircle ( 1, 10, 10, 15 ) );
  s1 . add ( CCircle ( 2, 30, 10, 15 ) );
  s1 . add ( CCircle ( 3, 20, 20, 15 ) );
  s1 . optimize();
  assert ( s1 . test ( 0, 0 ) == (vector<int>{ 1 }) );
  assert ( s1 . test ( 15, 15 ) == (vector<int>{ 1, 3 }) );
  assert ( s1 . test ( 20, 11 ) == (vector<int>{ 1, 2, 3 }) );
  assert ( s1 . test ( 32, 8 ) == (vector<int>{ 2 }) );

  CScreen s2;
  CCoord vertex21[] = { CCoord ( 10, 0 ), CCoord ( 20, 20 ), CCoord ( 30, 20 ), CCoord ( 40, 0 ) };
  s2 . add ( CPolygon ( 1, vertex21, vertex21 + 4 ) );
  CCoord vertex22[] = { CCoord ( 20, 10 ), CCoord ( 10, 20 ), CCoord ( 25, 30 ), CCoord ( 40, 20 ), CCoord ( 30, 10 ) };
  s2 . add ( CPolygon ( 2, vertex22, vertex22 + 5 ) );
  s2 . optimize();
  assert ( s2 . test ( 25, 15 ) == (vector<int>{ 1, 2 }) );
  assert ( s2 . test ( 25, 25 ) == (vector<int>{ 2 }) );
  assert ( s2 . test ( 15, 3 ) == (vector<int>{ 1 }) );
  assert ( s2 . test ( 11, 10 ) == (vector<int>{ }) );

  CScreen s3;
  std::initializer_list<CCoord> vertex31 = { CCoord ( 10, 0 ), CCoord ( 20, 20 ), CCoord ( 30, 20 ), CCoord ( 40, 0 ) };
  s3 . add ( CPolygon ( 1, vertex31 . begin (), vertex31 . end () ) );
  std::list<CCoord> vertex32 = { CCoord ( 20, 10 ), CCoord ( 10, 20 ), CCoord ( 25, 30 ), CCoord ( 40, 20 ), CCoord ( 30, 10 ) };
  s3 . add ( CPolygon ( 2, vertex32 . begin (), vertex32 . end () ) );
  s3 . optimize();
  assert ( s3 . test ( 25, 15 ) == (vector<int>{ 1, 2 }) );
  assert ( s3 . test ( 25, 25 ) == (vector<int>{ 2 }) );
  assert ( s3 . test ( 15, 3 ) == (vector<int>{ 1 }) );
  assert ( s3 . test ( 11, 10 ) == (vector<int>{ }) );
  s3 . add ( CPolygon ( 3, std::initializer_list<CCoord> { CCoord ( 10, 0 ), CCoord ( 20, 20 ), CCoord ( 30, 20 ), CCoord ( 40, 0 ) } ) );
  s3 . add ( CPolygon ( 4, { CCoord ( 20, 10 ), CCoord ( 10, 20 ), CCoord ( 25, 30 ), CCoord ( 40, 20 ), CCoord ( 30, 10 ) } ) );
  s3 . add ( CPolygon ( 5, CCoord ( 20, 10 ), CCoord ( 10, 20 ), CCoord ( 25, 30 ), CCoord ( 40, 20 ), CCoord ( 30, 10 ) ) );
  s3 . optimize();
  assert ( s3 . test ( 25, 15 ) == (vector<int>{ 1, 2, 3, 4, 5 }) );
  assert ( s3 . test ( 25, 25 ) == (vector<int>{ 2, 4, 5 }) );
  assert ( s3 . test ( 15, 3 ) == (vector<int>{ 1, 3 }) );
  assert ( s3 . test ( 11, 10 ) == (vector<int>{ }) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
