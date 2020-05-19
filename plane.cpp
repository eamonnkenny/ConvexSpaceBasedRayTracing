//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    November 26th 2001
// Last  Modification:    Dated (dd/mm/yy)
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "plane.hh"

CPlane::CPlane( const CPoint3d& outwardNormal, double rhs_constant )
{
  outwardNormal_ = outwardNormal;
  D_             = rhs_constant;
}

CPlane::CPlane( const CPoint3d& point1, const CPoint3d& point2 )
{
  CPoint3d midPoint = ( point1 + point2 ) / 2.0;
  outwardNormal_ = point1 - point2;
  outwardNormal_ = outwardNormal_ / outwardNormal_.abs();
  D_             = outwardNormal_ * midPoint;
}

double CPlane::sign( const CPoint3d& p )
{
  double signP;

  signP = outwardNormal_ * p - D_;
  return ( signP );
}

int CPlane::isign( const CPoint3d& p )
{
  double signP;

  signP = sign( p );

  if ( fabs( signP ) < TOL )
    return 0;
  else if ( signP >= TOL )
    return 1;
  else 
    return -1;
}

CPlane& CPlane::operator=( const CPlane& plane2 )
{
  outwardNormal_ = plane2.outwardNormal_;
  D_             = plane2.D_;

  return *this;
}

boolean CPlane::operator==( const CPlane& plane2 )
{
  if ( outwardNormal_ == plane2.outwardNormal_ && fabs( D_ - plane2.D_ ) < TOL )
    return true;
  else
    return false;
}

boolean CPlane::operator!=( const CPlane& plane2 )
{
  if ( outwardNormal_ != plane2.outwardNormal_ || fabs( D_ - plane2.D_ ) > TOL )
    return true;
  else
    return false;
}

ostream& operator<<( ostream& s, const CPlane& im )
{
  return s << "Equation of Plane: " << im.outwardNormal_.x << "x + "
                                    << im.outwardNormal_.y << "y + " 
				    << im.outwardNormal_.z << "z = " << im.D_;
}

double CPlane::distance( const CPoint3d& aPoint )
{
  return aPoint.abs();
}

CPoint3d CPlane::image( const CPoint3d& p )
{
  // reflect in the boundary to get the image point
  return ( p - outwardNormal_ * 2.0 * shortestDistance(p) );
}

boolean CPlane::validImage( const CPoint3d& p, CPoint3d& pr )
{
  double theDistance;

  theDistance = signedShortestDistance( p );

  if ( theDistance >= 0.0 )
  {
    // reflect in the boundary to get the image point
    pr = p - outwardNormal_ * 2.0 * theDistance;
    return true;
  }
  else
    return false;
}

boolean CPlane::validImage( const CSegment3d& aSegment, CSegment3d& newSegment )
{
  double d1, d2;

  d1 = signedShortestDistance( aSegment.start() );
  d2 = signedShortestDistance( aSegment.end() );

  if ( d1 >= 0.0 || d2 >= 0.0 )
  {
    // reflect in the boundary to get the image edge
    newSegment.setStartPoint( aSegment.start() - outwardNormal_ * 2.0 * d1 );
    newSegment.setEndPoint(   aSegment.end()   - outwardNormal_ * 2.0 * d2 );

    return true;
  }
  else
  {
    return false;
  }
}

double CPlane::signedShortestDistance( const CPoint3d& p )
{
  CPoint3d v;
  
  // use dot product to get shortest distance 
  // (Thomas Calculus, tenth edition, p. 814 )
  // v is some point on the plane
  if ( outwardNormal_.x != 0.0 )
    v = CPoint3d( D_ / outwardNormal_.x, 0.0, 0.0 );
  else if ( outwardNormal_.y != 0.0 )
    v = CPoint3d( 0.0, D_ /  outwardNormal_.y, 0.0 );
  else if ( outwardNormal_.z != 0.0 )
    v = CPoint3d( 0.0, 0.0, D_ /  outwardNormal_.z );

  return ( p - v ) * outwardNormal_;
}

double CPlane::shortestDistance( const CPoint3d& p )
{
  return fabs( signedShortestDistance( p ) );
}
