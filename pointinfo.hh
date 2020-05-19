//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    December 13th 2001
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef POINT_INFO_HH
#define POINT_INFO_HH

#include <iostream>
#include "point3d.hh"
#include "contact.hh"

class CPointInfo
//==========================================================================
// = FUNCTION 
// 	pointinfo.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      The ray-path consists of a list of 3d-points which can be acted
//      upon to obtain angles of departure, angles of arrival and length of
//      path.
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
    CPoint3d    node;
    int         convexNumber;
    int         boundaryNumber1;
    int         boundaryNumber2;
    int         edgeNumber;
    contactType contact; 

  CPointInfo()
  {
    edgeNumber        = -1;
    boundaryNumber1   = -1;
    boundaryNumber2   = -1;
    convexNumber      = -1;
    node              = CPoint3d( 0.0, 0.0, 0.0 );
    contact           = TRANSMITTER;
    // default constructor
  }

  CPointInfo( const CPoint3d& node1, int  convexNumber1, 
              int   b1, contactType contact1 );
  // 
  // constructor for a point node including all information for a 
  // reflection, transmitter, receiver or transmission point.
  //

  CPointInfo( const CPoint3d& node1, int  convexNumber1, 
              int   b1, int  b2, int edgeNumber, contactType contact1 );
  // 
  // constructor for a point node including all information for a
  // diffraction point.
  //

  CPointInfo& operator=( const CPointInfo& otherPointInfo );
  //
  // copy operator
  //

  friend ostream& operator<<( ostream& s, const CPointInfo& pi ); 

  ~CPointInfo()
  {
    // default destructor
  }
};

#endif
