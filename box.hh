//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    May 20th 2002
// Last Modification:     Dated below "Added dd/mm/yy"
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef BOUNDING_BOX_HH
#define BOUNDING_BOX_HH

#include <iostream>
#include "userconst.hh"
#include "boundary.hh"

class CBoundingBox
//==========================================================================
// = FUNCTION 
//     box.hh
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     Define a bounding box in which capacity requirement lie
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//==========================================================================
{
  public:
      double xSouthWest_, ySouthWest_, zSouthWest_;
      double xNorthEast_, yNorthEast_, zNorthEast_;
      double capacity_;
      // 
      // construction of a box
      //

      CBoundingBox()
      {
        xSouthWest_ = ySouthWest_ = zSouthWest_ = BIG_PLUS;
	xNorthEast_ = yNorthEast_ = zNorthEast_ = BIG_MINUS;
        // construction of a box (can not set to zeros since it will not be
	// as easy to check whether something is bounded above and below )
      }

      CBoundingBox( double xSouthWest, double ySouthWest, double zSouthWest,
                    double xNorthEast, double yNorthEast, double zNorthEast )
      {
        xSouthWest_ = xSouthWest;
	ySouthWest_ = ySouthWest;
	zSouthWest_ = zSouthWest;
	xNorthEast_ = xNorthEast;
	yNorthEast_ = yNorthEast;
	zNorthEast_ = zNorthEast;
      }
      
      CBoundingBox( ifstream& cinput )
      {
        cinput >> xSouthWest_;
        cinput >> ySouthWest_;
        cinput >> zSouthWest_;
        cinput >> xNorthEast_;
        cinput >> xNorthEast_;
        cinput >> xNorthEast_;
        // default constructor
      }

      void setCapacity( double capacity ) { capacity_ = capacity; }
      double getCapacity() { return capacity_; }
      //
      // get or set the capacity requirements for the box, this is used
      // to give a guide for the capacity of each server later on
      //

      void writeBoundingBox( ofstream& coutput )
      {
        coutput << xSouthWest_ << " ";
        coutput << ySouthWest_ << " ";
        coutput << zSouthWest_ << endl;
        coutput << xNorthEast_ << " ";
        coutput << yNorthEast_ << " ";
        coutput << zNorthEast_ << endl;
      }

      boolean intersection( double x, double y, double z ) const;
      boolean intersection( double x, double y ) const;
      boolean intersection( const CPoint3d& p ) const;
      //
      // intersect the point in a z-plane to see if it is inside the box
      //

      void updateBox( const CBoundary& facet );
      //
      // if a boundaries vertices are outside the box dimensions then use 
      // the exterior vertices to update the dimensions of the box.
      //

      CPoint3d centroid() const { return CPoint3d( xNorthEast_ + xSouthWest_,
                                                   yNorthEast_ + ySouthWest_, 
                                                   zNorthEast_ + zSouthWest_ )
						   / 2.0; }
      //
      // calculate the centroid of the box
      //

      double length() { return xNorthEast_ - xSouthWest_; }
      double height() { return yNorthEast_ - ySouthWest_; }
      CPoint3d southWest() const
      { return CPoint3d( xSouthWest_, ySouthWest_, zSouthWest_ ); }
      CPoint3d northEast() const
      { return CPoint3d( xNorthEast_, yNorthEast_, zNorthEast_ ); }
      //
      // return dimensions of the box
      //

      friend ostream& operator<<( ostream& os, const CBoundingBox& box );
      //
      // overload output stream
      //
};

#endif
