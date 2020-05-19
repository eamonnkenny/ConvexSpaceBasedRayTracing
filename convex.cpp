//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Project Manager:       Dr. Peter J. Cullen
// Last Modification:     October 2001
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "convex.hh"

CConvex::CConvex( ifstream& filePtr )
{
  CPoint3d v[8];
  int      i;
  double   x, y, z;
  string  dielectricString;

  for ( i = 0; i < 8; i++ )
  {
    filePtr >> x;
    filePtr >> y;
    filePtr >> z;
    v[i] = CPoint3d(x,y,z);
    cout << "Constructor CConvex() ( " << i << ", " << v[i] << " )" << endl;
  }
  if ( facet[0].numberOfAdjacentConvexs_ > 0 &&
       facet[0].numberOfAdjacentConvexs_ < 400 )
    cout << "still memory leakage in CConvex()" << endl;

  facet[0] = CBoundary( v[0], v[3], v[2], v[1] );
  facet[1] = CBoundary( v[0], v[1], v[5], v[4] );
  facet[2] = CBoundary( v[1], v[2], v[6], v[5] );
  facet[3] = CBoundary( v[0], v[4], v[7], v[3] );
  facet[4] = CBoundary( v[3], v[7], v[6], v[2] );
  facet[5] = CBoundary( v[4], v[5], v[6], v[7] );

  if (v[6].x - v[0].x < 0.0 || v[6].y - v[0].y < 0.0 || v[6].z - v[0].z < 0.0 )
  {
    // Added 27/11/01
    cerr << "Convex is invalid shape" << endl;
    cerr << facet[1] << endl;
    cerr << facet[5] << endl;
    exit(1);
  }
  
  filePtr >> dielectricString;
  if ( dielectricString != "DielectricParameters\0" )
  {
    cerr << "dielectric should be: " << dielectricString << endl;
    exit(1);
  }

  // constitutive parameters Added 21/11/01
  filePtr >> permittivity_;
  if ( permittivity_ < 1.0 || permittivity_ > 12.0 )
  {
    cerr << "permittivity_ was be: " << permittivity_ << endl;
    exit(1);
  }

  filePtr >> permeability_;
  filePtr >> conductivity_;
} 

boolean CConvex::intersection( const CPoint3d& pt )
{
  CPoint3d minpt, maxpt;

  minpt = facet[0].vertex_[0];
  maxpt = facet[5].vertex_[2];

  if ( minpt.x - TOL <= pt.x && pt.x <= maxpt.x + TOL )
    if ( minpt.y - TOL <= pt.y && pt.y <= maxpt.y + TOL )
      if ( minpt.z - TOL <= pt.z && pt.z <= maxpt.z + TOL )
        return true;

  return false;
}

boolean CConvex::intersection( const CPoint3d& p1, const CPoint3d& p2,
                               CPoint3d& p, int& boundaryNumber )
{
  boolean boundaryIntersection;

  for ( boundaryNumber = 0; boundaryNumber < 6; boundaryNumber++ )
  {
    boundaryIntersection = facet[boundaryNumber].intersection( p1, p2, p );
    if ( boundaryIntersection == true )
      return true;
  }
  return false;
}

boolean CConvex::notIntersection( const CPoint3d& p1, const CPoint3d& p2, 
                                  CPoint3d& p, int& boundaryNumber )
{
  boolean boundaryIntersection;

  for ( boundaryNumber = 0; boundaryNumber < 6; boundaryNumber++ )
  {
    boundaryIntersection = facet[boundaryNumber].intersection( p1, p2, p );

    if ( boundaryIntersection == true 
      && facet[boundaryNumber].intersection(p1) == false )
        return true;
  }

  return false;
}

int CConvex::boundaryWithLargestSurfaceArea()
{
  double area0, area1, area2;

  area0 = facet[0].area();
  area1 = facet[1].area();
  area2 = facet[2].area();
  if ( area0 >= area1 && area0 >= area2 ) return 0;
  if ( area1 >= area0 && area1 >= area2 ) return 1;
  if ( area2 >= area0 && area2 >= area1 ) return 2;
  return 0;
}

boolean CConvex::boundaryWithLargestSurfaceArea( int i )
{
  double area0, area1, area2, largest;

  area0 = facet[0].area();
  area1 = facet[1].area();
  area2 = facet[2].area();
  if ( area0 >= area1 && area0 >= area2 ) largest = area0;
  if ( area1 >= area0 && area1 >= area2 ) largest = area1;
  if ( area2 >= area0 && area2 >= area1 ) largest = area2;
  if ( ( i == 0 || i == 5 ) && largest == area0 ) return true;
  if ( ( i == 1 || i == 4 ) && largest == area1 ) return true;
  if ( ( i == 2 || i == 3 ) && largest == area2 ) return true;
  return false;
}

complex CConvex::complexPermittivity( const CBaseTransceiver& btsObject ) const
{
  return complex( permittivity_, -conductivity_ / ( Permittivity 
                                 * TwoPi * btsObject.frequency() ) );
}

ostream& operator<<( ostream& s, const CConvex& c )
{
  return s << c.facet[0] << endl << c.facet[5] << endl;
}

CConvex& CConvex::operator=( const CConvex& otherConvex )
{
  int i;

  if ( this != &otherConvex )
  {
    clear();

    for ( i = 0; i < 6; i++ )
      facet[i] = otherConvex.facet[i];
  
    permittivity_ = otherConvex.permittivity_;
    permeability_ = otherConvex.permeability_;
    conductivity_ = otherConvex.conductivity_;
  }

  return *this;
}


