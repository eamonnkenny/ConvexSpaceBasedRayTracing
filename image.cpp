//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// Last Modification:     November 2001
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "image.hh"

CImage::CImage( const CPoint3d& transmitter, int   previousImageIndex )
{
  // each node of the tree contains an image point and a reference to
  point_              = transmitter;
  previousImageIndex_ = previousImageIndex;
}

CImage& CImage::operator=( const CImage& image2 )
{
  point_              = image2.point_;
  previousImageIndex_ = image2.previousImageIndex_;

  return *this;
}

ostream& operator<<( ostream& s, const CImage& im )
{
  return s << "Point: " << im.point_ << "with previous index: " 
           << im.previousImageIndex_;
}
