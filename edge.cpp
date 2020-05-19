//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// Last Modification:     December 3rd 2001
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "edge.hh"

CEdge::CEdge( const CSegment3d& seg, int previousImageIndex )
{
  // each node of the tree contains an image edge and a reference to
  // a previous ordered edge node 
  edge_               = seg;
  previousImageIndex_ = previousImageIndex;
}

CEdge::CEdge( const CPoint3d& p, const CPoint3d& q, int previousImageIndex )
{
  // each node of the tree contains an image edge and a reference to
  // a previous ordered edge node 
  edge_               = CSegment3d( p, q );
  previousImageIndex_ = previousImageIndex;
}

CEdge& CEdge::operator=( const CEdge& image2 )
{
  edge_               = image2.edge_;
  previousImageIndex_ = image2.previousImageIndex_;

  return *this;
}

ostream& operator<<( ostream& s, const CEdge& im )
{
  return s << "Edge: " << im.edge_ << "with previous index: " 
           << im.previousImageIndex_;
}
