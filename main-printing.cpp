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
#include "configuration.hh"
#include "vector.hh"

int main()
{
  CPoint3d         trans    = CPoint3d( 5.468, 6.51, 1.45 );
  CPoint3d         receiverPoint, receiverStart;
  CComplexVector   totalFieldPathLoss;

  CBuilding        build;
  CReloadBuilding  reloader;
  CRayTracing      raytracingObject, goraytracingObject;
  CMatlab          matlabObject;
  CBaseTransceiver btsObject;
  CVector<double>  vector1;
  ifstream         measurementStream;
  ofstream         resultsStream;
  int              numberOfMeasurements, i;
  double           pathLoss;
  ofstream         samplepoints("receivers.txt");
  CProperties      directoryPathObject;
  string           measurementFile;
  int              numReflections = 5, numTransmissions = 6;
  string           outputFileName, suffixName;
  ostringstream    temp;

  temp << "r" << numReflections << "t" << numTransmissions;
  
  suffixName = temp.str();

  directoryPathObject.read( "../directory.xml" );

  btsObject.read( "bts1000.dat" );
  btsObject.setRandomWalk( true );
  btsObject.setPosition(trans);
  btsObject.setNumberOfReflections( numReflections );
  btsObject.setNumberOfTransmissions( numTransmissions );

  // printing house with Room 21 doors open
  reloader = CReloadBuilding( "printinghouse" );

  matlabObject.Story( 0, reloader.building_, "story1-" + btsObject.name() + ".txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1-" + btsObject.name() + ".txt" );
 
  samplepoints << trans << endl;
  measurementFile = CProperties::measurementDirectory + "/measureoffice.dat";
  measurementStream.open( measurementFile.c_str() );
  measurementStream >> numberOfMeasurements;

  cout << "Number Of Measurements: " << numberOfMeasurements << endl;
  
  outputFileName ="corridoropendoor1-" + suffixName + ".txt";
  resultsStream.open( outputFileName.c_str() );
  for ( i = 0; i < numberOfMeasurements; i++ )
  {
    measurementStream >> receiverPoint.x;
    measurementStream >> receiverPoint.y;
    measurementStream >> receiverPoint.z;
    measurementStream >> pathLoss;

    raytracingObject = CRayTracing( reloader.building_, btsObject, 
                                    receiverPoint, LOSSY, totalFieldPathLoss );

    resultsStream << receiverPoint.x << "  " 
                  << totalFieldPathLoss.euclideanNorm() << "  " 
		          << pathLoss << endl;
    cout << receiverPoint.x << "  " 
         << totalFieldPathLoss.euclideanNorm() << "  " 
		 << pathLoss << endl;
    samplepoints << receiverPoint << endl;
  }
  measurementStream.close();
  resultsStream.close();
  samplepoints.close();

  // printing house with Room 21 doors open but different complex permittivity 
  reloader = CReloadBuilding( "printinghousee5" );

  matlabObject.Story( 0, reloader.building_, "story1.txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1.txt" );
 
  measurementStream.open( measurementFile.c_str() );
  measurementStream >> numberOfMeasurements;

  cout << "Number Of Measurements: " << numberOfMeasurements << endl;
  outputFileName = "corridoropendoor2-" + suffixName + ".txt";
  resultsStream.open( outputFileName.c_str() );
  for ( i = 0; i < numberOfMeasurements; i++ )
  {
    measurementStream >> receiverPoint.x;
    measurementStream >> receiverPoint.y;
    measurementStream >> receiverPoint.z;
    measurementStream >> pathLoss;

    raytracingObject = CRayTracing( reloader.building_, btsObject, 
                                    receiverPoint, LOSSY, totalFieldPathLoss );

    resultsStream << receiverPoint.x << "  " 
                  << totalFieldPathLoss.euclideanNorm() << "  " 
		          << pathLoss << endl;
    cout << receiverPoint.x << "  " 
         << totalFieldPathLoss.euclideanNorm() << "  " 
		 << pathLoss << endl;
  }
  measurementStream.close();
  resultsStream.close();

  // printing house with Room 21 double doors closed
  reloader = CReloadBuilding( "conorclosed" );

  matlabObject.Story( 0, reloader.building_, "story1.txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1.txt" );
 
  measurementStream.open( measurementFile.c_str() );
  measurementStream >> numberOfMeasurements;

  cout << "Number Of Measurements: " << numberOfMeasurements << endl;
  outputFileName = "corridorclosed1-" + suffixName + ".txt";
  resultsStream.open( outputFileName.c_str() );
  for ( i = 0; i < numberOfMeasurements; i++ )
  {
    measurementStream >> receiverPoint.x;
    measurementStream >> receiverPoint.y;
    measurementStream >> receiverPoint.z;
    measurementStream >> pathLoss;

    raytracingObject = CRayTracing( reloader.building_, btsObject, 
                                    receiverPoint, LOSSY, totalFieldPathLoss );

    resultsStream << receiverPoint.x << "  " 
                  << totalFieldPathLoss.euclideanNorm() << "  " 
		          << pathLoss << endl;
    cout << receiverPoint.x << "  " 
         << totalFieldPathLoss.euclideanNorm() << "  " 
		 << pathLoss << endl;
  }
  measurementStream.close();
  resultsStream.close();

  // printing house with Room 21 having no double doors
  reloader = CReloadBuilding( "conornodoors" );

  matlabObject.Story( 0, reloader.building_, "story1.txt" );
  matlabObject.Floor( 0, reloader.building_, "floor1.txt" );
 
  measurementStream.open("measureoffice.dat");
  measurementStream.open( measurementFile.c_str() );
  measurementStream >> numberOfMeasurements;

  cout << "Number Of Measurements: " << numberOfMeasurements << endl;
  outputFileName = "corridornodoors-" + suffixName + ".txt";
  resultsStream.open( outputFileName.c_str() );
  for ( i = 0; i < numberOfMeasurements; i++ )
  {
    measurementStream >> receiverPoint.x;
    measurementStream >> receiverPoint.y;
    measurementStream >> receiverPoint.z;
    measurementStream >> pathLoss;

    raytracingObject = CRayTracing( reloader.building_, btsObject, 
                                    receiverPoint, LOSSY, totalFieldPathLoss );

    resultsStream << receiverPoint.x << "  " 
                  << totalFieldPathLoss.euclideanNorm() << "  " 
		          << pathLoss << endl;
    cout << receiverPoint.x << "  " 
         << totalFieldPathLoss.euclideanNorm() << "  " 
		 << pathLoss << endl;
  }
  measurementStream.close();
  resultsStream.close();


  return 0;
}
