//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    3rd December 2001
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG (based on segment.hh 2D)
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef SEGMENT_3D_HH
#define SEGMENT_3D_HH

#include "point3d.hh"

class CSegment3d 
//=========================================================================
// = FUNCTION 
//     segment3d.hh
//
// = AUTHOR
//     Eamonn M. Kenny, Liam O' Brien
//
// = DESCRIPTION
//     A line segment class with 2 end points and corresponding operators
//     in three dimensions. Based on an old 2D segment implementation
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//=========================================================================
{
  CPoint3d   p, q; // p = start, q = end

  public:
    CSegment3d( CPoint3d p1, CPoint3d q1 ) : p(p1), q(q1) { }
    CSegment3d(): p( CPoint3d( 0.0, 0.0, 0.0 ) ), 
                  q( CPoint3d( 0.0, 0.0, 0.0 ) ) { }
    CSegment3d( const CSegment3d& seg ) { p = seg.p; q = seg.q ; }
    //
    // construct for CSegment3d
    //
   
    CSegment3d& operator=( const CSegment3d& seg );
    //
    // copy of CSegment3d
    //

    boolean  operator==(const CSegment3d& seg );
    boolean  operator!=(const CSegment3d& seg );
    //
    // operators to evaluate if two segments are equal.
    //

    CPoint3d start() const {return p; }
    CPoint3d end()   const {return q; }
    //
    // extraction of first or last point of segment
    //

    void setStartPoint( const CPoint3d& startPoint );
    void setEndPoint(   const CPoint3d& endPoint );
    //
    // set the endpoints of a segment individually
    //

    double xcoord1() const { return p.xcoord(); }
    double xcoord2() const { return q.xcoord(); }
    double ycoord1() const { return p.ycoord(); }
    double ycoord2() const { return q.ycoord(); }
    double zcoord1() const { return p.zcoord(); }
    double zcoord2() const { return q.zcoord(); }
    //
    // extraction of coordinates of the segment
    //

    friend ostream& operator<<( ostream& os, const CSegment3d& seg );
    //
    // Output the segment to a stream
    //

    CSegment3d reverse() const;
    //
    // reverse the points in a segment
    //

    double length() const;
    double xlength() { return fabs( q.x - p.x ); };
    double ylength() { return fabs( q.y - p.y ); };
    double zlength() { return fabs( q.z - p.z ); };
    //
    // Calculate the length along the segment
    //

    //inline CPoint3d midPoint() { return (start()+end())/2.0; }

    boolean coplanar( const CSegment3d& seg2 ) const;
    //
    // check to see if current segment is coplanar to seg2
    //

    CPoint3d unitVector() const;
    //
    // Create a unit vector along the current segment
    //

    boolean intersection( const CPoint3d& p2 ) const;
    //
    // See if p2 is contained in the current segment 
    //

    boolean intersection( const CSegment3d& seg, CSegment3d& seg2 );
    //
    // intersection two segments to return a segment seg2
    //

    boolean combinable( const CSegment3d& t );
    //
    // can the current segemnt and segment t be combined together to form
    // one segment?
    //

    boolean attachTo( const CSegment3d& t, CSegment3d& newSegment );
    //
    // if possible, attach the current segment to t and return the new
    // segment, then return true, otherwise return false to say they
    // can not be attached.
    //

    double    angle( const CSegment3d& t ) const;
    double cosAngle( const CSegment3d& t ) const;
    double    angle( const CPoint3d& p1 ) const;  // added 13/12/01
    double cosAngle( const CPoint3d& p1 ) const;
    //
    // returns positive smallest angle between segments
    //
};

#endif
