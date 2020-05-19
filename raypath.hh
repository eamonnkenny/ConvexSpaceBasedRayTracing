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

#ifndef RAY_PATH_HH
#define RAY_PATH_HH

#include <iostream>
#include "point3d.hh"
#include "list.hh"
#include "contact.hh"
#include "pointinfo.hh"
#include "ComplexVector.hh"

class CRayPath
//==========================================================================
// = FUNCTION 
//     raypath.hh
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
     int numberOfReflections_;
     int numberOfTransmissions_;
     int numberOfDiffractions_; // Added 13/12/01
     CComplexVector fieldStrength_[3]; // Added 16/04/02 x,y,z component

     List<CPointInfo> pointList;

     int numberOfReflections()   { return numberOfReflections_; }
     int numberOfTransmissions() { return numberOfTransmissions_; }
     int numberOfDiffractions()  { return numberOfDiffractions_; }
     //
     // return the sizes of each type of interaction
     //

     CRayPath()
     {
       // default constructor
     }
      
     CRayPath( List<CPointInfo>& pointList1, 
               int numberOfTransmissions1,
               int numberOfReflections1,
               int numberOfDiffractions1 );
     CRayPath( const CRayPath&       otherRayPath, 
                     CComplexVector  otherFieldStrength[3] );
     //
     // make a ray path with or without field-strength values
     //

     CRayPath& operator=( const CRayPath& otherRayPath );
     //
     // assignment operator overload
     //

     inline int size() { return pointList.size(); }
     inline CPoint3d transmitter() { return pointList.GetItem(0).node; }
     inline CPoint3d receiver() { return pointList.GetItem(size()-1).node; }
     double angleOfArrival();
     double angleOfDeparture();
     //
     // define the angle of arrival and departure made in the XY plane
     //

     void clear()
     {
       // this will delete the contents of the current raypath
       pointList.clear();
     }

     ~CRayPath()
     {
       // destructor
       clear();
     }
};

#endif
