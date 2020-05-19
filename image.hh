//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    November 19th 2001
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef SOURCE_IMAGES_HH
#define SOURCE_IMAGES_HH

#include "point3d.hh"

class CImage
//==========================================================================
// = FUNCTION 
// 	image.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      Stores the image point and the reference to an image point that 
//      generated that point in a tree of images.
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
     CPoint3d    point_;
     int         previousImageIndex_;

     CImage()
     {
       point_              = CPoint3d( 0.0, 0.0, 0.0 );
       previousImageIndex_ = -1;
     }

     CImage( const CPoint3d& transmitter, int   previousImageIndex );
     //
     // each node of the tree contains an image point and a reference back
     // to the image point that generated it.
     //

     CImage& operator=( const CImage& image2 );
     //
     // return the copied image
     //

     friend ostream& operator<< ( ostream& s, const CImage& image2 );
     //
     // output image information to a file buffer, or screen.
     //
};

#endif
