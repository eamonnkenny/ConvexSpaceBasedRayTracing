//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    November 22nd 2001
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include "raypath.hh"

CRayPath::CRayPath( List<CPointInfo>& pointList1, 
                    int numberOfTransmissions1, int numberOfReflections1,
                    int numberOfDiffractions1 )
{
  // constructor
  pointList = pointList1;

  numberOfTransmissions_ = numberOfTransmissions1;
  numberOfReflections_   = numberOfReflections1;
  numberOfDiffractions_  = numberOfDiffractions1; // Added 13/12/01
  fieldStrength_[0]      = CComplexVector();
  fieldStrength_[1]      = CComplexVector();
  fieldStrength_[2]      = CComplexVector();
}

CRayPath& CRayPath::operator=( const CRayPath& otherRayPath )
{
  if ( this != &otherRayPath )
  {
    pointList = otherRayPath.pointList;
  
    numberOfTransmissions_ = otherRayPath.numberOfTransmissions_;
    numberOfReflections_   = otherRayPath.numberOfReflections_;
    numberOfDiffractions_  = otherRayPath.numberOfDiffractions_; 
    fieldStrength_[0]      = otherRayPath.fieldStrength_[0];
    fieldStrength_[1]      = otherRayPath.fieldStrength_[1];
    fieldStrength_[2]      = otherRayPath.fieldStrength_[2];
    // Added 08/01/02
  }

  return *this;
}

CRayPath::CRayPath( const CRayPath&       otherRayPath,
                          CComplexVector  otherFieldStrength[3] )
{
  // constructor
  pointList = otherRayPath.pointList;
  
  numberOfTransmissions_ = otherRayPath.numberOfTransmissions_;
  numberOfReflections_   = otherRayPath.numberOfReflections_;
  numberOfDiffractions_  = otherRayPath.numberOfDiffractions_; 
  fieldStrength_[0]      = otherFieldStrength[0];
  fieldStrength_[1]      = otherFieldStrength[1];
  fieldStrength_[2]      = otherFieldStrength[2];
}

double CRayPath::angleOfArrival()
{
  CPoint3d outgoingRay;
  int      N;

  N = pointList.size()-1;
  outgoingRay = pointList.GetItem(N-1).node - pointList.GetItem(N).node;
  return outgoingRay.angleXY();
}

double CRayPath::angleOfDeparture()
{
  CPoint3d outgoingRay;

  outgoingRay = pointList.GetItem(1).node - pointList.GetItem(0).node;
  return outgoingRay.angleXY();
}
