//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Project Manager:       Dr. Peter J. Cullen
// Last Modification:     20th May 2002
// Previous Projects:     none, new software
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include "box.hh"

void CBoundingBox::updateBox( const CBoundary& facet )
{
  int i;

  for ( i = 0; i < 4; i++ )
  {
    if ( facet.vertex_[i].x < xSouthWest_ )
      xSouthWest_ = facet.vertex_[i].x;
    if ( facet.vertex_[i].x > xNorthEast_ )
      xNorthEast_ = facet.vertex_[i].x;

    if ( facet.vertex_[i].y < ySouthWest_ )
      ySouthWest_ = facet.vertex_[i].y;
    if ( facet.vertex_[i].y > yNorthEast_ )
      yNorthEast_ = facet.vertex_[i].y;

    if ( facet.vertex_[i].z < zSouthWest_ )
      zSouthWest_ = facet.vertex_[i].z;
    if ( facet.vertex_[i].z > zNorthEast_ )
      zNorthEast_ = facet.vertex_[i].z;
  }
}

boolean CBoundingBox::intersection( const CPoint3d& p ) const
{
  return intersection( p.x, p.y, p.z );
}

boolean CBoundingBox::intersection( double x, double y, double z ) const
{
  if ( x >= xSouthWest_ && x <= xNorthEast_
    && y >= ySouthWest_ && y <= yNorthEast_
    && z >= zSouthWest_ && z <= zNorthEast_ )
    return true;
  return false;
}

boolean CBoundingBox::intersection( double x, double y ) const
{
  if ( x >= xSouthWest_ && x <= xNorthEast_
    && y >= ySouthWest_ && y <= yNorthEast_ )
    return true;
  return false;
}

ostream& operator<<( ostream& os, const CBoundingBox& box )
{
  os << CPoint3d( box.xSouthWest_, box.ySouthWest_, box.zSouthWest_ ) << endl;
  os << CPoint3d( box.xNorthEast_, box.yNorthEast_, box.zNorthEast_ );
  return os;
}
