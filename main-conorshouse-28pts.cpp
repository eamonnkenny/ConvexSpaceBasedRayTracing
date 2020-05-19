//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    November 2001
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include <iostream>
#include "point3d.hh"
#include "reloader.hh"
#include "raytracing.hh"
#include "vector.hh"

int main()
{
  CPoint3d         transmitter, receiver;
  CComplexVector   totalFieldPathLoss, totalFieldPathLoss0;
  CComplexVector   totalFieldPathLossNE, totalFieldPathLossSE;
  CComplexVector   totalFieldPathLossNW, totalFieldPathLossSW;

  CBuilding        build;
  CReloadBuilding  reloader;
  CRayTracing      raytracingObject, goraytracingObject;
  CMatlab          matlabObject;
  CBaseTransceiver btsObject;
  CVector<double>  vector1;
  CProperties      directoryPathObject;
  ifstream         measurementStream;
  ofstream         resultsStream;
  string           outputFileName;
  string           directoryString = "../directory.xml";
  string           receiverLocationsFile;
  string           btsString = "bts915.dat";
  double           totalField;
  int              numberOfMeasurements, i;
  
  directoryPathObject.read( directoryString.c_str() );

  btsObject.read( btsString.c_str() );
  transmitter = btsObject.position();
  btsObject.setRandomWalk( true );

  reloader = CReloadBuilding( "conorhouse" );

  matlabObject.Story( 0, reloader.building_, "story1-" + btsObject.name() + ".txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1-" + btsObject.name() + ".txt" );

  receiverLocationsFile = CProperties::matlabCoverageDirectory + "/paper/receiverpoints-conorshouse.dat";
  cout << "Reading receiver list: " << receiverLocationsFile << endl;
  measurementStream.open( receiverLocationsFile.c_str() );
  measurementStream >> numberOfMeasurements;

  cout << "Number Of Measurements: " << numberOfMeasurements << endl;

  outputFileName ="receiverFields-" + btsObject.name() + ".txt";
  resultsStream.open( outputFileName.c_str() );

  for ( i = 0; i < numberOfMeasurements; i++ )
  {
    measurementStream >> receiver.x;
    measurementStream >> receiver.y;
    measurementStream >> receiver.z;

    raytracingObject = CRayTracing( reloader.building_, btsObject, receiver, LOSSY, totalFieldPathLoss0 );
    raytracingObject = CRayTracing( reloader.building_, btsObject, receiver-CPoint3d(-0.075,-0.075,0.0), LOSSY,
                                    totalFieldPathLossSW );
    raytracingObject = CRayTracing( reloader.building_, btsObject, receiver-CPoint3d(-0.075, 0.075,0.0), LOSSY,
                                    totalFieldPathLossNW );
    raytracingObject = CRayTracing( reloader.building_, btsObject, receiver-CPoint3d( 0.075,-0.075,0.0), LOSSY,
                                    totalFieldPathLossSE );
    raytracingObject = CRayTracing( reloader.building_, btsObject, receiver-CPoint3d( 0.075, 0.075,0.0), LOSSY,
                                    totalFieldPathLossNE );

    totalField = ( totalFieldPathLoss0.euclideanNorm() + totalFieldPathLossSW.euclideanNorm() 
                   + totalFieldPathLossNW.euclideanNorm() + totalFieldPathLossNE.euclideanNorm() 
                   + totalFieldPathLossSE.euclideanNorm() ) / 5.0;

    cout << "************************** Total Field **************************" << endl;
    cout << totalFieldPathLoss0  << endl;
    cout << totalFieldPathLossSW << endl;
    cout << totalFieldPathLossSE << endl;
    cout << totalFieldPathLossNW << endl;
    cout << totalFieldPathLossNE << endl;

    resultsStream << ( transmitter - receiver ).abs() << "  "
                  << 20.0 * log10 ( totalField / 1.4 ) << endl;
                  //<< 20.0 * log10 ( totalFieldPathLoss.euclideanNorm() / sqrt((transmitter-receiver).abs()) ) << endl;
    cout << transmitter << receiver << transmitter - receiver << endl;

    cout << "Raytrace performed between Tx:" << btsObject.position() << " and Rx:" << receiver << endl;
  }
  measurementStream.close();
  resultsStream.close();


  return 0;
}
