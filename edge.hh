//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    December 3rd 2001
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef EDGES_HH
#define EDGES_HH

#include <iostream>
#include "segment3d.hh"

class CEdge
//==========================================================================
// = FUNCTION 
// 	edge.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      Stores the image of a diffraction edge (segment3d) and the 
//      reference to the edge that exists if the edge is placed in
//      an edge tree.
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
     CSegment3d  edge_;
     int         previousImageIndex_;

     CEdge()
     {
       edge_               = CSegment3d();
       previousImageIndex_ = -1;
     }

     CEdge( const CPoint3d& p, const CPoint3d& q, int previousImageIndex );
     CEdge( const CSegment3d& seg, int previousImageIndex );
     //
     // each node of the tree contains an image edge and a reference back
     // to the image point that generated it.
     //

     CEdge& operator=( const CEdge& image2 );
     //
     // return the copied edge
     //

     friend ostream& operator<< ( ostream& s, const CEdge& image2 );
     //
     // output image information to a file buffer, or screen.
     //
};

#endif
