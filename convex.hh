//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    November 2001
// Last Modification:     Dated below "Added dd/mm/yy"
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef CONVEX_SPACES_HH
#define CONVEX_SPACES_HH

#include <iostream>
#include "complex.hh"
#include "bts.hh"
#include "boundary.hh"
#include <string>

class CConvex
//==========================================================================
// = FUNCTION 
//         convex.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      The definition of a complex space, which is bounded by facets.
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//==========================================================================
{
  public:
      CBoundary   facet[6];
      double      permittivity_;
      double      permeability_;
      double      conductivity_;
      // 
      // define a convex space to have 6 bounding facets 
      // and constitutive parameters Added 21/11/01
      //
      
      CConvex()
      {
        int i;
        for ( i = 0; i < 6; i++ )
          facet[i] = CBoundary();
        permittivity_ = 1.0;
        permeability_ = 1.0;
        conductivity_ = 0.0;
        // default constructor
      }

      boolean intersection( const CPoint3d& p );
      //
      // see if the point p is contained in the current convex space
      //

      boolean intersection( const CPoint3d& p1, const CPoint3d& p2,
                            CPoint3d& p, int& boundaryNumber );
      //
      // see if a line segment [p1,p2] intersects the convex at a point
      // if so then return the point of contact and the boundary number
      // at which the contact is made.
      // Added 16/11/01
      //

      boolean notIntersection( const CPoint3d& p1, const CPoint3d& p2,
                               CPoint3d& p, int& boundaryNumber );
      //
      // We know that there are two intersection points for any line through
      // a convex space, so in certain cases we want to say which one of these
      // intersection points on a boundary we definitely don't want.
      // Hence the index notThisBoundary.
      // Added 20/11/01
      //

      //CConvex( CPoint3d vertexSet[8] );
      //
      // given 8 points construct the facets of the convex
      //

      CConvex( ifstream& filePointer );
      //
      // convex space from a file constructor
      //

      int boundaryWithLargestSurfaceArea();
      boolean boundaryWithLargestSurfaceArea( int i );
      //
      // calculate the boundary of the convex space with the largest surface
      // area.
      //

      complex complexPermittivity( const CBaseTransceiver& btsObject ) const;
      //
      // returns the relative complex permittivity e' = e_2/e_1
      //

      friend ostream& operator<<( ostream& s, const CConvex& c );
      //
      // returns the bottom and top facet of the convex space to a stream
      //

      CConvex& operator=( const CConvex& otherConvex );
      //
      // copy/assignment operator overload
      //
       
      CPoint3d centre()
      {
	    int      i;
        CPoint3d centrepoint;
        for ( i = 0; i < 4; i++ )
	  centrepoint = centrepoint + facet[0].vertex_[i]
	                             + facet[5].vertex_[i];
        centrepoint = centrepoint / 8.0;
	return centrepoint;
      }

      void clear()
      {
        for ( int i=0; i<6; i++ )
          facet[i].clear();
      }

      ~CConvex()
      {
        clear();
      }
};

#endif
