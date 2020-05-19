//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    September 2003
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include <iostream>
#include "point3d.hh"
#include "reloader.hh"
#include "raytracing.hh"
#include "maths-utilities.hh"
#include "configuration.hh"
#include "vector.hh"

int runConfiguration( CBaseTransceiver& btsObject, CReloadBuilding&  reloader, const string& receiverFileName );

int main( int argc, char *argv[] )
{
  CReloadBuilding  reloader;
  CMatlab          matlabObject;
  CBaseTransceiver btsObject;
  CProperties      directoryPathObject;
  string           receiverFileName;
  string           buildingName = argv[1];

  directoryPathObject.read("../directory.xml");
  btsObject.read( argv[2] );
  reloader = CReloadBuilding( buildingName );

  matlabObject.Story( 0, reloader.building_, "story1-" + btsObject.name() + ".txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1-" + btsObject.name() + ".txt" );
 
  // Random Walk with 1 and 3 reflections
  btsObject.setRandomWalk( false );
  receiverFileName = argv[3];
  return runConfiguration( btsObject, reloader, receiverFileName );
}


int runConfiguration( CBaseTransceiver& btsObject, CReloadBuilding& reloader, const string& receiverFileName )
{
  CRayTracing      raytracingObject, goraytracingObject;
  ofstream         resultsStream;
  CPoint3d         receiverPoint, receiverStart, stepDirection;
  ofstream         samplepoints("receivers.txt");
  ifstream         receiverStream;
  List<CRayPath>   allValidRays;
  string           measurementFile, direction, totalFieldOutputFileName;
  double           stepSize, i_value=0.01;
  CComplexVector   totalFieldPathLoss;
  int              numberOfReceiverPoints, i;
  string           strStartOutput="totalfield";
  string           strEndOutput="txt";

  measurementFile = CProperties::btsDirectory + "/" + receiverFileName;
  receiverStream.open(measurementFile.c_str());
  receiverStream >> receiverStart;
  receiverStream >> direction;
  receiverStream >> numberOfReceiverPoints;
  receiverStream >> stepSize;
  cout << "Receiver Start Point: " << receiverPoint << " with step-size: " << stepSize << endl;
  samplepoints << btsObject.position() << endl;
  if ( direction == "y-direction" )
  {
    stepDirection = CPoint3d( 0.0, stepSize, 0.0 );
  }
  else if ( direction == "z-direction" )
  {
    stepDirection = CPoint3d( 0.0, 0.0, stepSize );
  }
  else if ( direction == "x-direction" )
  {
    stepDirection = CPoint3d( stepSize, 0.0, 0.0 );
  }
  else 
  {
    cout << "Error: Big problem, not a defined x,y or z-direction for free-space plot" << endl;
    return -1;
  }

  string::iterator endPoint;
  totalFieldOutputFileName = receiverFileName;
  totalFieldOutputFileName.replace(0,9,strStartOutput);
  endPoint = totalFieldOutputFileName.end();
  totalFieldOutputFileName.replace(endPoint-3,endPoint,strEndOutput);
  cout << "Output Total Field Filename: " << totalFieldOutputFileName << endl;

  cout << " *** Starting: " << receiverStart << " with step: " << stepDirection << endl;

  resultsStream.open( totalFieldOutputFileName.c_str() );

  cout << "Number Of Measurements: " << numberOfReceiverPoints << endl;

  for ( i = 0; i < numberOfReceiverPoints; i++ )
  {
    receiverPoint = receiverStart + stepDirection * double(i);
    raytracingObject = CRayTracing( reloader.building_, btsObject, receiverPoint, LOSSY, totalFieldPathLoss );

    if ( direction == "y-direction" )
    {
      resultsStream << receiverPoint.y << "  " << totalFieldPathLoss.euclideanNorm() * i_value << endl;
      cout          << receiverPoint.y << "  " << totalFieldPathLoss.euclideanNorm() * i_value << endl;
    }
    else if ( direction == "z-direction" )
    {
      resultsStream << receiverPoint.z << "  " << totalFieldPathLoss.euclideanNorm() * i_value << endl;
      cout          << receiverPoint.z << "  " << totalFieldPathLoss.euclideanNorm() * i_value << endl;
    }
    else if ( direction == "x-direction" )
    {
      resultsStream << receiverPoint.x << "  " << totalFieldPathLoss.euclideanNorm() * i_value  << endl;
      cout          << receiverPoint.x << "  " << totalFieldPathLoss.euclideanNorm() * i_value  << endl;
    }

    samplepoints << receiverPoint << endl;
  }
  receiverStream.close();
  resultsStream.close();
  samplepoints.close();
  return 0;
}
