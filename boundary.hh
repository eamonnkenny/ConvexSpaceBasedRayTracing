//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Project Manager:       Dr. Peter J. Cullen
// First Modification:    October 2001
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef CONVEX_BOUNDARY_HH
#define CONVEX_BOUNDARY_HH

#include <iostream>
#include "point3d.hh"
#include "segment3d.hh"
#include "plane.hh"
#include "userconst.hh"

//class CConvex;

class CBoundary
//==========================================================================
// = FUNCTION 
// 	boundary.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      The definition of one of the six boundaries for a convex space.
//        - Contains four vertices.
//        - an outward normal.
//        - connections to other convex spaces.
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2 IRELAND
//     All rights reserved 
//==========================================================================
{
  public:
      CPoint3d vertex_[4];
      CPlane   equation_;
      int      *adjacentConvexIndex_;
      int      numberOfAdjacentConvexs_;

      CBoundary() { numberOfAdjacentConvexs_ = 0; }
      //
      // default constructor
      //

      CBoundary( const CPoint3d& p1, const CPoint3d& p2,
                 const CPoint3d& p3, const CPoint3d& p4 )
      {
        vertex_[0] = p1;
        vertex_[1] = p2;
        vertex_[2] = p3;
        vertex_[3] = p4;
	makeOutwardNormal();
	numberOfAdjacentConvexs_ = 0;
      }

      CBoundary( CPoint3d& p1, CPoint3d& p2, CPoint3d& p3, CPoint3d& p4 )
      {
        vertex_[0] = p1;
        vertex_[1] = p2;
        vertex_[2] = p3;
        vertex_[3] = p4;
	makeOutwardNormal();
	numberOfAdjacentConvexs_ = 0;
      }
      //
      // constructors
      //

      double   distance( const CPoint3d& aPoint );
      double   shortestDistance( const CPoint3d& aPoint );
      CPoint3d image( const CPoint3d& aPoint );
      boolean  validImage( const CPoint3d& aPoint, CPoint3d& pr );
      //
      // distance from the origin and reflection in the boundary
      //

      void     makeOutwardNormal();
      //
      // create the outward normal to the plane
      //

      CPoint3d  centre();
      //
      // define the centre of the boundary
      //

      boolean   intersection( const CBoundary& boundary2 );
      boolean   intersection( const CPoint3d&  p );
      boolean   intersection( const CPoint3d& p1, const CPoint3d& p2,
                              CPoint3d& p );
      boolean planeIntersection( const CPoint3d& p1, const CPoint3d& p2,
                                 CPoint3d& p );
      boolean   intersection( const CSegment3d& seg, CSegment3d& seg2 );
      //
      // calculate the intersection of a boundary with another boundary,
      // line or point. Note that boundary intersection requires call to point
      // intersection.
      //

      boolean oppositeNormal( const CBoundary& boundary2 );
      //
      // check to see if the boundary 2 has outward normal in the opposite
      // direction.
      //

      double area();
      //
      // calculate the surface area of the boundary
      // Added 16/11/01
      //

      double angle( const CSegment3d& seg);
      double cosAngle( const CSegment3d& seg);
      //
      // Find the angle made by a vector passing through the current
      // boundary and the outward normal on the boundary
      // Added 21/12/01
      //

      friend ostream& operator<<( ostream& s, const CBoundary& b );
      //
      // prints the 4 vertices of a boundary in order to a file stream
      // Added 19/11/01
      //

      CBoundary& operator=( const CBoundary& otherBoundary );
      //
      // copy operator overload
      //

      void clear()
      {
        if ( numberOfAdjacentConvexs_ > 0 
          && numberOfAdjacentConvexs_ < MAX_CONVEX )
          delete [] adjacentConvexIndex_;
            numberOfAdjacentConvexs_ = 0;
        // default destructor
      }

      ~CBoundary()
      {
        clear();
        // default destructor
      }
};

#endif
