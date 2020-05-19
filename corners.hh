//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Prof. Frank Boland
// First Modification:    December 3rd 2002
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef CORNERS_HH
#define CORNERS_HH

#include <iostream>
#include "edge.hh"
#include "list.hh"

class CCorners
//==========================================================================
// = FUNCTION 
// 	corners.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      define a list of corners which can be used in the calculation of
//      corner diffractions
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
     List<CPoint3d>   listOfCorners;
     List<CSegment3d> edges[3];

     CCorners()
     {
       // default constructor
     }

     boolean isPointNotACorner( const CPoint3d& aPoint )
     {
       if ( isPointACorner( aPoint ) == false )
         return true;
       else
         return false;
     }

     boolean isPointACorner( const CPoint3d& aPoint )
     {
       int i;

       for ( i = 0; i < listOfCorners.size(); i++ )
       {
         if ( aPoint == listOfCorners.GetItem(i) )
           return true;
       }
       return false;
     }

     void append( const CPoint3d&   aPoint, const CSegment3d& edgeI,
                  const CSegment3d& edgeJ,  const CSegment3d& edgeK,
		  int&  cornerCount ) 
     { 
       if ( isPointNotACorner( aPoint ) )
       {
         listOfCorners.append( aPoint );
         edges[0].append( edgeI );
         edges[1].append( edgeJ );
         edges[2].append( edgeK );
	 //cout << "new corner: " << aPoint << endl
	 //     << " edges: " << edgeI << edgeJ << edgeK << endl;
	 cornerCount++;
       }
     }

     inline int size() { return listOfCorners.size(); }

     ~CCorners()
     {
       listOfCorners.clear();
       edges[2].clear();
       edges[1].clear();
       edges[0].clear();
     }
};

#endif
