//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Project Manager:       Dr. Peter J. Cullen
// Last Modification:     October 2001
// Previous Projects:     none, new software
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "boundary.hh"

double CBoundary::distance( const CPoint3d& aPoint )
{
  return aPoint.abs();
}

CPoint3d CBoundary::image( const CPoint3d& p )
{
  // reflect in the boundary to get the image point
  return ( p - equation_.outwardNormal_ * 2.0 * shortestDistance(p) );
}

boolean CBoundary::validImage( const CPoint3d& p, CPoint3d& pr )
{
  double signedShortestDistance;

  signedShortestDistance = (( p - vertex_[0] ) * equation_.outwardNormal_);
  if ( signedShortestDistance >= 0.0 )
  {
    // reflect in the boundary to get the image point
    pr = p - equation_.outwardNormal_ * 2.0 * signedShortestDistance;
    return true;
  }
  else
  {
    return false;
  }
}

double CBoundary::shortestDistance( const CPoint3d& p )
{
  // use dot product to get shortest distance 
  // (Thomas Calculus, tenth edition, p. 814 )
  return fabs(( p - vertex_[0] ) * equation_.outwardNormal_);
}

void CBoundary::makeOutwardNormal()
{
  CPoint3d  normal;

  // create an outward normal to the surface plate by getting vector
  // which is 90 degrees to two sides of the boundary
  normal = ( vertex_[1] - vertex_[0] ) ^ ( vertex_[3]-vertex_[0] );
  equation_.outwardNormal_ = normal / normal.abs();
  if ( isnan(equation_.outwardNormal_.x) )
  {
    cerr << "-----------------------------------" << endl;
    cerr << vertex_[0] << " " << vertex_[1] << " | ";
    cerr << vertex_[2] << " " << vertex_[3] << endl;
    cerr << "equation of outward normal Problem " << endl;
    cerr << " Boundary: " << endl << *this << endl;
    exit(1);
  }
  equation_.D_ = equation_.outwardNormal_ * vertex_[0];
}

boolean CBoundary::oppositeNormal( const CBoundary& boundary2 )
{
  if ( equation_.outwardNormal_.x == -boundary2.equation_.outwardNormal_.x 
    && equation_.outwardNormal_.y ==  boundary2.equation_.outwardNormal_.y
    && equation_.outwardNormal_.z == boundary2.equation_.outwardNormal_.z 
    && fabs(equation_.outwardNormal_.x) > TOL )
  {
    return true;
  }
  else if ( equation_.outwardNormal_.x ==  boundary2.equation_.outwardNormal_.x
         && equation_.outwardNormal_.y == -boundary2.equation_.outwardNormal_.y
         && equation_.outwardNormal_.z ==  boundary2.equation_.outwardNormal_.z
         && fabs(equation_.outwardNormal_.y) > TOL )
  {
    return true;
  }
  else if ( equation_.outwardNormal_.x ==  boundary2.equation_.outwardNormal_.x
         && equation_.outwardNormal_.y ==  boundary2.equation_.outwardNormal_.y
         && equation_.outwardNormal_.z == -boundary2.equation_.outwardNormal_.z
         && fabs(equation_.outwardNormal_.z) > TOL )
  {
    return true;
  }

  return false;
}

boolean CBoundary::intersection( const CBoundary& boundary2 )
{
  boolean  inBoundary = false;
  double   distanceToPoint;
  CPoint3d p;
  int      i;

  for ( i = 0; i < 4; i++ )
  {
    p = boundary2.vertex_[i];
    distanceToPoint = shortestDistance( p );
    //if ( distanceToPoint < TOL )
    //{
      //inBoundary = intersection( boundary2.vertex_[i] );
      //break;
    //}
    if ( distanceToPoint < 0.0 )
    {
      cerr << "Problem in distance formula" << endl;
      exit(-1);
    }
    //cout << "intersection test: " << p << "==" << vertex_[0] << "=="
         //<< vertex_[2] << "==" << distanceToPoint << endl;
    if ( distanceToPoint < TOL 
      && p.x >= vertex_[0].x && p.x <= vertex_[2].x
      && p.y >= vertex_[0].y && p.y <= vertex_[2].y 
      && p.z >= vertex_[0].z && p.z <= vertex_[2].z )
    {
      return true;
    }
    //cout << "intersection test: " << p << "==" << vertex_[2] << "=="
         //<< vertex_[0] << "==" << distanceToPoint << endl;
    if ( distanceToPoint < TOL 
      && p.x <= vertex_[0].x && p.x >= vertex_[2].x
      && p.y <= vertex_[0].y && p.y >= vertex_[2].y 
      && p.z <= vertex_[0].z && p.z >= vertex_[2].z )
    {
      return true;
    }
  }

  return inBoundary;
}

boolean CBoundary::intersection( const CPoint3d& p )
{
  //double  distX, distY, distZ;

  // on XY-plane check if point p is included in XY bounded rectangle
  // if distance to point from plane is zero then check if point is 
  // inside plane boundaries
  if ( shortestDistance( p ) < TOL 
    && p.x >= vertex_[0].x-TOL && p.x <= vertex_[2].x+TOL
    && p.y >= vertex_[0].y-TOL && p.y <= vertex_[2].y+TOL
    && p.z >= vertex_[0].z-TOL && p.z <= vertex_[2].z+TOL )
  {
    return true;
  }

  return false;
}


boolean CBoundary::intersection( const CSegment3d& seg, CSegment3d& seg2 )
{
  int        i, j, k;
  CSegment3d newSegment;
  CPoint3d   p0[2];

  if ( intersection( seg.start() ) && intersection( seg.end() ) )
  {
    seg2 = seg;
    return true;
  }
  else
  {
    k = 0;
    for ( i = 0; i < 4; i++ )
    {
      j = i + 1;
      if ( j == 4 ) 
        j = 0;  // modulo arithmetic
 
      newSegment = CSegment3d( vertex_[i], vertex_[j] );
   
      // if we get a two points of intersection with boundary then we are on a 
      // coplanar surface. We return two points of contact.
      // otherwise we have to get one point of intersection with the 
      // lines that make up the boundary of the surface. We get all 
      // intersection points, and if there are two we can return the segment.
      if ( newSegment.intersection( seg, seg2 ) && seg2.start() != seg2.end() )
      {
        return true;
      }
      else if ( newSegment.intersection( seg, seg2 ) 
           && seg2.start() == seg2.end() )
      {
        p0[k] = seg2.start();
	k++;
      }

      // we now have two points on the boundary so we know we have 
      // found the correct line segment
      if ( k == 2 )
      {
	seg2 = CSegment3d( p0[0], p0[1] );
	return true;
      }
    }
  }

  // special case where one point intersects the boundary but the other 
  // can be inside and on the boundary
  if ( k == 1 )
  {
    if ( intersection( seg.start() ) )
    {
      seg2 = CSegment3d( seg.start(), p0[0] );
      return true;
    }
    else if ( intersection( seg.end() ) )
    {
      seg2 = CSegment3d( p0[0], seg.end() );
      return true;
    }
  }

  return false;
}

boolean CBoundary::intersection( const CPoint3d& p1, const CPoint3d& p2, 
                                 CPoint3d& p )
{
  boolean inPlane;
  // intersect boundary with line segment [p1,p2]
  // then check if the point is inside the boundary

  inPlane = planeIntersection( p1, p2, p );
  //if ( inPlane == true )
    //cout << " hit plane at " << p << endl;
  
  if ( inPlane == false )
    return false;

  if ( intersection( p ) )
    return true;
  else
    return false;
}

boolean CBoundary::planeIntersection( const CPoint3d& p1, const CPoint3d& p2,
                                      CPoint3d& p )
{
  double   a, b, t;

  // equation of line formula from Thomas Calculus, P.808-P.813, 10th Edition

  a = equation_.outwardNormal_ * p1;
  b = equation_.outwardNormal_ * p2 - a;
  if ( fabs(b) > TOL )
    t = ( equation_.D_ - a ) / b;
  else
    t = 0;
  p = p1 * (1.0-t) + p2 * t; // parameterized line equation

  //cout << "t = " << t << endl;

  if ( t <= 1.0+TOL && t >= -TOL )
    return true;
  else
    return false;
}

double CBoundary::area()
{
  return fabs( vertex_[2].abs( vertex_[1] ) * vertex_[1].abs( vertex_[0] ) );
}

ostream& operator<<( ostream& s, const CBoundary& b )
{
  return s << b.vertex_[0] << endl << b.vertex_[1] << endl << b.vertex_[2] 
           << endl << b.vertex_[3];
}

double CBoundary::angle( const CSegment3d& seg )
{
  double x;
  x = acos( cosAngle( seg ) );
  if ( x > 0.0 )
    return x;
  else
    return Pi - x;
}

double CBoundary::cosAngle( const CSegment3d& seg )
{
  return seg.cosAngle( equation_.outwardNormal_ );
}

CBoundary& CBoundary::operator=( const CBoundary& otherBoundary )
{
  int i;

  if ( this != &otherBoundary )
  {
    clear();

    for ( i = 0; i < 4; i++ )
      vertex_[i] = otherBoundary.vertex_[i];
  
    equation_                = otherBoundary.equation_;
    numberOfAdjacentConvexs_ = otherBoundary.numberOfAdjacentConvexs_;
  
    if ( numberOfAdjacentConvexs_ > 0 )   
    {
      adjacentConvexIndex_ = new int [numberOfAdjacentConvexs_];
      for ( i = 0; i < numberOfAdjacentConvexs_; i++ )
        adjacentConvexIndex_[i] = otherBoundary.adjacentConvexIndex_[i];
    } 
  }

  return *this;
}

CPoint3d CBoundary::centre()
{
  CPoint3d centre = CPoint3d();

  for ( int i = 0; i < 4; i++ )
    centre =  centre + vertex_[i];
  centre =  centre / 4.0;

  return centre;
}
