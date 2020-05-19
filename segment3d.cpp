//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny, Liam O' Brien
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    3rd December 2001
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG (based on segment.cpp)
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "segment3d.hh"

ostream& operator<<( ostream& os, const CSegment3d& seg )
{
  os << seg.start() << " == " << seg.end();
  return os;
}

boolean CSegment3d::operator==(const CSegment3d& seg )
{
  if ( seg.start() == p && seg.end() == q ) 
    return true;
  else if ( seg.start() == q && seg.end() == p ) 
    return true;
  else
    return false;
}

boolean CSegment3d::operator!=(const CSegment3d& seg )
{
  if ( seg.start() != p || seg.end() != q ) 
    return true;
  else if ( seg.start() != q || seg.end() != p ) 
    return true;
  else 
   return false;
}

double CSegment3d::length() const
{
  return sqrt(( p - q ) * ( p - q ));
}

double CSegment3d::cosAngle( const CSegment3d& t ) const
{
  CPoint3d r, s; 

  r = unitVector();
  s = t.unitVector();

  return r * s;
}

double CSegment3d::angle( const CSegment3d& t ) const
{
  double x;
  x = acos( cosAngle( t ) );
  if ( x >= 0.0 )
    return x;
  else
    return Pi - x;
}

double CSegment3d::cosAngle( const CPoint3d& p1 ) const
{
  CPoint3d r, s; 

  r = unitVector();
  s = p1.unitVector();

  return r * s;
}

double CSegment3d::angle( const CPoint3d& p1 ) const
{
  return acos( cosAngle( p1 ) );
}

boolean CSegment3d::intersection( const CSegment3d& seg, CSegment3d& seg2 )
{
  int      j;
  CPoint3d p1[4];
  CPoint3d v1, v2, distanceV;
  double   d1, d2;

  if ( coplanar( seg ) )
  {
    j = 0;
    if ( intersection( seg.p ) )
    {
      p1[j] = seg.p;
      j++;
    }
    if ( intersection( seg.q ) )
    {
      p1[j] = seg.q;
      j++;
    }
    if ( seg.intersection( p ) == true )
    {
      p1[j] = p;
      j++;
    }
    if ( seg.intersection( q ) == true )
    {
      p1[j] = q;
      j++;
    }

    // two points inside is the easiest case
    if ( j == 2 )
    {
      seg2 = CSegment3d( p1[0], p1[1] );
      return true;
    }
    // lines are identical
    if ( j == 4 )
    {
      seg2 = seg;
      return true;
    }
    // one point in common so determine which one
    if ( j == 3 )
    {
      if ( p1[0] == p1[1] )
        seg2 = CSegment3d( p1[0], p1[2] );
      else if( p1[1] == p1[2] )
        seg2 = CSegment3d( p1[0], p1[1] );
      else if( p1[0] == p1[2] )
        seg2 = CSegment3d( p1[1], p1[2] );
      return true;
    }
  }
  else
  {
    // extra code added 06/12/01
    v2 = unitVector();
    v1 = seg.unitVector();
    distanceV = ( seg.p - p ) ^ v2;
    d1 = distanceV.abs();
    distanceV = ( seg.q - p ) ^ v2;
    d2 = distanceV.abs();
    p1[0] = seg.p * ( d2 / ( d1+d2) ) + seg.q * ( d1 / ( d1+d2) );

    if ( intersection( p1[0] ) )
    {
      seg2 = CSegment3d( p1[0], p1[0] );
      return true;
    }
  }

  return false;
}

boolean CSegment3d::intersection( const CPoint3d& p2 ) const
{
  CPoint3d p1, q1;

  if ( p2 == p || p2 == q ) return true;

  if ( p2.x > p.x - TOL && p2.x < q.x + TOL 
    && p2.y > p.y - TOL && p2.y < q.y + TOL 
    && p2.z > p.z - TOL && p2.z < q.z + TOL )
    return true;
  else if ( p2.x < p.x + TOL && p2.x > q.x - TOL 
         && p2.y < p.y + TOL && p2.y > q.y - TOL 
         && p2.z < p.z + TOL && p2.z > q.z - TOL )
    return true;
  else 
    return false;
}

boolean CSegment3d::coplanar( const CSegment3d& seg ) const
{
  CPoint3d v1, v2;

  v1 = unitVector();
  v2 = seg.unitVector();

  if ( v1 == v2 || v1 == v2 * -1.0 )
    return true;
  else
    return false;
}

CPoint3d CSegment3d::unitVector() const
{
  CPoint3d v;

  v = q - p;
  v = v / v.abs();

  return v;
}

CSegment3d& CSegment3d::operator=( const CSegment3d& t )
{
  p = t.p;
  q = t.q;
  return *this;
}

boolean CSegment3d::combinable( const CSegment3d& t )
{
  if ( coplanar( t ) )
  {
    if ( intersection( t.p ) )
      return true;
    else if ( intersection( t.q ) )
      return true;
    else if ( t.intersection( p ) )
      return true;
    else if ( t.intersection( q ) )
      return true;
  }

  return false;
}

boolean CSegment3d::attachTo( const CSegment3d& t, CSegment3d& newSeg )
{
  int      i;
  CPoint3d p1[4];

  // if not combinable return false
  if ( combinable( t ) == false )
    return false;

  // if both are equal we don't want to make another one
  if ( *this == t )
    return false;

  i = 0;
  if ( intersection( t.p ) == false )
  {
    p1[i] = t.p;
    i++;
  }
  if ( intersection( t.q ) == false )
  {
    p1[i] = t.q;
    i++;
  }
  if ( t.intersection( p ) == false )
  {
    p1[i] = p;
    i++;
  }
  if ( t.intersection( q ) == false )
  {
    p1[i] = q;
    i++;
  }
  
  // unusual case: one segment is contained in the other but one endpoint
  // is common to both segments
  if ( i == 1 )
  {
    if ( p.abs(p1[0]) > q.abs(p1[0]) )
      newSeg = CSegment3d( p, p1[0] );
    else
      newSeg = CSegment3d( q, p1[0] );
    return true;
  }

  // this says that line segments overlap with one another
  if ( i == 2 ) 
  {
    newSeg = CSegment3d( p1[0], p1[1] );
    return true;
  }

  // coplanar but not joined (no overlap)
  if ( i > 2 ) 
    return false;

  return false;
}

void CSegment3d::setStartPoint( const CPoint3d& endPoint )
{
  p = endPoint;
}

void CSegment3d::setEndPoint( const CPoint3d& endPoint )
{
  q = endPoint;
}

CSegment3d CSegment3d::reverse() const
{
  return CSegment3d( q, p );
}
