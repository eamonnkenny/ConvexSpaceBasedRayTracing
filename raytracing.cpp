//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    November 16th 2000
// Previous Projects:     None
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "raytracing.hh"

#include <time.h>
int firstSeed=1;

void CRayTracing::MimoRayTracing( 
                                List<CRayPath>&   nonZeroRays,
                          const CBuilding&        buildingObject,
                                CBaseTransceiver& btsObject,
                          const CPoint3d&         receiverPoint,
                          const mediaType&        media )
{
  List<CPlane>     reflectionPlanes;
  CReflection      reflectionObject;
  CDiffraction     diffractionObject;
  List<CRayPath>   validRayPath;
  List<CPointInfo> diffractionInfo, pointList;
  CEdgeList        *diffractionImages;
  List<CImage>     *transmitterImages;
  CComplexVector   fieldRayPathLoss[3], complexZeroValue, totalFieldPathLoss;
  int              i, j, M, transConvexIndex, nonZeroCounter, N, t0;
  CPoint3d         transmitterPoint;
  CMatlab          matlabObject;
  ofstream         fieldFilePtr;
  CRayPath         temporaryRayPath;
  CMemoryStorage   storageObject;
  CCorners         cornerDiffractionPoints;
  CIntrinsic       intrinsicObject;

  fieldFilePtr.open( "fieldlog.txt" );
  //cout << "Ray-tracing Starting" << endl;
  // calculate possible reflection planes

  buildingObject.calculateReflectionPlanes( reflectionPlanes );

  transmitterPoint = btsObject.position();
  buildingObject.locateConvexNumber( transmitterPoint,
                                                        transConvexIndex );
  diffractionImages = storageObject.CreateEdgeList( 
                                    btsObject.numberOfReflections() );
  transmitterImages = storageObject.CreateImageList(
                                    btsObject.numberOfReflections()+1 );

  // create all images of the transmitter up to a given maximum order
  t0 = intrinsicObject.StartClock();

  reflectionObject.CreateImagesOfTransmitter(
                       transmitterImages, buildingObject, btsObject,
                       reflectionPlanes, transmitterPoint, transConvexIndex );


  //intrinsicObject.ClockTime( t0, "Time to build point image tree" );

  // create diffraction image tree and return information about which
  // convex space and boundaries each edge touches.
  t0 = intrinsicObject.StartClock();
  diffractionObject.createImagesOfDiffractionEdges( diffractionImages, 
                                    buildingObject, btsObject, diffractionInfo, 
                                    reflectionPlanes, cornerDiffractionPoints );
  //intrinsicObject.ClockTime( t0, "Time to build edge diffraction tree" );
  
  matlabObject.edgeImages( diffractionImages[0] );

  // calculate all possible reflection paths first
  t0 = intrinsicObject.StartClock();
  reflectionObject = CReflection( validRayPath,    transmitterImages,
                                  buildingObject, btsObject, transConvexIndex,
                                  transmitterPoint, receiverPoint );

  N = validRayPath.size();
  //cout << "Number Of Valid Reflections: " << N << endl;
  //intrinsicObject.ClockTime( t0, "Time for reflections" );
  // create files containing path information
  //matlabObject.fullRayPaths( validRayPath );
  //matlabObject.reflections(  validRayPath, "reflection.txt" );

  //matlabObject.printRays( buildingObject, validRayPath, "allref.txt" );

  // then calculate all possible diffraction paths secondly
  t0 = intrinsicObject.StartClock();
  if ( btsObject.numberOfDiffractions() > 0 ) 
    diffractionObject = CDiffraction( validRayPath, diffractionInfo,
                                      transmitterImages, diffractionImages,
                                      cornerDiffractionPoints, buildingObject,
                                      btsObject, transConvexIndex, 
                                      transmitterPoint, receiverPoint);
  M = validRayPath.size();
  //cout << "Number Of Valid Diffractions: " << M-N << endl;
  //intrinsicObject.ClockTime( t0, "Time for diffractions" );

  //matlabObject.diffractions(  validRayPath, N );
  //matlabObject.printRays( buildingObject, validRayPath, "alldiff.txt" );

  //cout << "Total Number Of Valid Rays: " << M << endl;
  //cout << "Ray-tracing Ending" << endl;

  //areAllValidDiffractions( validRayPath, buildingObject, 
                           //diffractionImages[0].listOfEdges );
  nonZeroCounter     = 0;
  complexZeroValue   = CComplexVector( 0.0, 0.0, 0.0 );
  totalFieldPathLoss = complexZeroValue;
  t0 = intrinsicObject.StartClock();

  for ( i = 0; i < M; i++ )
  {
    //if ( validRayPath.GetItem(i).numberOfDiffractions() == 0 )
    totalField( fieldRayPathLoss[0], validRayPath.GetItem(i),
                diffractionImages[0], cornerDiffractionPoints,
                buildingObject, btsObject, media, X_POLARISATION );
    totalField( fieldRayPathLoss[1], validRayPath.GetItem(i),
                diffractionImages[0], cornerDiffractionPoints,
                buildingObject, btsObject, media, Y_POLARISATION );
    totalField( fieldRayPathLoss[2], validRayPath.GetItem(i), 
                diffractionImages[0], cornerDiffractionPoints,
                buildingObject, btsObject, media, Z_POLARISATION );

    if ( fieldRayPathLoss[0] != complexZeroValue
      || fieldRayPathLoss[1] != complexZeroValue 
      || fieldRayPathLoss[2] != complexZeroValue )
    {
      nonZeroCounter++;
      pointList        = validRayPath.GetItem(i).pointList;
      temporaryRayPath = validRayPath.GetItem(i);
      temporaryRayPath = CRayPath( temporaryRayPath, fieldRayPathLoss );
      nonZeroRays.append( temporaryRayPath );
      totalFieldPathLoss += fieldRayPathLoss[0];
      totalFieldPathLoss += fieldRayPathLoss[1];
      totalFieldPathLoss += fieldRayPathLoss[2];
      fieldFilePtr << totalFieldPathLoss.euclideanNorm() << endl;
      for ( j = 0; j < pointList.size(); j++ )
        fieldFilePtr << pointList.GetItem(j) << endl;
      pointList.clear();
      temporaryRayPath.pointList.clear();
    }
  }
  fieldFilePtr.close();
  //cout << "Single Ray Total Pathloss Value: " << totalFieldPathLoss << endl;
  //cout << "number Of non-zero points: " << nonZeroCounter << endl;
  // matlabObject.reflections( *nonZeroRays, "reflection2.txt" );

  validRayPath.clearAndClearSubList();
  reflectionPlanes.clear();
  diffractionInfo.clear();

  for ( i = 0; i < btsObject.numberOfReflections()+1; i++ )
    transmitterImages[i].clear();

  for ( i = 0; i < btsObject.numberOfReflections(); i++ )
    diffractionImages[i].listOfEdges.clear();

  storageObject.DeleteImageList( transmitterImages );
  storageObject.DeleteEdgeList( diffractionImages );
  //intrinsicObject.ClockTime( t0, "Time for ray-trace" );
}

CRayTracing::CRayTracing( const CBuilding&        buildingObject,
                          const CBaseTransceiver& btsObject,
                          const CPoint3d&         receiverPoint,
                          const mediaType&        media )
{
  List<CPlane>     reflectionPlanes;
  CReflection      reflectionObject;
  CDiffraction     diffractionObject;
  List<CRayPath>   validRayPath, *nonZeroRays;
  CRayPath         temporaryRayPath;
  List<CPointInfo> diffractionInfo, pointList;
  CEdgeList        *diffractionImages;
  List<CImage>     *transmitterImages;
  CComplexVector   fieldRayPathLoss[3], complexZeroValue, totalFieldPathLoss;
  int              i, j, M, N, transConvexIndex, nonZeroCounter;
  CPoint3d         transmitterPoint;
  //CIntrinsic       intrinsicObject;
  CMatlab          matlabObject;
  ofstream         fieldFilePtr;
  CMemoryStorage   storageObject;
  CCorners         cornerDiffractionPoints;

  fieldFilePtr.open( "fieldlog.txt" );

  diffractionImages = storageObject.CreateEdgeList( btsObject.numberOfReflections() );
  transmitterImages = storageObject.CreateImageList( btsObject.numberOfReflections()+1 );

  nonZeroRays = new List<CRayPath>;
  //cout << "Ray-tracing Starting" << endl;
  // calculate possible reflection planes
  //reflectionPlanes  = buildingObject.calculateReflectionPlanes();
  buildingObject.calculateReflectionPlanes( reflectionPlanes );

  transmitterPoint = btsObject.position();
  buildingObject.locateConvexNumber( transmitterPoint,
                                                        transConvexIndex );

  // create all images of the transmitter up to a given maximum order
  //t0 = intrinsicObject.StartClock();
  reflectionObject.CreateImagesOfTransmitter(
                       transmitterImages, buildingObject, btsObject,
                       reflectionPlanes, transmitterPoint, transConvexIndex );
  //intrinsicObject.ClockTime( t0, "Time to build point image tree" );

  // create diffraction image tree and return information about which
  // convex space and boundaries each edge touches.
  //t0 = intrinsicObject.StartClock();
  diffractionObject.createImagesOfDiffractionEdges( diffractionImages, 
                                    buildingObject, btsObject, diffractionInfo,
                                    reflectionPlanes, cornerDiffractionPoints );
  //intrinsicObject.ClockTime( t0, "Time to build edge diffraction tree" );
  
  //matlabObject.edgeImages( diffractionImages[0] );

  // calculate all possible reflection paths first
  //t0 = intrinsicObject.StartClock();
  reflectionObject = CReflection( validRayPath,     transmitterImages,
                                  buildingObject, btsObject, transConvexIndex,
                                  transmitterPoint, receiverPoint );
  N = validRayPath.size();
  //cout << "Number Of Valid Reflections: " << N-1 << endl;
  //intrinsicObject.ClockTime( t0, "Time for reflections" );
  // create files containing path information
  matlabObject.fullRayPaths( validRayPath );
  matlabObject.reflections(  validRayPath, "reflection.txt" );


  // then calculate all possible diffraction paths secondly
  //t0 = intrinsicObject.StartClock();
  if ( btsObject.numberOfDiffractions() > 0 ) 
    diffractionObject = CDiffraction( validRayPath, diffractionInfo,
                                      transmitterImages,diffractionImages, 
                                      cornerDiffractionPoints, buildingObject,
                                      btsObject, transConvexIndex, 
                                      transmitterPoint, receiverPoint );
  M = validRayPath.size();
  //cout << "Number Of Valid Diffractions: " << M-N << endl;
  //intrinsicObject.ClockTime( t0, "Time for diffractions" );

  matlabObject.diffractions( validRayPath, N );

  //cout << "Total Number Of Valid Rays including diffractions: " << M << endl;
  //cout << "Ray-tracing Ending" << endl;

  //areAllValidDiffractions( validRayPath, buildingObject, 
                           //diffractionImages[0].listOfEdges );
  nonZeroCounter     = 0;
  complexZeroValue   = CComplexVector( 0.0, 0.0, 0.0 );
  totalFieldPathLoss = complexZeroValue;
  //t0 = intrinsicObject.StartClock();
  /*for ( i = 0; i < M; i++ )
  {
    cout << "---------------Ray Path " << i << "-------------------" << endl;
    cout << validRayPath.GetItem(i).pointList.GetItem(1) << endl;
  }*/

  for ( i = 0; i < M; i++ )
  {
    //cout << "For Ray number Z component: " << i << endl;
    /*totalField( fieldRayPathLoss[0], validRayPath.GetItem(i),
                diffractionImages[0], cornerDiffractionPoints,
                buildingObject, btsObject, media, 
                X_POLARISATION );
    totalField( fieldRayPathLoss[1], validRayPath.GetItem(i),
                diffractionImages[0], cornerDiffractionPoints,
                buildingObject, btsObject, media, Y_POLARISATION );*/
    totalField( fieldRayPathLoss[2], validRayPath.GetItem(i), 
                diffractionImages[0], cornerDiffractionPoints,
                buildingObject, btsObject, media, Z_POLARISATION );
    
    //cout << "field path loss [" << i << "]: " << fieldRayPathLoss[2] << endl;

    if ( fieldRayPathLoss[0] != complexZeroValue
      || fieldRayPathLoss[1] != complexZeroValue 
      || fieldRayPathLoss[2] != complexZeroValue )
    {
      /*if ( validRayPath.GetItem(i).numberOfDiffractions() == 1 )
        cout << "diffracted field: " << fieldRayPathLoss[2] << endl;
      else if ( validRayPath.GetItem(i).numberOfDiffractions() == 0 
             && validRayPath.GetItem(i).numberOfReflections() == 0 )
        cout << "incident field: " << fieldRayPathLoss[2] << endl;
      else
        cout << "reflected field: " << fieldRayPathLoss[2] << endl;*/

      nonZeroCounter++;
      pointList        = validRayPath.GetItem(i).pointList;
      temporaryRayPath = validRayPath.GetItem(i);
      temporaryRayPath = CRayPath( temporaryRayPath, fieldRayPathLoss );
      nonZeroRays->append( temporaryRayPath );
      totalFieldPathLoss += fieldRayPathLoss[0];
      totalFieldPathLoss += fieldRayPathLoss[1];
      totalFieldPathLoss += fieldRayPathLoss[2];
      fieldFilePtr << totalFieldPathLoss.euclideanNorm() << endl;
      for ( j = 0; j < pointList.size(); j++ )
        fieldFilePtr << pointList.GetItem(j) << endl;
      pointList.clear();
      temporaryRayPath.pointList.clear();
    }
  }
  fieldFilePtr.close();
  matlabObject.reflections( *nonZeroRays, "reflection2.txt" );
  matlabObject.printRays( buildingObject, *nonZeroRays, "alldiff.txt" );

  nonZeroRays->clearAndClearSubList();
  validRayPath.clearAndClearSubList();
  reflectionPlanes.clear();
  diffractionInfo.clear();

  for ( i = 0; i < btsObject.numberOfReflections()+1; i++ )
    transmitterImages[i].clear();

  for ( i = 0; i < btsObject.numberOfReflections(); i++ )
    diffractionImages[i].listOfEdges.clear();

  //intrinsicObject.ClockTime( t0, "Time for ray-trace" );

  storageObject.DeleteImageList( transmitterImages );
  storageObject.DeleteEdgeList( diffractionImages );
}

CRayTracing::CRayTracing( const CBuilding&        buildingObject,
                          const CBaseTransceiver& btsObject,
                          const CPoint3d&         receiverPoint,
                          const mediaType&        media,
			        CComplexVector&   totalFieldPathLoss )
{
  List<CPlane>     reflectionPlanes;
  CReflection      reflectionObject;
  CDiffraction     diffractionObject;
  List<CRayPath>   validRayPath, *nonZeroRays;
  CRayPath         temporaryRayPath;
  List<CPointInfo> diffractionInfo, pointList;
  CEdgeList        *diffractionImages;
  List<CImage>     *transmitterImages;
  CComplexVector   fieldRayPathLoss[3], complexZeroValue;
  int              i, j, M, N, transConvexIndex, nonZeroCounter;
  CPoint3d         transmitterPoint;
  //CIntrinsic       intrinsicObject;
  CMatlab          matlabObject;
  ofstream         fieldFilePtr;
  CMemoryStorage   storageObject;
  CCorners         cornerDiffractionPoints;

  fieldFilePtr.open( "fieldlog.txt" );

  diffractionImages = storageObject.CreateEdgeList( btsObject.numberOfReflections() );
  transmitterImages = storageObject.CreateImageList( btsObject.numberOfReflections()+1 );

  nonZeroRays = new List<CRayPath>;
  //cout << "Ray-tracing Starting" << endl;
  // calculate possible reflection planes
  //reflectionPlanes  = buildingObject.calculateReflectionPlanes();
  buildingObject.calculateReflectionPlanes( reflectionPlanes );

  transmitterPoint = btsObject.position();
  buildingObject.locateConvexNumber( transmitterPoint, transConvexIndex );

  // create all images of the transmitter up to a given maximum order
  //t0 = intrinsicObject.StartClock();
  reflectionObject.CreateImagesOfTransmitter(
                       transmitterImages, buildingObject, btsObject,
                       reflectionPlanes, transmitterPoint, transConvexIndex );
  //intrinsicObject.ClockTime( t0, "Time to build point image tree" );

  // create diffraction image tree and return information about which
  // convex space and boundaries each edge touches.
  //t0 = intrinsicObject.StartClock();
  diffractionObject.createImagesOfDiffractionEdges( diffractionImages, 
                                    buildingObject, btsObject, diffractionInfo,
                                    reflectionPlanes, cornerDiffractionPoints );
  //intrinsicObject.ClockTime( t0, "Time to build edge diffraction tree" );
  
  matlabObject.edgeImages( diffractionImages[0] );

  // calculate all possible reflection paths first
  //t0 = intrinsicObject.StartClock();
  reflectionObject = CReflection( validRayPath,     transmitterImages,
                                  buildingObject, btsObject, transConvexIndex,
                                  transmitterPoint, receiverPoint );
  N = validRayPath.size();
  cout << "Number Of Valid Reflections: " << N-1 << endl;
  //intrinsicObject.ClockTime( t0, "Time for reflections" );
  // create files containing path information
  matlabObject.fullRayPaths( validRayPath );
  matlabObject.reflections(  validRayPath, "reflection.txt" );


  // then calculate all possible diffraction paths secondly
  //t0 = intrinsicObject.StartClock();
  if ( btsObject.numberOfDiffractions() > 0 ) 
    diffractionObject = CDiffraction( validRayPath, diffractionInfo,
                                      transmitterImages,diffractionImages, 
                                      cornerDiffractionPoints, buildingObject,
                                      btsObject, transConvexIndex, 
                                      transmitterPoint, receiverPoint );
  M = validRayPath.size();
  cout << "Number Of Valid Diffractions: " << M-N << endl;
  //intrinsicObject.ClockTime( t0, "Time for diffractions" );

  matlabObject.diffractions( validRayPath, N );

  cout << "Total Number Of Valid Rays including diffractions: " << M << endl;
  //cout << "Ray-tracing Ending" << endl;

  //areAllValidDiffractions( validRayPath, buildingObject, 
                           //diffractionImages[0].listOfEdges );
  nonZeroCounter     = 0;
  complexZeroValue   = CComplexVector( 0.0, 0.0, 0.0 );
  totalFieldPathLoss = complexZeroValue;
  //t0 = intrinsicObject.StartClock();
  /*for ( i = 0; i < M; i++ )
  {
    cout << "---------------Ray Path " << i << "-------------------" << endl;
    cout << validRayPath.GetItem(i).pointList.GetItem(1) << endl;
  }*/

  for ( i = 0; i < M; i++ )
  {
    //cout << "For Ray number Z component: " << i << endl;
    /*totalField( fieldRayPathLoss[0], validRayPath.GetItem(i),
                diffractionImages[0], cornerDiffractionPoints,
                buildingObject, btsObject, media, 
                X_POLARISATION );
    totalField( fieldRayPathLoss[1], validRayPath.GetItem(i),
                diffractionImages[0], cornerDiffractionPoints,
                buildingObject, btsObject, media, Y_POLARISATION );*/
    totalField( fieldRayPathLoss[2], validRayPath.GetItem(i), 
                diffractionImages[0], cornerDiffractionPoints,
                buildingObject, btsObject, media, Z_POLARISATION );
    cout << "field path loss [" << i << "]: " << fieldRayPathLoss[2] << endl;

    if ( fieldRayPathLoss[0] != complexZeroValue
      || fieldRayPathLoss[1] != complexZeroValue 
      || fieldRayPathLoss[2] != complexZeroValue )
    {
      /*if ( validRayPath.GetItem(i).numberOfDiffractions() == 1 )
        cout << "diffracted field: " << fieldRayPathLoss[2] << endl;
      else if ( validRayPath.GetItem(i).numberOfDiffractions() == 0 
             && validRayPath.GetItem(i).numberOfReflections() == 0 )
        cout << "incident field: " << fieldRayPathLoss[2] << endl;
      else
        cout << "reflected field: " << fieldRayPathLoss[2] << endl;*/

      nonZeroCounter++;
      pointList        = validRayPath.GetItem(i).pointList;
      temporaryRayPath = validRayPath.GetItem(i);
      temporaryRayPath = CRayPath( temporaryRayPath, fieldRayPathLoss );
      nonZeroRays->append( temporaryRayPath );
      totalFieldPathLoss += fieldRayPathLoss[0];
      totalFieldPathLoss += fieldRayPathLoss[1];
      totalFieldPathLoss += fieldRayPathLoss[2];
      fieldFilePtr << totalFieldPathLoss.euclideanNorm() << endl;
      for ( j = 0; j < pointList.size(); j++ )
        fieldFilePtr << pointList.GetItem(j) << endl;
      pointList.clear();
      temporaryRayPath.pointList.clear();
    }
  }
  fieldFilePtr.close();
  matlabObject.reflections( *nonZeroRays, "reflection2.txt" );
  matlabObject.printRays( buildingObject, *nonZeroRays, "alldiff.txt" );

  nonZeroRays->clearAndClearSubList();
  validRayPath.clearAndClearSubList();
  reflectionPlanes.clear();
  diffractionInfo.clear();

  for ( i = 0; i < btsObject.numberOfReflections()+1; i++ )
    transmitterImages[i].clear();

  for ( i = 0; i < btsObject.numberOfReflections(); i++ )
    diffractionImages[i].listOfEdges.clear();

  //intrinsicObject.ClockTime( t0, "Time for ray-trace" );

  storageObject.DeleteImageList( transmitterImages );
  storageObject.DeleteEdgeList( diffractionImages );
}

void CRayTracing::geometricalOptics( 
                          const CBuilding&        buildingObject,
                          const CBaseTransceiver& btsObject,
                          const CPoint3d&         receiverPoint )
{
  List<CPlane>     reflectionPlanes;
  CReflection      reflectionObject;
  CDiffraction     diffractionObject;
  List<CRayPath>   validRayPath, nonZeroRays;
  List<CPointInfo> diffractionInfo, pointList;
  CEdgeList        *diffractionImages;
  List<CImage>     *transmitterImages;
  CComplexVector   fieldRayPathLoss, complexZeroValue, totalFieldPathLoss;
  int              i, j, t0, M, N, transConvexIndex, nonZeroCounter;
  CPoint3d         transmitterPoint;
  CIntrinsic       intrinsicObject;
  CMatlab          matlabObject;
  ofstream         fieldFilePtr;
  CMemoryStorage   storageObject;
  CCorners         cornerDiffractionPoints;
  string           clockTimeOutputString;

  diffractionImages = storageObject.CreateEdgeList(
                                    btsObject.numberOfReflections() );
  transmitterImages = storageObject.CreateImageList(
                                    btsObject.numberOfReflections()+1 );

  fieldFilePtr.open( "fieldloggo.txt" );
  cout << "Ray-tracing Starting" << endl;
  // calculate possible reflection planes
  //reflectionPlanes  = buildingObject.calculateReflectionPlanes();
  buildingObject.calculateReflectionPlanes( reflectionPlanes );

  transmitterPoint = btsObject.position();
  buildingObject.locateConvexNumber( transmitterPoint,
                                                        transConvexIndex );

  // create all images of the transmitter up to a given maximum order
  t0 = intrinsicObject.StartClock();
  reflectionObject.CreateImagesOfTransmitter(
                       transmitterImages, buildingObject, btsObject,
                       reflectionPlanes, transmitterPoint,  transConvexIndex );
  //clockTimeOutputString = "Time to build point image tree";
  //intrinsicObject.ClockTime( t0, clockTimeOutputString ); 

  // create diffraction image tree and return information about which
  // convex space and boundaries each edge touches.
  t0 = intrinsicObject.StartClock();
  diffractionObject.createImagesOfDiffractionEdges( diffractionImages, 
                                    buildingObject, btsObject, diffractionInfo,
                                    reflectionPlanes, cornerDiffractionPoints );
  //clockTimeOutputString = "Time to build edge diffraction tree";
  //intrinsicObject.ClockTime( t0, clockTimeOutputString );
  
  matlabObject.edgeImages( diffractionImages[0] );

  // calculate all possible reflection paths first
  t0 = intrinsicObject.StartClock();
  reflectionObject = CReflection( validRayPath,     transmitterImages,
                                  buildingObject, btsObject, transConvexIndex,
                                  transmitterPoint, receiverPoint );
  N = validRayPath.size();
  //cout << "Number Of Valid Reflections: " << N << endl;
  clockTimeOutputString = "Time for reflections";
  intrinsicObject.ClockTime( t0, clockTimeOutputString );
  // create files containing path information
  matlabObject.fullRayPaths( validRayPath );
  matlabObject.reflections(  validRayPath, "reflection.txt" );

  matlabObject.printRays( buildingObject, validRayPath, "allref.txt" );

  // then calculate all possible diffraction paths secondly
  t0 = intrinsicObject.StartClock();
  if ( btsObject.numberOfDiffractions() > 0 ) 
    diffractionObject = CDiffraction( validRayPath, diffractionInfo,
                                      transmitterImages, diffractionImages,
                                      cornerDiffractionPoints, buildingObject, 
                                      btsObject, transConvexIndex,
                                      transmitterPoint, receiverPoint );
  M = validRayPath.size();
  //cout << "Number Of Valid Diffractions: " << M-N << endl;
  clockTimeOutputString = "Time for diffractions";
  intrinsicObject.ClockTime( t0, clockTimeOutputString );

  matlabObject.diffractions(  validRayPath, N );
  matlabObject.printRays( buildingObject, validRayPath, "alldiff.txt" );

  //cout << "Total Number Of Valid Rays: " << M << endl;
  cout << "Ray-tracing Ending" << endl;

  //areAllValidDiffractions( validRayPath, buildingObject, 
                           //diffractionImages[0].listOfEdges );
  nonZeroCounter     = 0;
  complexZeroValue   = CComplexVector( 0.0, 0.0, 0.0 );
  totalFieldPathLoss = complexZeroValue;
  t0 = intrinsicObject.StartClock();

  for ( i = 0; i < M; i++ )
  {
    //if ( validRayPath.GetItem(i).numberOfDiffractions() == 0 )
    geometricalOptics( fieldRayPathLoss, validRayPath.GetItem(i), 
                       buildingObject, btsObject, PEC );
    if ( fieldRayPathLoss != complexZeroValue )
    {
      nonZeroCounter++;
      pointList = validRayPath.GetItem(i).pointList;
      nonZeroRays.append( validRayPath.GetItem(i) );
      totalFieldPathLoss += fieldRayPathLoss;
      fieldFilePtr << fieldRayPathLoss.euclideanNorm() << endl;
      fieldFilePtr << fieldRayPathLoss << endl;
      fieldFilePtr << totalFieldPathLoss.euclideanNorm() << endl;
      for ( j = 0; j < pointList.size(); j++ )
        fieldFilePtr << pointList.GetItem(j) << endl;
      pointList.clear();
    }
  }
  fieldFilePtr.close();
  cout << "Single ray total pathloss value: " << totalFieldPathLoss << endl;
  cout << "number Of non-zero points: " << nonZeroCounter << endl;
  matlabObject.reflections( nonZeroRays, "reflection2.txt" );

  nonZeroRays.clearAndClearSubList();
  validRayPath.clearAndClearSubList();
  reflectionPlanes.clear();
  diffractionInfo.clear();

  for ( i = 0; i < btsObject.numberOfReflections()+1; i++ )
    transmitterImages[i].clear();

  for ( i = 0; i < btsObject.numberOfReflections(); i++ )
    diffractionImages[i].listOfEdges.clear();

  storageObject.DeleteImageList( transmitterImages );
  storageObject.DeleteEdgeList( diffractionImages );
  clockTimeOutputString = "Time for ray-trace";
  intrinsicObject.ClockTime( t0, clockTimeOutputString );
}

CRayTracing::CRayTracing( const CBuilding&        buildingObject,
                          const CBaseTransceiver& btsObject,
                                CPathLoss&        pathLossObject,
                                CPoint3d*         receiverPoints,
                          const mediaType&        media )
{
  List<CPlane>     reflectionPlanes;
  List<CRayPath>   validRayPath, nonZeroRays;
  List<CPointInfo> diffractionInfo;
  CComplexVector   **totalFieldPathLoss;
  CEdgeList        *diffractionImages;
  List<CImage>     *transmitterImages;
  ofstream         incidentOutput, reflectionOutput, diffractionOutput;
  ofstream         incidentreimOutput, scatteredreimOutput;

  int              i, j, t0, M, transConvexIndex, nonZeroCounter;
  //int              N;
  //int              k;
  CComplexVector   fieldRayPathLoss[3], complexZeroValue;
  CPoint3d         transmitterPoint;
  CComplexVector   Ei[3], Er[3], Es;

  CReflection      reflectionObject;
  CDiffraction     diffractionObject;
  CIntrinsic       intrinsicObject;
  CMemoryStorage   storageObject;
  CMatlab          matlabObject;
  ofstream         coutput;
  CCorners         cornerDiffractionPoints;
  complex          EiSum, EsSum, EtSum;

  diffractionImages = storageObject.CreateEdgeList( 
                                    btsObject.numberOfReflections() );
  transmitterImages = storageObject.CreateImageList(
                                    btsObject.numberOfReflections()+1 );
  

  coutput.open( "rayinfo.txt" );
  coutput << "position:   " << btsObject.position() << endl;
  coutput << "k:          " << btsObject.wavenumber() << endl;
  coutput << "Impedance:  " << Impedance << endl;
  coutput << "l:          " << Pi / btsObject.wavenumber() << endl;
  coutput << "frequency:  " << btsObject.frequency() << endl;
  coutput << "plate convex space: " << endl;
  coutput << buildingObject.convexArray_[2] << endl;
  coutput.close();

  incidentOutput.open( "incidentfield.txt" );
  incidentOutput.close();
  incidentOutput.open( "incidentfield.txt", ios::app );

  incidentreimOutput.open( "incidentreim.txt" );
  incidentreimOutput.close();
  incidentreimOutput.open( "incidentreim.txt", ios::app );

  reflectionOutput.open( "reflectionfield.txt" );
  reflectionOutput.close();
  reflectionOutput.open( "reflectionfield.txt", ios::app );

  diffractionOutput.open( "diffractionfield.txt" );
  diffractionOutput.close();
  diffractionOutput.open( "diffractionfield.txt", ios::app );

  scatteredreimOutput.open( "scatteredreim.txt" );
  scatteredreimOutput.close();
  scatteredreimOutput.open( "scatteredreim.txt", ios::app );

  cout << "Ray-tracing Starting" << endl;
  // calculate possible reflection planes
  //reflectionPlanes  = buildingObject.calculateReflectionPlanes();
  buildingObject.calculateReflectionPlanes( reflectionPlanes );

  transmitterPoint = btsObject.position();
  buildingObject.locateConvexNumber( transmitterPoint, transConvexIndex );

  // create all images of the transmitter up to a given maximum order
  t0 = intrinsicObject.StartClock();
  reflectionObject.CreateImagesOfTransmitter(
                       transmitterImages, buildingObject, btsObject,
                       reflectionPlanes, transmitterPoint,  transConvexIndex );
  //intrinsicObject.ClockTime( t0, "Time to build point image tree" );

  // create diffraction image tree and return information about which
  // convex space and boundaries each edge touches.
  t0 = intrinsicObject.StartClock();
  diffractionObject.createImagesOfDiffractionEdges( diffractionImages, 
                                    buildingObject, btsObject, diffractionInfo,
                                    reflectionPlanes, cornerDiffractionPoints );
  //intrinsicObject.ClockTime( t0, "Time to build edge diffraction tree" );
  
  totalFieldPathLoss = storageObject.CreateComplexVectorMatrix( 
                                     pathLossObject.numberOfPoints(), 3 );

  cout << "Calculating reflection:     0 of " << setw(4)
       << pathLossObject.numberOfPoints();
  t0 = intrinsicObject.StartClock();
  // calculate all possible reflection paths first
  for ( j = 0; j < pathLossObject.numberOfPoints(); j++ )
  {
    //t0 = intrinsicObject.StartClock();
    //cout << "doing j = " << j << endl;
    reflectionObject = CReflection( validRayPath,     transmitterImages,
                                    buildingObject, btsObject, transConvexIndex,
                                    transmitterPoint, receiverPoints[j] ); 
    //N = validRayPath.size();
    // create files containing path information
  
    // then calculate all possible diffraction paths secondly
    if ( btsObject.numberOfDiffractions() > 0 ) 
      diffractionObject = CDiffraction( validRayPath, diffractionInfo,
                                        transmitterImages, diffractionImages,
                                        cornerDiffractionPoints, buildingObject,
                                        btsObject, transConvexIndex, 
                                        transmitterPoint, receiverPoints[j] );
    //intrinsicObject.ClockTime( t0, "Time for diffractions" );

    M = validRayPath.size();
    //cout << "number of rays: " << M << endl;
  
    nonZeroCounter = 0;
    complexZeroValue = CComplexVector( 0.0, 0.0, 0.0 );
  
    for ( i = 0; i < M; i++ )
    {
      //if ( validRayPath.GetItem(i).numberOfDiffractions() == 0 )
      /*totalField( fieldRayPathLoss[0], validRayPath.GetItem(i),
                  diffractionImages[0], cornerDiffractionPoints,
                  buildingObject, btsObject, media, X_POLARISATION );
      totalField( fieldRayPathLoss[1], validRayPath.GetItem(i),
                  diffractionImages[0], cornerDiffractionPoints,
                  buildingObject, btsObject, media, Y_POLARISATION );*/
      totalField( fieldRayPathLoss[2], validRayPath.GetItem(i),
                  diffractionImages[0], cornerDiffractionPoints,
                  buildingObject, btsObject, media, Z_POLARISATION );

      /*for ( k = 0; k < validRayPath.GetItem(i).pointList.size() ; k++ )
        cout << validRayPath.GetItem(i).pointList.GetItem(k) << "  ";
	cout << endl;*/

      if ( validRayPath.GetItem(i).size() == 2 )
      {
        Ei[0] = fieldRayPathLoss[0];
        Ei[1] = fieldRayPathLoss[1];
        Ei[2] = fieldRayPathLoss[2];
        if ( j == 650 || j == 850 )
        {
          cout << "test: incident field: " << fieldRayPathLoss[2] << endl;
        }
      }

      if ( validRayPath.GetItem(i).size() == 3 
        && validRayPath.GetItem(i).numberOfReflections() == 1 )
      {
        Er[0] = fieldRayPathLoss[0];
        Er[1] = fieldRayPathLoss[1];
        Er[2] = fieldRayPathLoss[2];
        //fieldRayPathLoss[2] = fieldRayPathLoss[2] * 0.5;
        if ( j == 650  || j == 850)
        {
          cout << "test: reflected field: " << fieldRayPathLoss[2] << endl;
        }
      }

      if ( fieldRayPathLoss[2] != complexZeroValue )
      {
        if ( ( j == 650 || j == 850 ) 
	  && validRayPath.GetItem(i).numberOfDiffractions() == 1 )
        {
          cout << "test: diffracted field: " << fieldRayPathLoss[2] << endl;
        }
        nonZeroCounter++;
        nonZeroRays.append( validRayPath.GetItem(i) );
        totalFieldPathLoss[j][0] += fieldRayPathLoss[0];
        totalFieldPathLoss[j][1] += fieldRayPathLoss[1];
        totalFieldPathLoss[j][2] += fieldRayPathLoss[2];
      }

      if ( i == M-1 )
      {
        diffractionOutput << receiverPoints[j]   << "  " 
                          << totalFieldPathLoss[j][0] - Ei[0] - Er[0] << "  " 
                          << totalFieldPathLoss[j][1] - Ei[1] - Er[1] << "  " 
                          << totalFieldPathLoss[j][2] - Ei[2] - Er[2] << endl; 
        reflectionOutput << receiverPoints[j]   << "  " 
                         << Er[0] << "  " << Er[1] << "  " << Er[2] << endl; 
        incidentOutput << receiverPoints[j]   << "  " 
                         << Ei[0] << "  " << Ei[1] << "  " << Ei[2] << endl; 

        /*EiReal = Ei[0].real() + Ei[1].real() + Ei[2].real(); 
        EiImag = Ei[0].imag() + Ei[1].imag() + Ei[2].imag(); 
        totalReal = totalFieldPathLoss[0].real() + totalFieldPathLoss[1].real() + totalFieldPathLoss[2].real();
        totalImag = totalFieldPathLoss[0].imag() + totalFieldPathLoss[1].imag() + totalFieldPathLoss[2].imag();*/
        EiSum = Ei[2].x * Ei[2].x + Ei[2].y * Ei[2].y + Ei[2].z * Ei[2].z;
        Es    = totalFieldPathLoss[j][2] - Ei[2];
        EsSum = Es.x * Es.x + Es.y * Es.y + Es.z * Es.z;
         
        /*scatteredreimOutput << receiverPoints[j] << "  " << sqrt(totalReal - EsReal) 
                                                 << "  " << sqrt(totalReal - EsImag) << endl;*/

        scatteredreimOutput << receiverPoints[j] << "  " << CMaths::csqrt(EsSum).real() * 0.01
                                                 << "  " << CMaths::csqrt(EsSum).imag() * 0.01 << endl;
        incidentreimOutput << receiverPoints[j]  << "  " << CMaths::csqrt(EiSum).real() * 0.01
                                                 << "  " << CMaths::csqrt(EiSum).imag() * 0.01 << endl;

    if ( j == 650 || j == 850 )
    {
      cout << "test: total field: " << totalFieldPathLoss[j][2] << endl;
    }

        Er[0] = Ei[0] = CComplexVector();
        Er[1] = Ei[1] = CComplexVector();
        Er[2] = Ei[2] = CComplexVector();
      }
    }


    if ( (j % 100) == 0 )
      cerr << setw(4) << j << " of " << setw(4)
           << pathLossObject.numberOfPoints() 
           << "  " << receiverPoints[j] <<endl;
    nonZeroRays.clearAndClearSubList();
    validRayPath.clearAndClearSubList();
  }
  matlabObject.fieldRayPathLoss( totalFieldPathLoss, pathLossObject,
                                 "pathloss.txt", receiverPoints );

  // delete created arrays or lists
  storageObject.DeleteComplexVectorMatrix( totalFieldPathLoss, 
                                           pathLossObject.numberOfPoints() );
  reflectionPlanes.clear();
  diffractionInfo.clear();

  for ( i = 0; i < btsObject.numberOfReflections()+1; i++ )
    transmitterImages[i].clear();

  for ( i = 0; i < btsObject.numberOfReflections(); i++ )
    diffractionImages[i].listOfEdges.clear();

  //intrinsicObject.ClockTime( t0, "Time for ray-trace" );

  reflectionOutput.close();
  diffractionOutput.close();
  scatteredreimOutput.close();
  incidentOutput.close();
  incidentreimOutput.close();
  storageObject.DeleteImageList( transmitterImages );
  storageObject.DeleteEdgeList( diffractionImages );
  cout << endl << "Ray-tracing Ending" << endl;
  intrinsicObject.ClockTime( t0, "Time for ray-trace" );
}

void CRayTracing::geometricalOptics( const CBuilding&        buildingObject,
                                     const CBaseTransceiver& btsObject,
                                           CPathLoss&        pathLossObject,
                                           CPoint3d*         receiverPoints,
                                           int               height )
{
  List<CPlane>     reflectionPlanes;
  List<CRayPath>   validRayPath, nonZeroRays;
  List<CPointInfo> diffractionInfo;
  CComplexVector   **totalFieldPathLoss;
  CEdgeList        *diffractionImages;
  List<CImage>     *transmitterImages;

  int              i, j, t0, M, transConvexIndex, nonZeroCounter;
  //int            N;
  CComplexVector   fieldRayPathLoss[3], complexZeroValue;
  CPoint3d         transmitterPoint;

  CReflection      reflectionObject;
  CDiffraction     diffractionObject;
  CIntrinsic       intrinsicObject;
  CMemoryStorage   storageObject;
  CMatlab          matlabObject;
  CCorners         cornerDiffractionPoints;

  diffractionImages = storageObject.CreateEdgeList( btsObject.numberOfReflections() );
  transmitterImages = storageObject.CreateImageList( btsObject.numberOfReflections()+1 );

  cout << "Ray-tracing Starting" << endl;
  // calculate possible reflection planes
  //reflectionPlanes  = buildingObject.calculateReflectionPlanes();
  buildingObject.calculateReflectionPlanes( reflectionPlanes );

  transmitterPoint = btsObject.position();
  buildingObject.locateConvexNumber( transmitterPoint,
                                                        transConvexIndex );

  // create all images of the transmitter up to a given maximum order
  t0 = intrinsicObject.StartClock();
  reflectionObject.CreateImagesOfTransmitter(
                       transmitterImages, buildingObject, btsObject,
                       reflectionPlanes, transmitterPoint, transConvexIndex );
  //intrinsicObject.ClockTime( t0, "Time to build point image tree" );

  // create diffraction image tree and return information about which
  // convex space and boundaries each edge touches.
  t0 = intrinsicObject.StartClock();
  diffractionObject.createImagesOfDiffractionEdges( diffractionImages, 
                                    buildingObject, btsObject, diffractionInfo,
                                    reflectionPlanes, cornerDiffractionPoints );
  //intrinsicObject.ClockTime( t0, "Time to build edge diffraction tree" );
  
  totalFieldPathLoss = storageObject.CreateComplexVectorMatrix( 
                                     pathLossObject.numberOfPoints(), 3 );

  cout << "Calculating reflection:     0 of " << setw(4)
       << pathLossObject.numberOfPoints();
  t0 = intrinsicObject.StartClock();
  // calculate all possible reflection paths first
  for ( j = 0; j < pathLossObject.numberOfPoints(); j++ )
  {
    //t0 = intrinsicObject.StartClock();
    reflectionObject = CReflection( validRayPath,     transmitterImages,
                                    buildingObject, btsObject, transConvexIndex,
                                    transmitterPoint, receiverPoints[j] ); 
    //N = validRayPath.size();
    //cout << "Number Of Valid Reflections: " << N << endl;
    // create files containing path information
  
    // then calculate all possible diffraction paths secondly
    if ( btsObject.numberOfDiffractions() > 0 ) 
      diffractionObject = CDiffraction( validRayPath, diffractionInfo,
                                        transmitterImages, diffractionImages,
                                        cornerDiffractionPoints, buildingObject,
                                        btsObject, transConvexIndex,
                                        transmitterPoint, receiverPoints[j] );
    //intrinsicObject.ClockTime( t0, "Time for diffractions" );

    M = validRayPath.size();
  
    nonZeroCounter = 0;
    complexZeroValue = CComplexVector( 0.0, 0.0, 0.0 );
  
    for ( i = 0; i < M; i++ )
    {
      geometricalOptics( fieldRayPathLoss[2], validRayPath.GetItem(i),
                         buildingObject, btsObject, PEC );
      if ( fieldRayPathLoss[2] != complexZeroValue )
      {
        nonZeroCounter++;
        nonZeroRays.append( validRayPath.GetItem(i) );
        totalFieldPathLoss[j][0] += fieldRayPathLoss[0];
        totalFieldPathLoss[j][1] += fieldRayPathLoss[1];
        totalFieldPathLoss[j][2] += fieldRayPathLoss[2];
      }
    }

    if ( (j % 100) == 0 )
      cerr << setw(4) << j << " of " << setw(4)
           << pathLossObject.numberOfPoints() 
           << "  " << receiverPoints[j] <<endl;
    nonZeroRays.clearAndClearSubList();
    validRayPath.clearAndClearSubList();
  }
  matlabObject.fieldRayPathLoss( totalFieldPathLoss, pathLossObject,
                                 "gopathloss.txt", receiverPoints );

  // delete created arrays or lists
  storageObject.DeleteComplexVectorMatrix( totalFieldPathLoss, 
                                           pathLossObject.numberOfPoints() );
  reflectionPlanes.clear();
  diffractionInfo.clear();

  for ( i = 0; i < btsObject.numberOfReflections()+1; i++ )
    transmitterImages[i].clear();

  for ( i = 0; i < btsObject.numberOfReflections(); i++ )
    diffractionImages[i].listOfEdges.clear();

  //intrinsicObject.ClockTime( t0, "Time for ray-trace" );

  storageObject.DeleteImageList( transmitterImages );
  storageObject.DeleteEdgeList( diffractionImages );

  cout << endl << "Ray-tracing Ending" << endl;
  intrinsicObject.ClockTime( t0, "Time for ray-trace" );
}

void CRayTracing::areAllValidDiffractions( List<CRayPath>& rayPathList,
                                    const  CBuilding&      buildingObject,
                                           List<CEdge>&    diffractionEdges )
{
  int              i, j, k;
  CRayPath         rayPath;
  CPointInfo       pointInfo;
  List<CPointInfo> pointList;
  CSegment3d       v1, v2, e1, e2;

  for ( i = 0; i < rayPathList.size(); i++ )
  {
    rayPath = rayPathList.GetItem(i);
    if ( rayPath.numberOfDiffractions() > 0 )
    {
      pointList = rayPath.pointList;
      for ( j = 0; j < pointList.size(); j++ )
      {
        pointInfo = pointList.GetItem(j);

        if ( pointInfo.contact == DIFFRACTION )
        {
          for ( k = 0; k < diffractionEdges.size(); k++ )
            if (diffractionEdges.GetItem(k).edge_.intersection(pointInfo.node))
              break;

          v1 = CSegment3d( pointInfo.node, pointList.GetItem(j-1).node );
          v2 = CSegment3d( pointInfo.node, pointList.GetItem(j+1).node );
          e1 = diffractionEdges.GetItem(k).edge_;
          e2 = diffractionEdges.GetItem(k).edge_.reverse();

          cout << pointList.GetItem(j-1).node << "  " 
               << pointList.GetItem(j+1).node << endl;
          cout << pointInfo.node << " is in " << e1 << endl;
          cout << "angles: " << e1.angle( v1 ) << " " << e2.angle( v2 ) << endl;
        }
      }
    }
  }
  pointList.clear();
}

void CRayTracing::totalField( CComplexVector&    Et,
                              CRayPath&          rayPath,
                              CEdgeList&         diffractionImages,
                              CCorners&          cornerDiffractionPoints,
                        const CBuilding&         buildingObject,
                        const CBaseTransceiver&  btsObject,
                        const mediaType&         media,
                        const polarisationType&  polarisation )
{
  List<CPointInfo> pointList, reflectionPath;
  int              i, k, currentIndex;
  double           pathLength, totalPathLength;
  //double           k1;
  boolean          isValid;
  CSegment3d       diffractingEdge, si, sr;
  CPointInfo       startPoint, reflectionPoint, nextPoint, previousPoint;
  CPoint3d         receiver;
  CMemoryStorage   storageObject;
  int              *refOrDiffIndex;

  pointList = rayPath.pointList;
  k         = pointList.size();
  //k1        = btsObject.wavenumber();
  //cout << "---------------------------" << endl;
  //for ( i = 0; i < k; i++ )
    //cout << rayPath.pointList.GetItem(i) << endl;

  // add two to allow for transmitter and receiver
  refOrDiffIndex = storageObject.CreateIntegerVector( 
                                  rayPath.numberOfReflections() 
                                + rayPath.numberOfDiffractions() + 2 );
  currentIndex    = 0;
  totalPathLength = 0;
  receiver        = rayPath.pointList.GetItem( rayPath.pointList.size() - 1 ).node;

  for ( i = 0; i < k; i++ )
  {
    previousPoint = nextPoint;

    nextPoint = pointList.GetItem(i);
    if ( nextPoint.contact != TRANSMISSION )
    {
      reflectionPath.append(nextPoint);
      refOrDiffIndex[currentIndex] = i;
      currentIndex++;
    }
    if ( i != 0 )
      totalPathLength += (nextPoint.node).abs( previousPoint.node );
  }
  //cout << "number Of points in new list" << pointList.size() << endl;
  //for ( i = 0; i < k; i++ )
    //cout << pointList.GetItem(i) << endl;

  if ( rayPath.numberOfReflections() == 0 )
  {
    startPoint      = reflectionPath.GetItem(0);
    nextPoint       = reflectionPath.GetItem(1);
    si              = CSegment3d( startPoint.node, nextPoint.node );

    if ( btsObject.hertzianDipole() == true )
    {
      Et = incidentFieldHertzian( si, polarisation, btsObject );
      //cout << Et << endl;
      //cout << si << endl;
    }
    else
      Et = incidentField( si, polarisation, btsObject );
    //cout << nextPoint.node << endl;
    //if ( nextPoint.node == CPoint3d( -20.0, 0.0, 20.0 ) )
    //{
    //  cout << Et << endl;
    //  cout << si << endl;
    //  exit(1);
    //}

    //cout << "Ei(0):    " << Et << endl;
    //cout << "silength: " << si.length() << endl;
                         
    //isValid = transmissionCoefficient( Et, pointList, si, btsObject, 
                                       //buildingObject, refOrDiffIndex[0]+1,
                                       //refOrDiffIndex[1]-1, media );
    transmissionCoefficient( Et, pointList, si, btsObject, 
                                       buildingObject, refOrDiffIndex[0]+1,
                                       refOrDiffIndex[1]-1, media );
    if ( nextPoint.contact == DIFFRACTION  //)
      || nextPoint.contact == VIRTUAL_DIFFRACTION )
    {
      reflectionPoint = nextPoint;
      nextPoint       = reflectionPath.GetItem(2);
      si              = CSegment3d( startPoint.node, reflectionPoint.node );
      sr              = CSegment3d( reflectionPoint.node, nextPoint.node  );
      diffractingEdge = diffractionImages.listOfEdges.GetItem( 
                                        reflectionPoint.edgeNumber ).edge_;
      isValid = diffractionCoefficient( Et, si, sr, si.length(), 
                                        totalPathLength, reflectionPoint, 
                                        diffractingEdge, buildingObject, 
                                        btsObject, media );
      //isValid = cornerDiffraction( Et, si, sr, si.length(), 
                                   //totalPathLength, reflectionPoint, 
                                   //diffractingEdge, buildingObject, 
                                   //btsObject, media, cornerDiffractionPoints,
				   //polarisation );
      /*cornerDiffraction( Et, si, sr, si.length(), 
                                   totalPathLength, reflectionPoint, 
                                   diffractingEdge, buildingObject, 
                                   btsObject, media, cornerDiffractionPoints,
				   polarisation );*/
      cout << "lengths: " << si << " " << sr << endl;
      cout << "Ed(0):    " << Et << endl;
    }
    /*else if ( reflectionPoint.contact == VIRTUAL_DIFFRACTION )
    {
      Et = CComplexVector();
    }*/
    //if ( isValid == true )
      //cout << "transmission:     " << Et << endl;
  }
  else
  {
    pathLength = 0;
    for ( i = 0; i < rayPath.numberOfReflections(); i++ )
    {
      startPoint      = reflectionPath.GetItem(i);
      reflectionPoint = reflectionPath.GetItem(i+1);
      nextPoint       = reflectionPath.GetItem(i+2);
      si              = CSegment3d( startPoint.node, reflectionPoint.node );
      pathLength     += si.length();
      sr              = CSegment3d( reflectionPoint.node, nextPoint.node  );
  
      // first calculate the incident field to the first reflection point
      if ( i == 0 )
      {
        //cout << "Ei(1) before: " << Et << endl;
        if ( btsObject.hertzianDipole() == true )
          Et = incidentFieldHertzian( si, polarisation, btsObject );
        else
          Et = incidentField( si, polarisation, btsObject );
        /*cout << "inci (" << rayPath.numberOfReflections() 
             << ", " << rayPath.numberOfDiffractions()
             << ", " << rayPath.numberOfTransmissions()
             << " )   Et = " << Et.euclideanNorm() << endl;*/
        //cout << "Ei(1): after" << Et << endl;
        //cout << "silength: " << si.length() << endl;
      }
  
      // we need information about the convex space that the reflection
      // point is contained in.
      currentIndex = refOrDiffIndex[i+1];
  
      // amend the incident field to allow for transmissions before
      // reflection point
      //cout << "Et(1): before" << Et << endl;
      //isValid = transmissionCoefficient( Et, pointList, si, btsObject, 
                                         //buildingObject, refOrDiffIndex[i]+1,
                                         //refOrDiffIndex[i+1]-1, media );
      transmissionCoefficient( Et, pointList, si, btsObject, 
                                         buildingObject, refOrDiffIndex[i]+1,
                                         refOrDiffIndex[i+1]-1, media );
      //cout << "Et(1): after" << Et << endl;
      //if ( isValid == true )
        //cout << "transmission:     " << Et << endl;
  
      // next calculate the reflection coefficient 
      if ( reflectionPoint.contact == REFLECTION )
      {
        //cout << "Er(1): before" << Et << endl;
        reflectionCoefficient( Et, si, sr, pathLength, 
                                         reflectionPoint, buildingObject,
                                         btsObject, media );
        //isValid = reflectionCoefficient( Et, si, sr, pathLength, 
                                         //reflectionPoint, buildingObject,
                                         //btsObject, media );
        //cout << "Er(1): after" << Et << endl;
      }
      else if ( reflectionPoint.contact == DIFFRACTION  //)
             || reflectionPoint.contact == VIRTUAL_DIFFRACTION )
      {
        diffractingEdge = diffractionImages.listOfEdges.GetItem( 
                                          reflectionPoint.edgeNumber ).edge_;
        cout << "Ed(1): before" << Et << endl;
        isValid = diffractionCoefficient( Et, si, sr, pathLength,
                                          totalPathLength,
                                          reflectionPoint, diffractingEdge, 
                                          buildingObject, btsObject, media );
        //isValid = cornerDiffraction( Et, si, sr, si.length(), 
                                   //totalPathLength, reflectionPoint, 
                                   //diffractingEdge, buildingObject, 
                                   //btsObject, media, cornerDiffractionPoints,
                                   //polarisation );
        /*cornerDiffraction( Et, si, sr, si.length(), 
                                   totalPathLength, reflectionPoint, 
                                   diffractingEdge, buildingObject, 
                                   btsObject, media, cornerDiffractionPoints,
                                   polarisation );*/

        // the diffraction point becomes the new source 
        // future attenuation factors

        totalPathLength -= pathLength;
        pathLength       = 0;
        cout << "Ed(1): " << Et << endl;
      }
      /*else if ( reflectionPoint.contact == VIRTUAL_DIFFRACTION )
      {
        Et = CComplexVector();
      }*/
      else
      {
        cout << "Wrong computation: " << reflectionPoint.contact << endl;
        cout << "Should have been DIFFRACTION or REFLECTION" << endl;
        exit(1);
      }
      //if ( isValid == true )
        //cout << "reflection:       " << Et << endl;
    }
    // Finish calculation by seeing if transmission points occur between
    // last reflection point and receiver.
    si      = sr;
    //cout << "Et2(1): before " << Et << endl;
    transmissionCoefficient( Et, pointList, si, btsObject, 
                                       buildingObject, refOrDiffIndex[i]+1,
                                       refOrDiffIndex[i+1]-1, media );
    //cout << "Et2(1): after " << Et << endl;
    //if ( isValid == true )
      //cout << "transmission:     " << Et << endl;
  }
  
  // delete any created temporary lists
  storageObject.DeleteIntegerVector( refOrDiffIndex );
  pointList.clear();
  reflectionPath.clear();
}

void CRayTracing::geometricalOptics( 
                              CComplexVector&   Et,
                              CRayPath&         rayPath,
                        const CBuilding&        buildingObject,
                        const CBaseTransceiver& btsObject,
                        const mediaType&        media )
{
  List<CPointInfo> pointList, reflectionPath;
  int              i, k, currentIndex;
  //double           k1;
  CSegment3d       diffractingEdge, si, sr, siimage;
  CPointInfo       startPoint, endPoint, reflectionPoint, nextPoint;
  CMemoryStorage   storageObject;
  CPoint3d         unitIncInXY, zhat, xhat;
  int              *refOrDiffIndex;

  pointList = rayPath.pointList;
  k         = pointList.size();
  //k1        = btsObject.wavenumber();

  // add two to allow for transmitter and receiver
  refOrDiffIndex = storageObject.CreateIntegerVector( 
                                  rayPath.numberOfReflections() 
                                + rayPath.numberOfDiffractions() + 2 );
  currentIndex = 0;
  for ( i = 0; i < k; i++ )
  {
    nextPoint = pointList.GetItem(i);
    if ( nextPoint.contact != TRANSMISSION )
    {
      reflectionPath.append(nextPoint);
      refOrDiffIndex[currentIndex] = i;
      currentIndex++;
    }
  }

  if ( rayPath.numberOfReflections() == 0 )
  {
    startPoint    = reflectionPath.GetItem(0);
    nextPoint     = reflectionPath.GetItem(1);
    si            = CSegment3d( startPoint.node, nextPoint.node );

    if ( btsObject.hertzianDipole() == true )
    {
      Et = incidentFieldHertzian( si, Z_POLARISATION, btsObject );
    }
    else
      Et = incidentField( si, Z_POLARISATION, btsObject );
  }
  else
  {
    // using the reflection point and the receiver we can 
    // position the image of the transmitter.
    startPoint    = reflectionPath.GetItem(0);
    nextPoint     = reflectionPath.GetItem(1); 
    endPoint      = reflectionPath.GetItem(2); 
    si            = CSegment3d( startPoint.node, nextPoint.node );

    startPoint.node.z = 2.0 * nextPoint.node.z - startPoint.node.z;
    siimage           = CSegment3d( startPoint.node, endPoint.node );
      
    if ( btsObject.hertzianDipole() == true )
      Et = incidentFieldHertzian( siimage, Z_POLARISATION, btsObject );
    else
      Et = incidentField( siimage, Z_POLARISATION, btsObject );
  }
  
  // delete any created temporary lists
  storageObject.DeleteIntegerVector( refOrDiffIndex );
  pointList.clear();
  reflectionPath.clear();
}

boolean CRayTracing::transmissionCoefficient( 
                                        CComplexVector&   Ei,
                                        List<CPointInfo>& pointList,
                                        CSegment3d&       si,
                                  const CBaseTransceiver& btsObject,
                                  const CBuilding&        buildingObject,
                                  const int&              firstTransmission,
                                  const int&              lastTransmission,
                                  const mediaType&        media )
{
  int            i, j, k;
  double         k1, d,cosTheta_i, sinSquaredTheta_i, lengthInMedia;
  //double         Eibefore;
  complex        k2, sqrtTerm, permittivity, l, t, Pd, Pt;
  CPoint3d       point1, point2;
  CPointInfo     pointInfo1, pointInfo2;
  CConvex        *convexSpace1, *convexSpace2;
  CBoundary      *boundary1;
  //CBoundary      *boundary2;
  boolean        isValid;
  
  if ( media == PEC && lastTransmission - firstTransmission >= 0 )
  {
    Ei = CComplexVector( 0.0, 0.0, 0.0 );
    //cout << "oh no, set to zero" << endl;
    return true;
  }

  for ( i = firstTransmission; i <= lastTransmission; i++ )
  {
    pointInfo1 = pointList.GetItem(i);

    if ( pointInfo1.contact != TRANSMISSION )
    {
      cout << "Error: not a transmission in CRayTracing.cpp" << endl;
      cout << "First index: " << firstTransmission << endl;
      cout << " and less than or equal to: " << lastTransmission << endl;

      if ( pointInfo1.contact == REFLECTION )
        cout << "Type: REFLECTION " << endl;
      else if ( pointInfo1.contact == DIFFRACTION )
        cout << "Type: DIFFRACTION " << endl;
      else if ( pointInfo1.contact == VIRTUAL_DIFFRACTION )
        cout << "Type: VIRTUAL_DIFFRACTION " << endl;

      cout << "Exiting" << endl;
      exit(1);
    }

    convexSpace1  = &buildingObject.convexArray_[pointInfo1.convexNumber];
    j             = pointInfo1.boundaryNumber1;
    isValid       = convexSpace1->notIntersection( pointInfo1.node, si.start(),
                                                   point2, k );
    point1 = pointInfo1.node;
    if ( k != 5 - j ) // if not opposite
    {
      if ( convexSpace1->boundaryWithLargestSurfaceArea(j) )
      {
        convexSpace1->facet[5-j].planeIntersection(si.start(),
                                                   si.end(), point2 );
        // change to opposite of j
        k = 5-j;
      }
      else 
      {
        convexSpace1->facet[5-k].planeIntersection(si.start(),
                                                   si.end(), point1 );
        j = 5-k;
      }
        // else change to opposite of k
    }

    if ( i+1 <= lastTransmission )
    {
      pointInfo2 = pointList.GetItem(i+1);
      convexSpace2  = &buildingObject.convexArray_[pointInfo2.convexNumber];
      if ( convexSpace2->intersection( pointInfo1.node ) )
        i++;
    }

    //if ( isValid == false )
    //{
      //cout << "Line enters and leaves convex space at same point" << endl;
      //cout << "implies no transmission coefficient contribution to field" << endl;
      //cout << pointInfo1.node << endl;
    //}
    //else
    if ( isValid == true )
    {
      boundary1     = &convexSpace1->facet[k];
      //boundary2     = &convexSpace1->facet[j];
      lengthInMedia = point1.abs( point2 );
      cosTheta_i    = boundary1->cosAngle( si.reverse() );
      d             = cosTheta_i * lengthInMedia;
  
      permittivity      = convexSpace1->complexPermittivity( btsObject );
      sinSquaredTheta_i = 1.0 - qsqr( cosTheta_i );
      sqrtTerm          = CMaths::csqrt( permittivity - sinSquaredTheta_i );
      k1                = btsObject.wavenumber();
      k2                = k1 * CMaths::csqrt( permittivity );
  
      l = d * CMaths::csqrt( permittivity ) / sqrtTerm;
      t = d * ( cosTheta_i * sqrtTerm + sinSquaredTheta_i ) / sqrtTerm;
      Pd = CMaths::expminusj( k2 * l );
      Pt = CMaths::expj( k1 * t );
  
      //Eibefore = Ei.euclideanNorm();
      transmissionThroughDielectric( Ei, si, *convexSpace1, *boundary1,
                                     btsObject, Pd, Pt, media );
      randomVariable( btsObject, Ei );
    }
  }

  if ( lastTransmission - firstTransmission >= 0 )
    return true;

  return false;
}

void CRayTracing::randomVariable( const CBaseTransceiver& btsObject, 
                                        CComplexVector&   Ei )
{
  //cout << "randomVariance: " << btsObject.randomVariance() << endl;
  if ( btsObject.randomVariance() != 0.0 )
  {
    time_t ltime; 
    long int Seed;
    complex  x;
    double   w; 
        //ofstream coutput( "random.txt",ios::app );

    if ( firstSeed == 1 )
    { 
      time( &ltime );
      Seed=ltime;
      firstSeed=0;
      srand( Seed );
    }
        
    //cout << oldSeed << endl;
   
    do 
    {
      x = complex( 2.0 * double(rand())/double(RAND_MAX) - 1.0,
                   2.0 * double(rand())/double(RAND_MAX) - 1.0 );
      w = qsqr( x.real() ) + qsqr( x.imag() );
    } while ( w >= 1.0 );

    w  = sqrt( (-2.0 * log( w ) ) * btsObject.randomVariance() / w );
    //cout << "before: " << Ei << endl;
    Ei = Ei * ( x * w + 1.0 );
    //cout << "after: " << Ei << endl;
    //coutput << x * w << endl;
  }
}

void CRayTracing::transmissionThroughDielectric( 
                                            CComplexVector&   Ei,
                                      const CSegment3d&       si,
                                      const CConvex&          convexSpace,
                                      const CBoundary&        boundary,
                                      const CBaseTransceiver& btsObject,
                                            complex           Pd,
                                            complex           Pt,
                                      const mediaType&        media )
{
  CPoint3d       nhat, sihat, srhat;
  CPoint3d       ei_perpendicular, ei_parallel;
  double         cosTheta_i;
  //double         p, k1;
  complex        Tparallel, Tperpendicular, sqrtTerm;
  complex        Rparallel, Rperpendicular;
  complex        E_iparallel, E_iperpendicular, permittivity;

  // define ray-based orthogonal basis.
  sihat            = si.unitVector();
  nhat             = boundary.equation_.outwardNormal_;
  ei_perpendicular = sihat ^ nhat;

  if ( ei_perpendicular == CPoint3d( 0.0, 0.0, 0.0 ) )
  {
    sihat.orthogonalVectors( ei_perpendicular, ei_parallel );
  }
  else
  {
    ei_perpendicular = ei_perpendicular.unitVector();
    ei_parallel      = sihat ^ ei_perpendicular;
  }

  // define unit vectors and lengths along the ray
  //p                = si.length();
  //k1               = btsObject.wavenumber();

  // convert incident field into the local coordinate system
  E_iparallel      = Ei * ei_parallel;
  E_iperpendicular = Ei * ei_perpendicular;

  // define the transmission matrix for different dielectric types
  if ( media == LOSSLESS )
    permittivity   = convexSpace.permittivity_;
  else
    permittivity   = convexSpace.complexPermittivity( btsObject );

  if ( media == PEC )
  {
    Tparallel      = 0.0;
    Tperpendicular = 0.0;
  }
  else// if ( media == LOSSLESS )
  {
    cosTheta_i     = sihat * nhat * -1.0;
    if ( cosTheta_i < 0.0 )
    { 
      cout << "cos theta is negative" << endl;
      cout << sihat << endl;
      cout << nhat << endl;
      cout << si << endl;
    }
    sqrtTerm       = CMaths::csqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Rparallel      = ( -permittivity * cosTheta_i + sqrtTerm )
                   / (  permittivity * cosTheta_i + sqrtTerm );
    Rperpendicular = ( cosTheta_i - sqrtTerm ) / ( cosTheta_i + sqrtTerm );
    Tparallel      = ( complex(1.0) - qsqr( Rparallel ) ) * Pd * Pt;
    Tperpendicular = ( complex(1.0) - qsqr( Rperpendicular ) ) * Pd * Pt;
  }

  // multiply out all terms to give the electric field
  E_iperpendicular *= Tperpendicular;
  E_iparallel      *= Tparallel;

  // convert back into global coordinate system
  Ei = CComplexVector( ei_perpendicular ) * E_iperpendicular 
     + CComplexVector( ei_parallel ) * E_iparallel;
}


void CRayTracing::transmissionFreeSpaceToDielectric( 
                                            CComplexVector&   Ei,
                                      const CSegment3d&       si,
                                      const CConvex&          convexSpace,
                                      const CBoundary&        boundary,
                                      const CBaseTransceiver& btsObject,
                                      const mediaType&        media )
{
  CPoint3d       nhat, sihat, srhat;
  CPoint3d       ei_perpendicular, ei_parallel;
  //double         k1, p;
  double         cosTheta_i;
  complex        Tparallel, Tperpendicular, sqrtTerm;
  complex        E_iparallel, E_iperpendicular, permittivity;

  // define ray-based orthogonal basis.
  sihat            = si.unitVector();
  nhat             = boundary.equation_.outwardNormal_;
  ei_perpendicular = sihat ^ nhat;

  if ( ei_perpendicular == CPoint3d( 0.0, 0.0, 0.0 ) )
  {
    sihat.orthogonalVectors( ei_perpendicular, ei_parallel );
  }
  else
  {
    ei_perpendicular = ei_perpendicular.unitVector();
    ei_parallel      = sihat ^ ei_perpendicular;
  }

  // define unit vectors and lengths along the ray
  //p                = si.length();
  //k1               = btsObject.wavenumber();

  // convert incident field into the local coordinate system
  E_iparallel      = Ei * ei_parallel;
  E_iperpendicular = Ei * ei_perpendicular;

  // define the transmission matrix for different dielectric types
  if ( media == PEC )
  {
    Tparallel      = 0.0;
    Tperpendicular = 0.0;
  }
  else if ( media == LOSSLESS )
  {
    permittivity   = convexSpace.permittivity_;
    cosTheta_i     = sihat * nhat * -1.0;
    if ( cosTheta_i < 0.0 )
    { 
      cout << "cos theta is negative" << endl;
      cout << sihat << endl;
      cout << nhat << endl;
      cout << si << endl;
    }
    sqrtTerm       = CMaths::csqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Tparallel      = 2.0 * CMaths::csqrt(permittivity) * cosTheta_i 
                   / ( permittivity * cosTheta_i + sqrtTerm );
    Tperpendicular =  complex( 2.0 * cosTheta_i ) / ( cosTheta_i + sqrtTerm );
  }
  else
  {
    permittivity   = convexSpace.complexPermittivity( btsObject );
    cosTheta_i     = sihat * nhat * -1.0;
    sqrtTerm       = CMaths::csqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Tparallel      = CMaths::csqrt(permittivity) * cosTheta_i * 2.0
                   / ( permittivity * cosTheta_i + sqrtTerm );
    Tperpendicular =  complex( 2.0 * cosTheta_i ) / ( cosTheta_i + sqrtTerm );
  }

  // multiply out all terms to give the electric field
  E_iperpendicular *= Tperpendicular;
  E_iparallel      *= Tparallel;

  // convert back into global coordinate system
  Ei = CComplexVector( ei_perpendicular ) * E_iperpendicular 
     + CComplexVector( ei_parallel ) * E_iparallel;
}

void CRayTracing::transmissionDielectricToFreeSpace( 
                                            CComplexVector&   Ei,
                                      const CSegment3d&       si,
                                      const CConvex&          convexSpace,
                                      const CBoundary&        boundary,
                                      const CBaseTransceiver& btsObject,
                                      const mediaType&        media )
{
  CPoint3d       nhat, sihat, srhat;
  CPoint3d       ei_perpendicular, ei_parallel;
  //double         k1, p;
  double         cosTheta_i;
  complex        Tparallel, Tperpendicular, permittivity, sqrtTerm;
  complex        E_iparallel, E_iperpendicular;

  // define ray-based orthogonal basis.
  sihat            = si.unitVector();
  nhat             = boundary.equation_.outwardNormal_;
  ei_perpendicular = sihat ^ nhat;

  if ( ei_perpendicular == CPoint3d( 0.0, 0.0, 0.0 ) )
  {
    sihat.orthogonalVectors( ei_perpendicular, ei_parallel );
  }
  else
  {
    ei_perpendicular = ei_perpendicular.unitVector();
    ei_parallel      = sihat ^ ei_perpendicular;
  }

  // define unit vectors and lengths along the ray
  //p                = si.length();
  //k1               = btsObject.wavenumber();

  // convert incident field into the local coordinate system
  E_iparallel      = Ei * ei_parallel;
  E_iperpendicular = Ei * ei_perpendicular;
  if ( cabs( E_iperpendicular ) > BIG_PLUS )
    cout << "Problem 1 E_iperpendicular " << endl;
  if ( cabs( E_iparallel ) > BIG_PLUS )
    cout << "Problem 1 E_iparallel " << endl;

  // define the transmission matrix for different dielectric types
  if ( media == PEC )
  {
    Tparallel      = 0.0;
    Tperpendicular = 0.0;
    cosTheta_i     = 0.0;
  }
  else if ( media == LOSSLESS )
  {
    permittivity   = convexSpace.permittivity_;
    cosTheta_i     = sihat * nhat;
    sqrtTerm       = CMaths::csqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Tparallel      = 2.0 * CMaths::csqrt(permittivity) * sqrtTerm 
                   / ( permittivity * cosTheta_i + sqrtTerm );
    Tperpendicular =  sqrtTerm * 2.0 / ( cosTheta_i + sqrtTerm );
    if ( cabs( Tperpendicular ) > BIG_PLUS )
      cout << "Problem 1 Tperpendicular " << endl;
    if ( cabs( Tparallel ) > BIG_PLUS )
      cout << "Problem 1 Tparallel " << endl;
  }
  else
  {
    permittivity   = convexSpace.complexPermittivity( btsObject );
    cosTheta_i     = sihat * nhat;
    sqrtTerm       = CMaths::csqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Tparallel      = 2.0 * CMaths::csqrt(permittivity) * sqrtTerm 
                   / ( permittivity * cosTheta_i + sqrtTerm );
    Tperpendicular =  sqrtTerm * 2.0 / ( cosTheta_i + sqrtTerm );
    if ( cabs( Tperpendicular ) > BIG_PLUS )
      cout << "Problem 2 Tperpendicular " << endl;
    if ( cabs( Tparallel ) > BIG_PLUS )
      cout << "Problem 2 Tparallel " << endl;
  }

  // multiply out all terms to give the electric field
  E_iperpendicular *= Tperpendicular;
  E_iparallel      *= Tparallel;

  // convert back into global coordinate system
  Ei = CComplexVector( ei_perpendicular ) * E_iperpendicular 
     + CComplexVector( ei_parallel ) * E_iparallel;
  if ( Ei.euclideanNorm() > BIG_PLUS )
  {
    cout << "Problem 4 Ei" << endl;
    cout << ei_perpendicular << endl << ei_parallel << endl;
    cout << E_iperpendicular << endl << E_iparallel << endl;
    cout << Tperpendicular << endl << Tparallel << endl;
    cout << permittivity << endl;
    cout << convexSpace.permittivity_ << endl;
    cout << "sqrt " << sqrtTerm << endl;
    cout << "T_i  " << cosTheta_i << endl;
    cout << CMaths::csqrt(permittivity) << endl;
    cout << 2.0 * CMaths::csqrt(permittivity) * sqrtTerm << endl;
    cout << permittivity * cosTheta_i + sqrtTerm << endl;
    cout << sihat << endl;
    cout << nhat << endl;
    cout << si << endl;
    cout << boundary.vertex_[0] << endl;
    cout << boundary.vertex_[1] << endl;
    cout << boundary.vertex_[2] << endl;
    cout << boundary.vertex_[3] << endl;
  }
}

void CRayTracing::divideOutReflectionTerms( 
                                            double            slabWidth,
                                            CComplexVector&   Ei,
                                      const CSegment3d&       si,
                                      const CConvex&          convexSpace,
                                      const CBoundary&        boundary,
                                      const CBaseTransceiver& btsObject,
                                      const mediaType&        media )
{
  CPoint3d       nhat, sihat, srhat;
  CPoint3d       ei_perpendicular, ei_parallel;
  //double         k1, p;
  double         k1, cosTheta_i;
  complex        cosTheta_t;
  complex        Rparallel12, Rparallel23, Rperpendicular12, Rperpendicular23;
  complex        permittivity, sqrtTerm, sqrtPer;
  complex        denomPara, denomPerp, expjkb;
  complex        E_iparallel, E_iperpendicular;

  // define ray-based orthogonal basis.
  sihat            = si.unitVector();
  nhat             = boundary.equation_.outwardNormal_;
  ei_perpendicular = sihat ^ nhat;

  if ( ei_perpendicular == CPoint3d( 0.0, 0.0, 0.0 ) )
  {
    sihat.orthogonalVectors( ei_perpendicular, ei_parallel );
  }
  else
  {
    ei_perpendicular = ei_perpendicular.unitVector();
    ei_parallel      = sihat ^ ei_perpendicular;
  }

  // define unit vectors and lengths along the ray
  //p                = si.length();
  k1               = btsObject.wavenumber();

  // convert incident field into the local coordinate system
  E_iparallel      = Ei * ei_parallel;
  E_iperpendicular = Ei * ei_perpendicular;
  if ( cabs( E_iperpendicular ) > BIG_PLUS )
    cout << "Problem 1 E_iperpendicular " << endl;
  if ( cabs( E_iparallel ) > BIG_PLUS )
    cout << "Problem 1 E_iparallel " << endl;

  // define the transmission matrix for different dielectric types
  if ( media == LOSSLESS )
  {
    permittivity     = convexSpace.permittivity_;
    sqrtPer          = CMaths::csqrt( permittivity );
    cosTheta_i       = sihat * nhat;
    cosTheta_t       = complex(1.0) / sqrtPer
                     * CMaths::csqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Rperpendicular12 = ( cosTheta_i - sqrtPer * cosTheta_t ) 
                     / ( cosTheta_i + sqrtPer * cosTheta_t );
    Rperpendicular23 = ( -cosTheta_i + sqrtPer * cosTheta_t ) 
                     / ( cosTheta_i + sqrtPer * cosTheta_t );
    Rparallel12      = ( -sqrtPer * cosTheta_i + cosTheta_t ) 
                     / ( sqrtPer * cosTheta_i + cosTheta_t );
    Rparallel23      = ( -sqrtPer * cosTheta_i + cosTheta_t ) 
                     / ( sqrtPer * cosTheta_i + cosTheta_t );
                  
    expjkb           = CMaths::expminusj( k1 * sqrtPer * slabWidth * cosTheta_t ); 
    denomPerp        = Rperpendicular12 * Rperpendicular23 * expjkb + 1.0;
    denomPara        = Rparallel12 * Rparallel23 * expjkb + 1.0;
  }
  else
  {
    permittivity     = convexSpace.complexPermittivity( btsObject );
    sqrtPer          = CMaths::csqrt( permittivity );
    cosTheta_i       = sihat * nhat;
    cosTheta_t       = complex(1.0) / sqrtPer
                     * CMaths::csqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Rperpendicular12 = ( cosTheta_i - sqrtPer * cosTheta_t ) 
                     / ( cosTheta_i + sqrtPer * cosTheta_t );
    Rperpendicular23 = ( -cosTheta_i + sqrtPer * cosTheta_t ) 
                     / ( cosTheta_i + sqrtPer * cosTheta_t );
    Rparallel12      = ( -sqrtPer * cosTheta_i + cosTheta_t ) 
                     / ( sqrtPer * cosTheta_i + cosTheta_t );
    Rparallel23      = ( -sqrtPer * cosTheta_i + cosTheta_t ) 
                     / ( sqrtPer * cosTheta_i + cosTheta_t );
                  
    expjkb           = CMaths::expminusj( k1 * sqrtPer * slabWidth * cosTheta_t ); 
    denomPerp        = Rperpendicular12 * Rperpendicular23 * expjkb + 1.0;
    denomPara        = Rparallel12 * Rparallel23 * expjkb + 1.0;
  }

  // multiply out all terms to give the electric field
  E_iperpendicular = E_iperpendicular / denomPerp;
  E_iparallel      = E_iparallel / denomPara;

  // convert back into global coordinate system
  Ei = CComplexVector( ei_perpendicular ) * E_iperpendicular 
     + CComplexVector( ei_parallel ) * E_iparallel;
}

boolean CRayTracing::reflectionCoefficient(  CComplexVector&   Ei,
                                       const CSegment3d&       si,
                                       const CSegment3d&       sr,
                                       const double&           pathLength,
                                       const CPointInfo&       reflectionPoint,
                                       const CBuilding&        buildingObject,
                                       const CBaseTransceiver& btsObject,
                                       const mediaType&        media )
{
  CPoint3d nhat, sihat, srhat;
  CPoint3d ei_perpendicular, ei_parallel, er_perpendicular, er_parallel;
  double   k1, p, s, astigmatic, cosTheta_i;
  complex  Rparallel, Rperpendicular, phase, permittivity;
  complex  E_iparallel, E_iperpendicular, E_rparallel, E_rperpendicular;
  CConvex  *convexSpace;
  int      j;

  convexSpace = &buildingObject.convexArray_[reflectionPoint.convexNumber];
  j           = reflectionPoint.boundaryNumber1;
  nhat        = convexSpace->facet[j].equation_.outwardNormal_;

  // define ray-based orthogonal basis.
  sihat            = si.unitVector();
  srhat            = sr.unitVector();
  ei_perpendicular = sihat ^ nhat;

  if ( ei_perpendicular == CPoint3d( 0.0, 0.0, 0.0 ) )
  {
    sihat.orthogonalVectors( ei_perpendicular, ei_parallel );
    //cout << "orthogonal" << endl;
  }
  else
  {
    ei_perpendicular = ei_perpendicular.unitVector();
    ei_parallel      = ei_perpendicular ^ sihat;
  }

  er_perpendicular = ei_perpendicular;
  er_parallel      = nhat * ( nhat * ei_parallel ) * 2.0 - ei_parallel;

  // define unit vectors and lengths along the ray
  p                = pathLength;
  s                = sr.length();
  k1               = btsObject.wavenumber();

  // convert incident field into the local coordinate system
  E_iparallel      = Ei * ei_parallel;
  E_iperpendicular = Ei * ei_perpendicular;

  if ( media == PEC )
    permittivity = 0.0;
  else if ( media == LOSSLESS )
    permittivity = convexSpace->permittivity_;
  else if ( media == LOSSY )
    permittivity = convexSpace->complexPermittivity( btsObject );
    
  // components of reflection coefficient along the incoming ray
  cosTheta_i = sihat * nhat * -1.0;
  localReflectionCoefficient( cosTheta_i, permittivity, media, 
                              Rparallel, Rperpendicular );

  // define the reflection matrix for different dielectric types
  // multiply out all terms to give the electric field
  astigmatic      = p / ( p + s );
  phase            = CMaths::expminusj( k1 * s );
  E_rperpendicular = Rperpendicular * E_iperpendicular * astigmatic * phase;
  E_rparallel      = Rparallel      * E_iparallel      * astigmatic * phase;

  // convert back into global coordinate system
  Ei = CComplexVector( er_perpendicular ) * E_rperpendicular 
     + CComplexVector( er_parallel ) * E_rparallel;

  randomVariable( btsObject, Ei );
  return true;
}

void CRayTracing::localReflectionCoefficient(       double     cosTheta_i,
                                              const complex&   permittivity,
                                              const mediaType& media,
                                                    complex&   Rparallel,
                                                    complex&   Rperpendicular )
{
  complex sqrtTerm;

  if ( media == PEC )
  {
    Rparallel      =  1.0;
    Rperpendicular = -1.0;
  }
  else if ( media == LOSSLESS )
  {
    sqrtTerm       = CMaths::csqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Rparallel      = ( -permittivity * cosTheta_i + sqrtTerm )
                   / (  permittivity * cosTheta_i + sqrtTerm );
    Rperpendicular = ( cosTheta_i - sqrtTerm ) / ( cosTheta_i + sqrtTerm );
  }
  else
  {
    sqrtTerm       = CMaths::csqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Rparallel      = ( -permittivity * cosTheta_i + sqrtTerm )
                   / (  permittivity * cosTheta_i + sqrtTerm );
    Rperpendicular = ( cosTheta_i - sqrtTerm ) / ( cosTheta_i + sqrtTerm );
  }
}

boolean CRayTracing::cornerDiffraction(      CComplexVector&   Et,
                                       const CSegment3d&       si,
                                       const CSegment3d&       sr,
                                       const double&           pathLength,
                                       const double&           totalPathLength,
                                       const CPointInfo&       diffractionPt,
                                       const CSegment3d&       diffractingEdge,
                                       const CBuilding&        buildingObject,
                                       const CBaseTransceiver& btsObject,
                                       const mediaType&        media,
				             CCorners&         corners,
				       const polarisationType& polarisation )
{
  CConvex   *convexSpace;
  CBoundary *boundary0, *boundary1;
  CPoint3d  n0_hat, n1_hat, sihat, srhat, ehat, si_cross_e, sr_cross_e;
  CPoint3d  e_cross_si, e_cross_sr, firstCorner, secondCorner;
  CPoint3d  phi_dash_hat, phi_hat, beta0_dash_hat, beta0_hat;
  CPoint3d  betaC_dash_hat, betaC_hat, p1;
  double    k1, p, s, n, L, Lc, sinBeta0, anAngle, phi, phi_dash;
  double    cosTheta_i0, cosTheta_i1, angle2, astigmatic;
  double    betaC, beta0c, absS_ic, absS_rc;
  complex   Rparallel1, Rperpendicular1, Rparallel2, Rperpendicular2;
  complex   Dsoft, Dhard, phase, permittivity, constTerm;
  CPoint3d  t0_hat, t1_hat, minusSt_dash_hat, st_hat, st_dash, st;
  CPoint3d  badPoint;
  complex   Ei_beta0_dash, Ei_phi_dash, Ed_beta0, Ed_phi;
  //boolean   isCorners;
  CSegment3d s_ic, s_rc;

  CDiffractionCoefficient diffractionObject;
  CCornerDiffraction      cornerDiffObject;
  CComplexVector          Ei;

  badPoint = CPoint3d( -0.99, -0.99, -0.99 );

  // define the boundaries of the convex space containing the diffraction edge
  convexSpace = &buildingObject.convexArray_[diffractionPt.convexNumber];
  boundary0   = &convexSpace->facet[diffractionPt.boundaryNumber1];
  boundary1   = &convexSpace->facet[diffractionPt.boundaryNumber2];

  // define unit vector of all vectors (needed below)
  n0_hat = boundary0->equation_.outwardNormal_;
  n1_hat = boundary1->equation_.outwardNormal_;

  //if ( n0_hat == CPoint3d( 0.0, 0.0, 1.0 ) 
    //&& n1_hat == CPoint3d( -1.0, 0.0, 0.0 ) )
  {
    //return false;
    //cout << "n0_hat: " << n0_hat << "n1_hat: " << n1_hat << endl;
  }

  /*if ( n0_hat != CPoint3d( 0.0, 0.0, 1.0 )
    && n1_hat != CPoint3d( 0.0, 0.0, 1.0 ) )
  {
    Et = CComplexVector();
    return false;
  }*/


  // uncomment this to turn off side components
  /*if ( n1_hat == CPoint3d( 0.0, 1.0, 0.0 )  )
  {
    Et = CComplexVector();
    return false;
  }

  if ( n1_hat == CPoint3d( 0.0, -1.0, 0.0 )  )
  {
    Et = CComplexVector();
    return false;
  }

  // to turn off first diffraction nearest source
  if ( n1_hat == CPoint3d( 1.0, 0.0, 0.0 )  )
  {
    Et = CComplexVector();
    return false;
  }*/

  /*if ( n1_hat == CPoint3d( -1.0, 0.0, 0.0 )  )
  {
    Et = CComplexVector();
    return false;
  }*/

  // to set up a plate solution uncomment these two lines
  if ( n0_hat == CPoint3d( 0.0, 0.0, 1.0 )  )
    n1_hat = CPoint3d( 0.0, 0.0, -1.0 );

  sihat  = si.unitVector();
  srhat  = sr.unitVector();
  ehat   = diffractingEdge.unitVector();
  t0_hat = n0_hat ^ ehat;

  // check just inside the boundary to see if t0_hat is contained.
  // if not, then we must ensure that it will be
  //if ( boundary0->intersection( diffractionPt.node + t0_hat * TOL3 ) == false )
  p1 = diffractionPt.node + t0_hat * TOL3;
  if ( p1.abs( boundary0->centre() ) 
     > diffractionPt.node.abs( boundary0->centre() ) )
  {
    ehat   = ehat * -1.0;
    t0_hat = n0_hat ^ ehat; 
    //cout << "Got t0 the wrong way around in this instance" << endl;
  }

  /*if ( boundary0->intersection( diffractionPt.node + t0_hat * TOL3 ) == false )
  {
    cout << "t0 not contained in plane" << endl;
    cout << "t0: " << t0_hat << "  n0: " << n0_hat << endl;
    cout << "edge: " << ehat << endl;
    cout << "boundary: " << *boundary0 << endl;
    exit(1);
  }*/

  t1_hat = n1_hat ^ ehat;
  //cout << "ehat: " << ehat << endl;

  p1 = diffractionPt.node + t1_hat * TOL3;

  if ( p1.abs( boundary0->centre() ) 
     > diffractionPt.node.abs( boundary0->centre() ) )
  {
    t1_hat = t1_hat * -1.0;
  }

  // pre-define some vectors to speed up the computation
  si_cross_e = sihat ^ ehat;
  e_cross_si = ehat ^ sihat;

  if ( si_cross_e == CPoint3d( 0.0, 0.0, 0.0 ) )
    cout << "Tried to get a diffraction point along edge" << endl;

  sr_cross_e = srhat ^ ehat;
  e_cross_sr = ehat ^ srhat;
  //sinBeta0   = si_cross_e.abs();
  sinBeta0   = e_cross_si.abs();

  // define ray-based orthogonal basis
  /*phi_dash_hat   = si_cross_e / sinBeta0;
  beta0_dash_hat = sihat ^ phi_dash_hat;
  phi_hat        = sr_cross_e / sinBeta0;
  beta0_hat      = srhat ^ phi_hat;*/
  phi_dash_hat   = e_cross_si / (-sinBeta0);
  beta0_dash_hat = phi_dash_hat ^ sihat;
  phi_hat        = e_cross_sr / sinBeta0;
  beta0_hat      = phi_hat ^ srhat;

  // define vector lengths along the ray
  //p  = si.length();
  //s  = sr.length();
  p  = pathLength;
  s  = totalPathLength - pathLength;
  k1 = btsObject.wavenumber();

  // calculation of phi and phi' angles is difficult but follows. Requires a 
  // projection along both wedge faces in the direction perpendicular to the 
  // edge. This help us find the angles using that axis and the outward normal.


  st_dash          = ( sihat - ehat * ( sihat * ehat ));
  minusSt_dash_hat = st_dash / -sinBeta0;
  st               = ( srhat - ehat * ( srhat * ehat )); 
  st_hat           = st / st.abs();
  n                = 2.0 - t0_hat.angle( t1_hat ) / Pi;

  // found this in McNamara pp.268. Obviously he found a trick for getting the 
  // correct angle every time. Otherwise you would need to use if statements.
  // angles must be in the range [0,Pi] from Point3d class otherwise the
  // angles returned would be wrong. sign(*) acts as the if statement.
  anAngle     = minusSt_dash_hat.angle(t0_hat);
  angle2      = minusSt_dash_hat.angle(n0_hat);
  cosTheta_i0 = cos( angle2 );
  phi_dash    = Pi - ( Pi - anAngle ) * sign( minusSt_dash_hat * n0_hat );
  anAngle     = st_hat.angle(t0_hat);
  angle2      = st_hat.angle(n0_hat);
  cosTheta_i1 = cos( angle2 );
  phi         = Pi - ( Pi - anAngle ) * sign( st_hat * n0_hat );

  // convert incident field into the local ray-based coordinate system
  Ei_beta0_dash = Et * beta0_dash_hat;
  Ei_phi_dash   = Et * phi_dash_hat;

  // define the permittivity of the interfacing media for different types
  if ( media == PEC )
    permittivity  = 0.0;
  else if ( media == LOSSLESS )
    permittivity  = convexSpace->permittivity_;
  else if ( media == LOSSY )
    permittivity = convexSpace->complexPermittivity( btsObject );

  // components of reflection coefficient along the incoming ray
  // from both side of the wedge
  localReflectionCoefficient( cosTheta_i0, permittivity, media, 
                              Rparallel1, Rperpendicular1 );

  localReflectionCoefficient( cosTheta_i1, permittivity, media, 
                              Rparallel2, Rperpendicular2 );

  astigmatic = sqrt( p / s / ( p + s ) );
  //attenuation    = ( p * s * qsqr( sinBeta0 ) ) / ( p + s );
  L              = ( p * s ) * qsqr( sinBeta0 ) / ( p + s );
  Lc             = ( p * s ) / ( p + s );
  phase          = CMaths::expminusj( k1 * s );
  cout << "Phase: " << phase << endl;

  //cout << "---------------------------------------------" << endl;
  //cout << "Ei = " << Ei << endl;
  // build soft (E-polarized) are hard (H-polarized) diffraction coefficients
  Dsoft = diffractionObject.softDiffraction( phi, phi_dash, n, k1, sinBeta0, 
                                             L, Rperpendicular1, 
                                             Rperpendicular2 );

  Dhard = diffractionObject.hardDiffraction( phi, phi_dash, n, k1, sinBeta0,
                                             L, Rparallel1, Rparallel2 );

  // define the reflection matrix for different dielectric types
  // multiply out all terms to give the electric field
  Ed_beta0 = Dsoft * Ei_beta0_dash * astigmatic * phase;
  Ed_phi   = Dhard * Ei_phi_dash   * astigmatic * phase;

  // convert back into global coordinate system
  /*if ( fabs( sinBeta0 - 1.0 ) > TOL )
  {
    Et = CComplexVector();
    cout << "diffraction coefficients off line" << endl;
    return true;
  }*/

  if ( diffractionPt.contact != VIRTUAL_DIFFRACTION )
  {
    //cout << "calculated diffraction " << diffractionPt << endl;
    Et = ( CComplexVector( beta0_hat ) * Ed_beta0 * -1.0
       - CComplexVector( phi_hat   ) * Ed_phi );
  }

  cornerDiffObject.checkIfCornerDiffractionExists(
                    corners, diffractingEdge, firstCorner, secondCorner );
  //isCorners = cornerDiffObject.checkIfCornerDiffractionExists(
                    //corners, diffractingEdge, firstCorner, secondCorner );

  if ( firstCorner != badPoint && btsObject.cornerDiffraction() == true 
     && diffractionPt.contact == VIRTUAL_DIFFRACTION )
  {
    calculateAnglesAndLocalCoordinates( firstCorner, ehat, t0_hat, n0_hat,
                                        si, sr, diffractingEdge,
                                        betaC_dash_hat, phi_dash_hat, 
					betaC_hat, phi_hat, betaC,
					beta0c, Lc, s_ic, s_rc,
					absS_ic, absS_rc );  
    if ( btsObject.hertzianDipole() == true )
      Ei = incidentFieldHertzian( s_ic, polarisation, btsObject );
    else
      Ei = incidentField( s_ic, polarisation, btsObject );
      

    Ei_beta0_dash = Ei * betaC_dash_hat;
    Ei_phi_dash   = Ei * phi_dash_hat;

    constTerm     = cornerDiffObject.cornerDiffractionConst( k1, sinBeta0, 
                                     betaC, beta0c, Lc, absS_rc );

    Dsoft = cornerDiffObject.cornerDiffractionCoefficient(
                                                phi, phi_dash, n, k1, sinBeta0,
                                                betaC, beta0c, L, Lc,
                                                Rperpendicular1,
						Rperpendicular2 );
    Dhard = cornerDiffObject.cornerDiffractionCoefficient( 
                                                phi, phi_dash, n, k1, sinBeta0,
                                                betaC, beta0c, L, Lc,
                                                Rparallel1, Rparallel2 );
    Ed_beta0 = Dsoft * Ei_beta0_dash;
    Ed_phi   = Dhard * Ei_phi_dash;
    /*cout << "------------------------------------------------------" << endl;
    cout << "given diffracting edge: " << diffractingEdge << endl;
    cout << "diffraction point: " << diffractionPt << endl;
    cout << "first corner point: " << firstCorner << endl;
    cout << "betaC: " << betaC / TwoPi * 180.0 
         << " beta0c: " << beta0c / TwoPi * 180.0 << endl;
    cout << "beta0: " << asin( sinBeta0 ) / TwoPi * 180.0 << endl;*/
    Et = ( CComplexVector( betaC_hat ) * Ed_beta0 
         + CComplexVector( phi_hat   ) * Ed_phi ) *  constTerm * -1.0;
  }

  if ( secondCorner != badPoint && btsObject.cornerDiffraction() == true 
    && diffractionPt.contact == VIRTUAL_DIFFRACTION )
  {
    //cout << "diffraction point: " << diffractionPt << endl;
    calculateAnglesAndLocalCoordinates( secondCorner, ehat, t0_hat, n0_hat,
                                        si, sr, diffractingEdge,
                                        betaC_dash_hat, phi_dash_hat, 
					betaC_hat, phi_hat, betaC, beta0c,
					Lc, s_ic, s_rc, absS_ic, absS_rc );  
    if ( btsObject.hertzianDipole() == true )
      Ei = incidentFieldHertzian( s_ic, polarisation, btsObject );
    else
      Ei = incidentField( s_ic, polarisation, btsObject );

    Ei_beta0_dash = Ei * betaC_dash_hat;
    Ei_phi_dash   = Ei * phi_dash_hat;

    constTerm     = cornerDiffObject.cornerDiffractionConst( k1, sinBeta0, 
                                     betaC, beta0c, Lc, absS_rc );

    Dsoft = cornerDiffObject.cornerDiffractionCoefficient(
                                                phi, phi_dash, n, k1, sinBeta0,
                                                betaC, beta0c, L, Lc,
                                                Rperpendicular1,
						Rperpendicular2 );
    Dhard = cornerDiffObject.cornerDiffractionCoefficient( 
                                                phi, phi_dash, n, k1, sinBeta0,
                                                betaC, beta0c, L, Lc,
                                                Rparallel1, Rparallel2 );
    Ed_beta0 = Dsoft * Ei_beta0_dash;
    Ed_phi   = Dhard * Ei_phi_dash;
    /*cout << "second corner point: " << secondCorner << endl;
    cout << "betaC: " << betaC / TwoPi * 180.0 
         << " beta0c: " << beta0c / TwoPi * 180.0 << endl;
    cout << "beta0: " << asin( sinBeta0 ) / TwoPi * 180.0 << endl;
    cout << "------------------------------------------------------" << endl;*/
    Et = ( CComplexVector( betaC_hat ) * Ed_beta0 
         + CComplexVector( phi_hat   ) * Ed_phi ) * constTerm * -1.0;
  }

  //randomVariable( btsObject, Ei );

  /*cout << "si    = " << si << endl;
  cout << "sihat = " << sihat << endl;
  cout << "sr    = " << sr << endl;
  cout << "srhat = " << srhat << endl;
  cout << "t0hat = " << t0_hat << endl;
  cout << "t1hat = " << t1_hat << endl;
  cout << "ehat  = " << ehat  << endl;
  cout << "k1 (beta) = " << k1  << endl;
  cout << "beta0_hat = " << beta0_hat  << endl;
  cout << "phi_hat  = " << phi_hat  << endl;
  cout << "beta0_dash_hat = " << beta0_dash_hat  << endl;
  cout << "phi_dash_hat   = " << phi_dash_hat  << endl;
  cout << "sinBeta0 = " << sinBeta0 << endl;
  cout << "attenuation = " << attenuation << endl;
  cout << "edge  = " << diffractingEdge << endl;
  cout << "phi\'  = " << phi_dash << endl;
  cout << "phi   = " << phi << endl;
  cout << "Dsoft = " << Dsoft << endl;
  cout << "Dhard = " << Dhard << endl;
  cout << "Ed_beta0 = " << Ed_beta0 << endl;
  cout << "Ed_phi   = " << Ed_phi << endl;
  cout << "Es       = " << Ei << endl;*/

  return true;
}

void CRayTracing::calculateAnglesAndLocalCoordinates(
                                        const CPoint3d&   cornerPoint,
					const CPoint3d&   ehat,
					const CPoint3d&   t0_hat,
					const CPoint3d&   n0_hat,
					const CSegment3d& si,
                                        const CSegment3d& sr,
					const CSegment3d& diffractingEdge,
                                              CPoint3d&   betaC_dash_hat, 
					      CPoint3d&   phi_dash_hat, 
					      CPoint3d&   betaC_hat,
					      CPoint3d&   phi_hat, 
					      double&     betaC,
					      double&     beta0c, 
					      double&     Lc, 
					      CSegment3d& s_ic, 
					      CSegment3d& s_rc,
					      double&     absS_ic,
					      double&     absS_rc )  
{
  CPoint3d   e_cross_si, e_cross_sr, sihat, srhat;
  CPoint3d   newEhat;
  double     sinBetaC, cosBeta0c, cosBetaC;
  
  s_ic   = CSegment3d( si.start(), cornerPoint );
  s_rc   = CSegment3d( cornerPoint, sr.end() );
  sihat  = s_ic.unitVector();
  srhat  = s_rc.unitVector();
  absS_ic = s_ic.length(); 
  absS_rc = s_rc.length(); 
  Lc      = absS_ic * absS_rc / ( absS_ic + absS_rc );

  // pre-define some vectors to speed up the computation
  e_cross_si = ehat ^ sihat;
  e_cross_sr = ehat ^ srhat;
  sinBetaC   = e_cross_si.abs();

  // define ray-based orthogonal basis
  phi_dash_hat   = e_cross_si / (-sinBetaC);
  betaC_dash_hat = phi_dash_hat ^ sihat;
  phi_hat        = e_cross_sr / e_cross_sr.abs();
  betaC_hat      = phi_hat ^ srhat;

  // found this in McNamara pp.268. Obviously he found a trick for getting the 
  // correct angle every time. Otherwise you would need to use if statements.
  // angles must be in the range [0,Pi] from Point3d class otherwise the
  // angles returned would be wrong. sign(*) acts as the if statement.

  newEhat   = diffractingEdge.unitVector();
  cosBetaC  = newEhat * sihat;
  cosBeta0c = newEhat * srhat;
  // unit vector must point along and into plate from corner point
  // to get correct angles, otherwise we switch the cos(angle) results
  if ( diffractingEdge.start() == cornerPoint )
  {
    betaC  = acos( -cosBetaC );
    beta0c = acos( -cosBeta0c );
  }
  else 
  {
    betaC  = acos(  cosBetaC );
    beta0c = acos(  cosBeta0c );
  }
}

boolean CRayTracing::diffractionCoefficient( CComplexVector&   Ei,
                                       const CSegment3d&       si,
                                       const CSegment3d&       sr,
                                       const double&           pathLength,
                                       const double&           totalPathLength,
                                       const CPointInfo&       diffractionPt,
                                       const CSegment3d&       diffractingEdge,
                                       const CBuilding&        buildingObject,
                                       const CBaseTransceiver& btsObject,
                                       const mediaType&        media )
{
  CConvex   *convexSpace;
  CBoundary *boundary0, *boundary1;
  CPoint3d  n0_hat, n1_hat, sihat, srhat, ehat, si_cross_e, sr_cross_e;
  CPoint3d  e_cross_si, e_cross_sr;
  CPoint3d  phi_dash_hat, phi_hat, beta0_dash_hat, beta0_hat;
  double    k1, p, s, n, attenuation, sinBeta0, anAngle, phi, phi_dash;
  double    cosTheta_i0, cosTheta_i1, angle2, astigmatic;
  complex   Rparallel1, Rperpendicular1, Rparallel2, Rperpendicular2;
  complex   Dsoft, Dhard, phase, permittivity;
  CPoint3d  t0_hat, t1_hat, minusSt_dash_hat, st_hat, st_dash, st;
  complex   Ei_beta0_dash, Ei_phi_dash, Ed_beta0, Ed_phi;

  CDiffractionCoefficient diffractionObject;

  // define the boundaries of the convex space containing the diffraction edge
  convexSpace = &buildingObject.convexArray_[diffractionPt.convexNumber];
  boundary0   = &convexSpace->facet[diffractionPt.boundaryNumber1];
  boundary1   = &convexSpace->facet[diffractionPt.boundaryNumber2];

  // define unit vector of all vectors (needed below)
  n0_hat = boundary0->equation_.outwardNormal_;
  n1_hat = boundary1->equation_.outwardNormal_;
  /*if ( n0_hat != CPoint3d( 0.0, 0.0, 1.0 )  )
  {
    //cout << "boundary0:" << *boundary0 << endl;
    //cout << "boundary1:" << *boundary1 << endl;
    //cout << "n0_hat: " << n0_hat << "n1_hat: " << n1_hat << endl;
  }*/

  /*if ( n0_hat != CPoint3d( 0.0, 0.0, 1.0 )
    && n1_hat != CPoint3d( 0.0, 0.0, 1.0 ) )
  {
    cout << "Both outward normals are unit(z) normals" << endl;
    Ei = CComplexVector();
    return false;
  }*/

  if ( n0_hat == CPoint3d( 0.0, 0.0, 1.0 )  )
    n1_hat = CPoint3d( 0.0, 0.0, -1.0 );
  sihat  = si.unitVector();
  srhat  = sr.unitVector();
  ehat   = diffractingEdge.unitVector();
  t0_hat = n0_hat ^ ehat;

  // check just inside the boundary to see if t0_hat is contained.
  // if not, then we must ensure that it will be
  if ( boundary0->intersection( diffractionPt.node + t0_hat * TOL3 ) == false )
  {
    ehat   = ehat * -1.0;
    t0_hat = n0_hat ^ ehat; 
    //cout << "Got t0 the wrong way around in this instance" << endl;
  }
  if ( boundary0->intersection( diffractionPt.node + t0_hat * TOL3 ) == false )
  {
    cout << "t0 not contained in plane" << endl;
    cout << "t0: " << t0_hat << "  n0: " << n0_hat << endl;
    cout << "edge: " << ehat << endl;
    cout << "boundary: " << *boundary0 << endl;
    exit(1);
  }

  t1_hat = n1_hat ^ ehat;

  /*
  //if ( boundary1->intersection( diffractionPt.node + t1_hat * TOL3 ) == false )
  if ( boundary0->intersection( diffractionPt.node + t1_hat * TOL3 ) == false )
  {
    t1_hat = t1_hat * -1.0;
  }

  ///if ( boundary1->intersection(diffractionPt.node + t1_hat * TOL3 ) == false)
  if ( boundary0->intersection( diffractionPt.node + t1_hat * TOL3 ) == false )
  {
    cout << "t1 not contained in plane" << endl;
    exit(1);
  }*/

  // pre-define some vectors to speed up the computation
  si_cross_e = sihat ^ ehat;
  e_cross_si = ehat ^ sihat;

  if ( si_cross_e == CPoint3d( 0.0, 0.0, 0.0 ) )
    cout << "Tried to get a diffraction point along edge" << endl;

  sr_cross_e = srhat ^ ehat;
  e_cross_sr = ehat ^ srhat;
  //sinBeta0   = si_cross_e.abs();
  sinBeta0   = e_cross_si.abs();

  // define ray-based orthogonal basis
  /*phi_dash_hat   = si_cross_e / sinBeta0;
  beta0_dash_hat = sihat ^ phi_dash_hat;
  phi_hat        = sr_cross_e / sinBeta0;
  beta0_hat      = srhat ^ phi_hat;*/
  phi_dash_hat   = e_cross_si / (-sinBeta0);
  beta0_dash_hat = phi_dash_hat ^ sihat;
  phi_hat        = e_cross_sr / sinBeta0;
  beta0_hat      = phi_hat ^ srhat;

  // define vector lengths along the ray
  //p  = si.length();
  //s  = sr.length();
  p  = pathLength;
  s  = totalPathLength - pathLength;
  k1 = btsObject.wavenumber();

  // calculation of phi and phi' angles is difficult but follows. Requires a 
  // projection along both wedge faces in the direction perpendicular to the 
  // edge. This help us find the angles using that axis and the outward normal.


  st_dash          = ( sihat - ehat * ( sihat * ehat ));
  minusSt_dash_hat = st_dash / -sinBeta0;
  st               = ( srhat - ehat * ( srhat * ehat )); 
  st_hat           = st / st.abs();
  n                = 2.0 - t0_hat.angle( t1_hat ) / Pi;

  // found this in McNamara pp.268. Obviously he found a trick for getting the 
  // correct angle every time. Otherwise you would need to use if statements.
  // angles must be in the range [0,Pi] from Point3d class otherwise the
  // angles returned would be wrong. sign(*) acts as the if statement.
  anAngle     = minusSt_dash_hat.angle(t0_hat);
  angle2      = minusSt_dash_hat.angle(n0_hat);
  cosTheta_i0 = cos( angle2 );
  phi_dash    = Pi - ( Pi - anAngle ) * sign( minusSt_dash_hat * n0_hat );
  anAngle     = st_hat.angle(t0_hat);
  angle2      = st_hat.angle(n0_hat);
  cosTheta_i1 = cos( angle2 );
  phi         = Pi - ( Pi - anAngle ) * sign( st_hat * n0_hat );

  // convert incident field into the local ray-based coordinate system
  Ei_beta0_dash = Ei * beta0_dash_hat;
  Ei_phi_dash   = Ei * phi_dash_hat;
  cout << "Ei: " << Ei << endl;

  // define the permittivity of the interfacing media for different types
  if ( media == PEC )
    permittivity  = 0.0;
  else if ( media == LOSSLESS )
    permittivity  = convexSpace->permittivity_;
  else if ( media == LOSSY )
    permittivity = convexSpace->complexPermittivity( btsObject );
    
  // components of reflection coefficient along the incoming ray
  // from both side of the wedge
  localReflectionCoefficient( cosTheta_i0, permittivity, media, 
                              Rparallel1, Rperpendicular1 );

  localReflectionCoefficient( cosTheta_i1, permittivity, media, 
                              Rparallel2, Rperpendicular2 );

  astigmatic     = sqrt( p / s / ( p + s ) );
  attenuation    = ( p * s * qsqr( sinBeta0 ) ) / ( p + s );
  //attenuation    = ( p * s ) / ( p + s );
  phase          = CMaths::expminusj( k1 * s );

  cout << "---------------------------------------------" << endl;
  cout << "Ei = " << Ei << endl;
  // build soft (E-polarized) are hard (H-polarized) diffraction coefficients
  Dsoft = diffractionObject.softDiffraction( phi, phi_dash, n, k1, sinBeta0, 
                                             attenuation, Rperpendicular1, 
                                             Rperpendicular2 );

  Dhard = diffractionObject.hardDiffraction( phi, phi_dash, n, k1, sinBeta0,
                                             attenuation, Rparallel1, 
                                             Rparallel2 );
  cout << "sin Beta 0: " << sinBeta0 << endl;
  cout << "p :" << p << "  s: " << s << endl;
  if ( cabs(Ei_phi_dash) > 3.0 )
  {
    Ei = CComplexVector();
    cout << "Warning: diffraction coefficients asymptotic failure, return zero" << endl;
    return true;
  }
  cout << "Dsoft: " << Dsoft << "  Dhard: " << Dhard << endl;

  // define the reflection matrix for different dielectric types
  // multiply out all terms to give the electric field
  Ed_beta0 = Dsoft * Ei_beta0_dash * astigmatic * phase;
  Ed_phi   = Dhard * Ei_phi_dash   * astigmatic * phase;
  cout << Dhard << " | Ei_phi_dash: " << Ei_phi_dash << " | " << astigmatic << " | " << phase << endl;

  // convert back into global coordinate system
  /*if ( fabs( sinBeta0 - 1.0 ) > TOL )
  {
    Ei = CComplexVector();
    cout << "diffraction coefficients off line" << endl;
    return true;
  }*/

  Ei = ( CComplexVector( beta0_hat ) * Ed_beta0 * -1.0
     - CComplexVector( phi_hat   ) * Ed_phi );
  //randomVariable( btsObject, Ei );

  /*cout << "si    = " << si << endl;
  cout << "sihat = " << sihat << endl;
  cout << "sr    = " << sr << endl;
  cout << "srhat = " << srhat << endl;
  cout << "t0hat = " << t0_hat << endl;
  cout << "t1hat = " << t1_hat << endl;
  cout << "ehat  = " << ehat  << endl;
  cout << "k1 (beta) = " << k1  << endl;
  cout << "beta0_hat = " << beta0_hat  << endl;
  cout << "phi_hat  = " << phi_hat  << endl;
  cout << "beta0_dash_hat = " << beta0_dash_hat  << endl;
  cout << "phi_dash_hat   = " << phi_dash_hat  << endl;
  cout << "sinBeta0 = " << sinBeta0 << endl;
  cout << "attenuation = " << attenuation << endl;
  cout << "edge  = " << diffractingEdge << endl;
  cout << "phi\'  = " << phi_dash << endl;
  cout << "phi   = " << phi << endl;*/
  cout << "Dsoft = " << Dsoft << endl;
  cout << "Dhard = " << Dhard << endl;
  cout << "Ed_beta0 = " << Ed_beta0 << endl;
  cout << "Ed_phi   = " << Ed_phi << endl;
  cout << "Es       = " << Ei << endl;

  return true;
}

CComplexVector CRayTracing::incidentField( 
                                  CSegment3d&       si, 
                            const polarisationType& polarisation,
                            const CBaseTransceiver& btsObject )
{
  CPoint3d       xhat, yhat, zhat, unitIncInPlane;
  CComplexVector Et;
  CPoint3d       x_l, y_l, z_l;
  double         phi, theta;

  unitIncInPlane = si.unitVector();

  if ( polarisation == Z_POLARISATION )
  {
    if ( fabs( fabs(unitIncInPlane.z) - 1.0 ) < TOL )
      Et = CComplexVector();
    else
    {
      x_l  = CPoint3d( 1.0, 0.0, 0.0 );
      y_l  = CPoint3d( 0.0, 1.0, 0.0 );
      z_l  = CPoint3d( 0.0, 0.0, 1.0 );
      theta= si.angle( z_l );
      phi  = atan2( unitIncInPlane * y_l, unitIncInPlane * x_l );
      Et   = btsObject.incidentField( si.length(), x_l, y_l, z_l, 
                                      theta, phi );
      if ( myIsnan( Et.z.real() ) )
      {
        cout << "incident field Z" << endl;
        cout << theta << "  " << phi << endl;
        cout << Et << endl;
        cout << unitIncInPlane << endl;
        cout << x_l << endl;
        cout << y_l << endl;
        cout << z_l << endl;
        exit(1);
      }
    }
  }
  else if ( polarisation == X_POLARISATION )
  {
    if ( fabs( fabs(unitIncInPlane.x ) - 1.0 ) < TOL )
      Et = CComplexVector();
    else
    {
      x_l  = CPoint3d( 0.0, 1.0, 0.0 );
      y_l  = CPoint3d( 0.0, 0.0, 1.0 );
      z_l  = CPoint3d( 1.0, 0.0, 0.0 );
      theta= si.angle( z_l );
      phi  = atan2( unitIncInPlane * y_l, unitIncInPlane * x_l );
      Et   = btsObject.incidentField( si.length(), x_l, y_l, z_l, theta, phi );
      if ( myIsnan( Et.x.real() ) )
      {
        cout << "incident field X" << endl;
        cout << theta << "  " << phi << endl;
        cout << Et << endl;
        cout << unitIncInPlane << endl;
        cout << x_l << endl;
        cout << y_l << endl;
        cout << z_l << endl;
        exit(1);
      }
    }
  }
  else if ( polarisation == Y_POLARISATION )
  {
    if ( fabs( fabs(unitIncInPlane.y) - 1.0 ) < TOL )
      Et = CComplexVector();
    else
    {
      x_l  = CPoint3d( 0.0, 0.0, 1.0 );
      y_l  = CPoint3d( 1.0, 0.0, 0.0 );
      z_l  = CPoint3d( 0.0, 1.0, 0.0 );
      theta= si.angle( z_l );
      phi  = atan2( unitIncInPlane * y_l, unitIncInPlane * x_l );
      Et   = btsObject.incidentField( si.length(), x_l, y_l, z_l, theta, phi );
      if ( myIsnan( Et.y.real() ) )
      {
        cout << "incident field Y" << endl;
        cout << theta << "  " << phi << endl;
        cout << Et << endl;
        cout << unitIncInPlane << endl;
        cout << x_l << endl;
        cout << y_l << endl;
        cout << z_l << endl;
        si.angle( yhat );
        exit(1);
      }
    }
  }
  else
    Et = CComplexVector();

  return Et;
}

CComplexVector CRayTracing::incidentFieldHertzian( 
                                  CSegment3d&       si, 
                            const polarisationType& polarisation,
                            const CBaseTransceiver& btsObject )
{
  CPoint3d       xhat, yhat, zhat, unitIncInPlane;
  CComplexVector Et;
  CPoint3d       x_l, y_l, z_l;
  double         phi, theta;

  unitIncInPlane = si.unitVector();

  if ( polarisation == Z_POLARISATION )
  {
    if ( fabs( fabs(unitIncInPlane.z) - 1.0 ) < TOL )
      Et = CComplexVector();
    else
    {
      x_l  = CPoint3d( 1.0, 0.0, 0.0 );
      y_l  = CPoint3d( 0.0, 1.0, 0.0 );
      z_l  = CPoint3d( 0.0, 0.0, 1.0 );
      theta= si.angle( z_l );
      phi  = atan2( unitIncInPlane * y_l, unitIncInPlane * x_l );
      Et   = btsObject.incidentFieldHertzian( si.length(), x_l, y_l, z_l, theta, phi );
     
      if ( myIsnan( Et.z.real() ) )
      {
        cout << "incident field Z" << endl;
        cout << theta << "  " << phi << endl;
        cout << Et << endl;
        cout << unitIncInPlane << endl;
        cout << x_l << endl;
        cout << y_l << endl;
        cout << z_l << endl;
        exit(1);
      }
    }
  }
  else if ( polarisation == X_POLARISATION )
  {
    if ( fabs( fabs(unitIncInPlane.x ) - 1.0 ) < TOL )
      Et = CComplexVector();
    else
    {
      x_l  = CPoint3d( 0.0, 1.0, 0.0 );
      y_l  = CPoint3d( 0.0, 0.0, 1.0 );
      z_l  = CPoint3d( 1.0, 0.0, 0.0 );
      theta= si.angle( z_l );
      phi  = atan2( unitIncInPlane * y_l, unitIncInPlane * x_l );
      Et   = btsObject.incidentFieldHertzian( si.length(), x_l, y_l, z_l, theta, phi );
      if ( myIsnan( Et.x.real() ) )
      {
        cout << "incident field X" << endl;
        cout << theta << "  " << phi << endl;
        cout << Et << endl;
        cout << unitIncInPlane << endl;
        cout << x_l << endl;
        cout << y_l << endl;
        cout << z_l << endl;
        exit(1);
      }
    }
  }
  else if ( polarisation == Y_POLARISATION )
  {
    if ( fabs( fabs(unitIncInPlane.y) - 1.0 ) < TOL )
      Et = CComplexVector();
    else
    {
      x_l  = CPoint3d( 0.0, 0.0, 1.0 );
      y_l  = CPoint3d( 1.0, 0.0, 0.0 );
      z_l  = CPoint3d( 0.0, 1.0, 0.0 );
      theta= si.angle( z_l );
      phi  = atan2( unitIncInPlane * y_l, unitIncInPlane * x_l );
      Et   = btsObject.incidentFieldHertzian( si.length(), x_l, y_l, z_l, theta, phi );
      if ( myIsnan( Et.y.real() ) )
      {
        cout << "incident field Y" << endl;
        cout << theta << "  " << phi << endl;
        cout << Et << endl;
        cout << unitIncInPlane << endl;
        cout << x_l << endl;
        cout << y_l << endl;
        cout << z_l << endl;
        si.angle( yhat );
        exit(1);
      }
    }
  }
  else
    Et = CComplexVector();

  return Et;
}
