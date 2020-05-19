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

void runConfiguration( CBaseTransceiver& btsObject,
                       CReloadBuilding&  reloader );
double meanAbsSum( List<CRayPath>& allRayPaths );

int main()
{
  CPoint3d         trans    = CPoint3d( 4.668, 4.516, 1.45 );

  CReloadBuilding  reloader;
  CMatlab          matlabObject;
  CBaseTransceiver btsObject;
  ifstream         measurementStream;
  CProperties      directoryPathObject;

  directoryPathObject.read("../directory.xml");
  btsObject.read( "bts1000.dat" );
  btsObject.setPosition(trans);
  reloader = CReloadBuilding( "printingclosed" );

  matlabObject.Story( 0, reloader.building_, "story1-" + btsObject.name() + ".txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1-" + btsObject.name() + ".txt" );
 
  // Random Walk with 1 and 3 reflections
  btsObject.setRandomWalk( true );
  btsObject.setNumberOfReflections( 1 );
  runConfiguration( btsObject, reloader );

  btsObject.setNumberOfReflections( 3 );
  runConfiguration( btsObject, reloader );

  // No Random Walk with 1 and 3 reflections
  btsObject.setRandomWalk( false );
  btsObject.setNumberOfReflections( 1 );
  runConfiguration( btsObject, reloader );

  btsObject.setNumberOfReflections( 3 );
  runConfiguration( btsObject, reloader );
}


void runConfiguration( CBaseTransceiver& btsObject,
                       CReloadBuilding&  reloader )
{
  CRayTracing      raytracingObject, goraytracingObject;
  ofstream         resultsStream;
  CPoint3d         receiverPoint, receiverStart;
  ofstream         samplepoints("receivers.txt");
  ifstream         measurementStream;
  List<CRayPath>   allValidRays;
  string           measurementFile;
  double           pathLoss;
  double           absTotalFieldPathLoss;
  CComplexVector   totalFieldPathLoss;
  int              numberOfMeasurements, i;

  measurementFile = CProperties::measurementDirectory + "/measureacross.dat";
  measurementStream.open(measurementFile.c_str());
  measurementStream >> numberOfMeasurements;
  samplepoints << btsObject.position() << endl;

  cout << "Number Of Measurements: " << numberOfMeasurements << endl;
  if ( btsObject.numberOfReflections() == 1 )
    if ( btsObject.randomWalk() == true )
      resultsStream.open( "corridorclosedR1RW.txt" );
    else
      resultsStream.open( "corridorclosedR1.txt" );

  if ( btsObject.numberOfReflections() == 3 )
    if ( btsObject.randomWalk() == true )
      resultsStream.open( "corridorclosedR3RW.txt" );
    else
      resultsStream.open( "corridorclosedR3.txt" );

  for ( i = 0; i < numberOfMeasurements; i++ )
  {
    measurementStream >> receiverPoint.x;
    measurementStream >> receiverPoint.y;
    measurementStream >> receiverPoint.z;
    measurementStream >> pathLoss;

    if ( btsObject.randomWalk() == false )
    {
      raytracingObject = CRayTracing( reloader.building_, btsObject, 
                                     receiverPoint, LOSSY, totalFieldPathLoss );
      resultsStream << receiverPoint.y << "  " 
                    << totalFieldPathLoss.euclideanNorm() << "  " 
                    << pathLoss << endl;
      cout << receiverPoint.y << "  " 
           << totalFieldPathLoss.euclideanNorm() << "  " 
           << pathLoss << endl;
    }
    else
    {
      raytracingObject.MimoRayTracing( allValidRays, reloader.building_, 
                                       btsObject, receiverPoint, LOSSY );

      absTotalFieldPathLoss = meanAbsSum( allValidRays );
      resultsStream << receiverPoint.y << "  " << absTotalFieldPathLoss << "  " 
                    << pathLoss << endl;
      cout << receiverPoint.y << "  " << absTotalFieldPathLoss << "  " 
           << pathLoss << endl;
      allValidRays.clearAndClearSubList();
    }

    samplepoints << receiverPoint << endl;
  }
  measurementStream.close();
  resultsStream.close();
  samplepoints.close();
}

double meanAbsSum( List<CRayPath>& allRayPaths )
{
  double   R;
  CRayPath *rayPath;
  complex  fieldStrength, expjtest, temp;
  int      i, l, numberOfIterations = 100;

  R=0;

  for ( i = 0; i < numberOfIterations; i++ )
  {
    temp = ComplexZero;
    for ( l = 0; l < allRayPaths.size(); l++ )
    {
      rayPath        = &allRayPaths.GetItem(l);
      fieldStrength  = rayPath->fieldStrength_[2].z;

      expjtest = CMaths::expj( TwoPi * ( double(rand()) / double(RAND_MAX) ) );
      temp += fieldStrength * expjtest;
    }
    R += cabs( temp );
  }

  R = R / numberOfIterations;
  return R;
}
