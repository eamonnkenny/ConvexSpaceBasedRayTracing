//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    13th June 2002
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include "downlink.hh"

double CDownlink::objective( boolean& isCapacityExceeded,
                                 int&     l, double   t,
                                 int      numberOfSectors,
                                 const    CVector<double>&  vcentres,
                                 const    CVector<double>&  oldCentres,
                                          CBaseTransceiver& btsObject,
                                 const    CBuilding&  buildingObject,
                                 const    CCapacity&  capacityGrid,
                                          double*     targetSIR,
                                 const    SystemParameters& parameterObject,
                                 const    boolean&    writeBestServer,
                                          complex     ***fieldStrength )
                             
{
  CPathLoss          bestServerGrid;
  double             *maximumPower;
  CRayTracing        rayTracingObject;
  CPoint3d           userCapacityPoint;
  CMemoryStorage     storageObject;
  List<CRayPath>     allRayPaths;
  List<CPoint3d>     centres, prevCentres;
  complex            **G;
  complex            *Rtotal, **Rmin, ***R;
  double             sectorAngle;
  double             answer;
  complex            *totalGains;
  complex            totalCheck;
  int                i, j, k, numberOfServers;

  centres     = vectorToCentres( vcentres );
  prevCentres = vectorToCentres( oldCentres );

  // find maximum power and gains for the base stations
  sectorAngle     = TwoPi / numberOfSectors;
  numberOfServers = centres.size();

  R    = storageObject.CreateComplex3dMatrix( numberOfServers, numberOfServers,
                                              numberOfSectors );
  Rmin = storageObject.CreateComplexMatrix( numberOfServers, numberOfSectors );
  G    = storageObject.CreateComplexMatrix( numberOfServers, numberOfSectors );
  Rtotal = storageObject.CreateComplexVector( numberOfServers );
  totalGains   = storageObject.CreateComplexVector( numberOfServers );

  for ( i = 0; i < numberOfServers; i++ )
  {
    btsObject.setPosition( centres.GetItem(i) );
    for ( j = 0; j < numberOfServers; j++ )
    {
      if ( i != j )
      {
        rayTracingObject.MimoRayTracing( allRayPaths, buildingObject, btsObject,
                                         centres.GetItem(j), LOSSLESS ); 

        //meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, R[i][j] );
        //cout << "mean Absolute sum: " << R[i][j][0] << endl;
        //cout << "About to perfrom random Walk " << i << j << endl;
        if ( btsObject.randomWalk() == true )
          randomWalkMean( allRayPaths, numberOfSectors, sectorAngle, R[i][j] );
        else
          absSum( allRayPaths, numberOfSectors, sectorAngle, R[i][j] );
          //sumAbs( allRayPaths, numberOfSectors, sectorAngle, R[i][j] );
 
        //cout << "analytical mean Absolute sum: " << R[i][j][0] << endl;

        for ( k = 0; k < numberOfSectors; k++ )
        {
          if ( cabs(Rmin[i][k]) < cabs(R[i][j][k]) )
            Rmin[i][k] = R[i][j][k];
        }
        allRayPaths.clearAndClearSubList();
      }
    }

    for ( k = 0; k < numberOfSectors; k++ )
    {
      Rtotal[i] += Rmin[i][k];
    }
  }

  // generate maximum power EIRP

  for ( i = 0; i < numberOfServers; i++ )
  {
    totalCheck = 0.0;
    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( Rmin[i][k] != 0 )
      {
        G[i][k] = cabs( 1.0 / numberOfSectors * Rtotal[i] / Rmin[i][k] );
      }
      else
      {
        cout << "no fields in one of the sectorized antennas" << endl;
      }
    }

    totalGains[i] = 0.0;
    for ( k = 0; k < numberOfSectors; k++ )
      totalGains[i] += G[i][k];

    for ( k = 0; k < numberOfSectors; k++ )
      G[i][k] = G[i][k] / cabs(totalGains[i]);
  }

  maximumPower = storageObject.CreateDoubleVector( numberOfServers );
  for ( i = 0; i < numberOfServers; i++ )
  {
    maximumPower[i] = parameterObject.maximumPermissiblePower( btsObject, 
                                                               Rtotal[i] );
    maximumPower[i] *= cabs(totalGains[i]);
    //cout << "Power [" << i << "] = " << maximumPower[i] << endl;
  }

  // test the objective function and count how many points meet target SIR
  answer = objectiveFunction( isCapacityExceeded, l, t, maximumPower, G, 
                              numberOfServers, numberOfSectors, centres,
                              prevCentres, buildingObject, btsObject, 
                              parameterObject, capacityGrid, targetSIR,
                              LOSSLESS, writeBestServer, fieldStrength );

  // delete allocated memory
  storageObject.DeleteComplex3dMatrix( R, numberOfServers, numberOfServers );
  storageObject.DeleteComplexMatrix( Rmin, numberOfServers );
  storageObject.DeleteComplexMatrix( G, numberOfServers );
  storageObject.DeleteComplexVector( Rtotal );
  storageObject.DeleteComplexVector( totalGains );
  storageObject.DeleteDoubleVector( maximumPower );
  centres.clear();
  prevCentres.clear();

  return answer;
} 

double CDownlink::objective( boolean& isCapacityExceeded,
                                 int&     l, double   t,
                                 int      numberOfSectors,
                                 const    CVector<double>&  vcentres,
                                 const    CVector<double>&  oldCentres,
                                 const    CVector<double>&  minimisedPower,
                                          CBaseTransceiver& btsObject,
                                 const    CBuilding&  buildingObject,
                                 const    CCapacity&  capacityGrid,
                                          double*     targetSIR,
                                 const    SystemParameters& parameterObject,
                                 const    boolean&    writeBestServer,
                                          complex     ***fieldStrength )
                             
{
  CPathLoss          bestServerGrid;
  double             *maximumPower;
  CRayTracing        rayTracingObject;
  CPoint3d           userCapacityPoint;
  CMemoryStorage     storageObject;
  List<CRayPath>     allRayPaths;
  List<CPoint3d>     centres, prevCentres;
  complex            **G;
  complex            *Rtotal, **Rmin, ***R;
  double             sectorAngle;
  double             answer;
  complex            *totalGains;
  complex            totalCheck;
  int                i, j, k, numberOfServers;

  centres = vectorToCentres( vcentres );
  prevCentres = vectorToCentres( oldCentres );

  // find maximum power and gains for the base stations
  sectorAngle     = TwoPi / numberOfSectors;

  numberOfServers = centres.size();
  R    = storageObject.CreateComplex3dMatrix( numberOfServers, numberOfServers,
                                              numberOfSectors );
  Rmin = storageObject.CreateComplexMatrix( numberOfServers, numberOfSectors );
  G    = storageObject.CreateComplexMatrix( numberOfServers, numberOfSectors );
  Rtotal = storageObject.CreateComplexVector( numberOfServers );
  totalGains   = storageObject.CreateComplexVector( numberOfServers );

  for ( i = 0; i < numberOfServers; i++ )
  {
    btsObject.setPosition( centres.GetItem(i) );
    for ( j = 0; j < numberOfServers; j++ )
    {
      if ( i != j )
      {
        rayTracingObject.MimoRayTracing( allRayPaths, buildingObject, btsObject,
                                         centres.GetItem(j), LOSSLESS ); 
    
        //meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, R[i][j] );
        //cout << "About to perfrom random Walk " << i << j << endl;
        if ( btsObject.randomWalk() == true )
          randomWalkMean( allRayPaths, numberOfSectors, sectorAngle, R[i][j] );
        else
          absSum( allRayPaths, numberOfSectors, sectorAngle, R[i][j] );
          //sumAbs( allRayPaths, numberOfSectors, sectorAngle, R[i][j] );
        //cout << "analytical mean Absolute sum: " << R[i][j][0] << endl;

        for ( k = 0; k < numberOfSectors; k++ )
        {
          if ( cabs(Rmin[i][k]) < cabs(R[i][j][k]) )
            Rmin[i][k] = R[i][j][k];
        }
        allRayPaths.clearAndClearSubList();
      }
    }

    for ( k = 0; k < numberOfSectors; k++ )
    {
      Rtotal[i] += Rmin[i][k];
    }
  }

  for ( i = 0; i < numberOfServers; i++ )
  {
    totalCheck = 0.0;
    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( Rmin[i][k] != 0 )
      {
        G[i][k] = cabs( 1.0 / numberOfSectors * Rtotal[i] / Rmin[i][k] );
      }
      else
      {
        cout << "no fields in one of the sectorized antennas" << endl;
      }
    }
    totalGains[i] = 0.0;
    for ( k = 0; k < numberOfSectors; k++ )
      totalGains[i] += G[i][k];

    for ( k = 0; k < numberOfSectors; k++ )
    {
      G[i][k] = G[i][k] / cabs(totalGains[i]);
    }
  }

  // generate maximum power EIRP

  maximumPower = storageObject.CreateDoubleVector( numberOfServers );
  for ( i = 0; i < numberOfServers; i++ )
  {
    // define the maximum power to be the result obtained by convex
    // optimisation minimisation: note gains already included from 
    // non-linear optimisation
    maximumPower[i]  = minimisedPower[i];
    //maximumPower[i] *= fabs(totalGains[i]);
  }

  // test the objective function and count how many points meet target SIR
  answer = objectiveFunction( 
                       isCapacityExceeded,
                       l, t, maximumPower, G, numberOfServers, numberOfSectors, 
                       centres, prevCentres, buildingObject, btsObject,
                       parameterObject, capacityGrid, targetSIR, LOSSLESS, 
                       writeBestServer, fieldStrength );

  // delete allocated memory
  storageObject.DeleteComplex3dMatrix( R, numberOfServers, numberOfServers );
  storageObject.DeleteComplexMatrix( Rmin, numberOfServers );
  storageObject.DeleteComplexMatrix( G, numberOfServers );
  storageObject.DeleteComplexVector( Rtotal );
  storageObject.DeleteComplexVector( totalGains );
  storageObject.DeleteDoubleVector( maximumPower );
  centres.clear();
  prevCentres.clear();

  return answer;
} 

void CDownlink::bestServers( List<CPoint3d>&     centres,
                                 CBaseTransceiver&   btsObject,
                                 const CBuilding&    buildingObject,
                                 const CCapacity&    capacityGrid )
{
  CPathLoss          pathLossGrid;
  CPathLoss          bestServerGrid;
  double             *currentPathLossVector, *maxPathLossVector;
  CRayTracing        rayTracingObject;
  CMatlab            matlabObject;
  CPoint3d           *receiverPointArray, userCapacityPoint;
  CMemoryStorage     storageObject;
  List<CRayPath>     allRayPaths;
  int                M, i, j, m, n, numberOfServers;

  pathLossGrid = CPathLoss( capacityGrid.xSouthWest(),
                            capacityGrid.ySouthWest(),
                            capacityGrid.xStepSize(),
                            capacityGrid.yStepSize(),
                            capacityGrid.xNumberOfPoints(),
                            capacityGrid.yNumberOfPoints() );

  bestServerGrid = CPathLoss( capacityGrid.xSouthWest(),
                              capacityGrid.ySouthWest(),
                              capacityGrid.xStepSize(),
                              capacityGrid.yStepSize(),
                              capacityGrid.xNumberOfPoints(),
                              capacityGrid.yNumberOfPoints() );

  M = capacityGrid.numberOfPoints();
  numberOfServers = centres.size();

  receiverPointArray = storageObject.CreatePoint3dVector( M );
  for ( i = 0; i < M; i++ )
    receiverPointArray[i] = pathLossGrid.getPoint( i, 
                                           capacityGrid.zSouthWest() );

  // initialise the maximum path-loss vector grid to be full of small numbers
  maxPathLossVector = storageObject.CreateDoubleVector( M );
  for ( m = 0; m < M; m++ )
    maxPathLossVector[m] = -120.0;

  // for each centre of capacity point, generate a path-loss grid and 
  // determine the best server for that grid, updating the best server
  // position as each receiver is added.
  for ( j = 0; j < numberOfServers; j++ )
  {
    btsObject.setPosition( centres.GetItem(j) );
    rayTracingObject = CRayTracing( buildingObject, btsObject, 
                                    pathLossGrid, receiverPointArray,
                                    LOSSLESS ); 

    // find the best server for each receiver position in the building
    currentPathLossVector = pathLossGrid.getPathLossVector();

    i = 0;
    // if the current path loss value is great that the maximum to date,
    // then make the current server the best server at that point
    for ( n = 0; n < bestServerGrid.yNumberOfPoints(); n++ )
    {
      for ( m = 0; m < bestServerGrid.xNumberOfPoints(); m++ )
      {
        if ( currentPathLossVector[i] > maxPathLossVector[i] )
        {
          maxPathLossVector[i] = currentPathLossVector[i];
          bestServerGrid.setPathLoss( double(j+1), m, n );
        }
        i++;
      }
    }
    delete [] currentPathLossVector;

    //cout << "Ray trace performed at Tx: " << btsObject.position() << endl;
  }
  matlabObject.pathLoss( bestServerGrid, "bestserver.txt" );

  // check where target SIR Was met

  storageObject.DeletePoint3dVector( receiverPointArray );
  storageObject.DeleteDoubleVector( maxPathLossVector  );
  allRayPaths.clearAndClearSubList();
}

double CDownlink::objectiveFunction( 
                            boolean& isCapacityExceeded,
                            int&   l,                double t,
                            double *maximumPower,    complex **gain,
                            int    numberOfServers,  int     numberOfSectors,
                                   List<CPoint3d>&   centres,
                                   List<CPoint3d>&   oldCentres,
                            const  CBuilding&        buildingObject,
                                   CBaseTransceiver& btsObject,
                            const  SystemParameters& parameterObject,
                            const  CCapacity&        capacityGrid,
                                   double*           targetSIR,
                            const  mediaType&        media,
                            const  boolean&          writeBestServer,
                                   complex           ***fieldStrength )
{
  double         objectiveFunction, objectiveFunction1, objectiveFunction2;
  double         objectiveFunctionT, objectiveFunctiona, objectiveFunctionb;
  complex        **totalField; 
  double         *totalInterference;
  int            currentServer, newBestServer;
  double         *pathLoss, *newSIR, sectorAngle, TwoK;
  double         phi, sensitivity;
  int            i, k, m, n, p, bestServer;
  ofstream       coutput( "checkSIR.txt" );
  CPathLoss      pathLossGrid;
  CMatlab        matlabObject;
  CRayTracing    rayTracingObject;
  List<CRayPath> allRayPaths;
  CMemoryStorage storageObject;
  CPoint3d       userCapacityPoint;
  double         *totalCapacity, *minimax;
  int            **bestServerMatrix, numberOfCapPoints;
  int            *bestServerIndices, *currentBestServer;
  CPoint3d       *oldCentresArray, *centresArray;
  string         bestServerFileName = "bestserver-";
  //boolean        offloaded = false;
  complex        temp1;

  centresArray = storageObject.CreatePoint3dVector( numberOfServers );
  oldCentresArray = storageObject.CreatePoint3dVector( numberOfServers );

  for ( i = 0; i < numberOfServers; i++ )
  {
    centresArray[i]    = centres.GetItem(i);
    oldCentresArray[i] = oldCentres.GetItem(i);
    //if ( centresArray[i] != oldCentresArray[i] )
      //cout << "Position: " << i << " changed" << endl;
  }

  TwoK              = 2.0 * btsObject.wavenumber();
  sectorAngle       = TwoPi / numberOfSectors;
  objectiveFunction = 0.0;
  objectiveFunction1 = 0.0;
  objectiveFunction2 = 0.0;
  numberOfCapPoints  = capacityGrid.numberOfPoints();
  
  totalCapacity     = storageObject.CreateDoubleVector( numberOfServers );
  totalField        = storageObject.CreateComplexMatrix( numberOfCapPoints,
                                                         numberOfServers );
  bestServerIndices = storageObject.CreateIntegerVector( numberOfServers );
  minimax           = storageObject.CreateDoubleVector( numberOfServers );
  bestServerMatrix  = storageObject.CreateIntegerMatrix( numberOfCapPoints,
                                                         numberOfServers );
  currentBestServer = storageObject.CreateIntegerVector( numberOfCapPoints );
  pathLoss          = storageObject.CreateDoubleVector( numberOfCapPoints );
  newSIR            = storageObject.CreateDoubleVector( numberOfCapPoints );
  totalInterference = storageObject.CreateDoubleVector( numberOfCapPoints );

  if ( writeBestServer == true )
    pathLossGrid = CPathLoss( capacityGrid.xSouthWest(),
                              capacityGrid.ySouthWest(),
                              capacityGrid.xStepSize(),
                              capacityGrid.yStepSize(),
                              capacityGrid.xNumberOfPoints(),
                              capacityGrid.yNumberOfPoints() );

  // weighted objective function for optimisation with more weight added
  // as the target SIR is met

  sensitivity = btsObject.receiverSensitivityUE();

  p = 0;
  l = 0;
  for ( n = 0; n < capacityGrid.yNumberOfPoints(); n++ )
  {
    for ( m = 0; m < capacityGrid.xNumberOfPoints(); m++ )
    {
      // extract the user capacity requirement at location r(m,n)
      userCapacityPoint = capacityGrid.getPoint( p, capacityGrid.zSouthWest() );
      //bestServerIndex   = int( bestServerGrid.getPathLoss( m, n ) - 1 );
  
      // calculate the field strength at that point r(m,n) due to all
      // base stations individually
      // note that if the centres positions have not changed then
      // ray-tracing values are identical to previous computation.
      // This also means that random walk mean is also the same.
      // We obtain n = numberOfServers speed up in solution using this fact
      // since gradient calculation only changes one search direction at a
      // time.
      // Note: gains change even if some servers do not move (sectorized case)
      for ( i = 0; i < numberOfServers; i++ )
      {
        temp1 = fieldStrength[p][i][0];

        if ( centresArray[i] != oldCentresArray[i] )
        {
          for ( k = 0; k < numberOfSectors; k++ )
            fieldStrength[p][i][k] = 0.0;

          btsObject.setPosition( centresArray[i] );
          rayTracingObject.MimoRayTracing( allRayPaths, buildingObject, 
                                           btsObject, userCapacityPoint, media );
        
          if ( btsObject.randomWalk() == true )
          {
            //cout << "About to perfrom random Walk " << m << " " << n << endl;
            randomWalkMean( allRayPaths, numberOfSectors, sectorAngle,
                            fieldStrength[p][i] );
           //randomWalkMean( allRayPaths, numberOfSectors, sectorAngle, gain[i],
                           //fieldStrength[p][i] );
          }
          else
            absSum( allRayPaths, numberOfSectors, sectorAngle,
                    fieldStrength[p][i] );
            //absSum( allRayPaths, numberOfSectors, sectorAngle, gain[i], 
                    //fieldStrength[p][i] );
            //sumAbs( allRayPaths, numberOfSectors, sectorAngle, gain[i], 
                    //fieldStrength[i] );
 
            //cout << "mean Absolute sum: " << fieldStrength[i][0] << endl;

          allRayPaths.clearAndClearSubList();
        }

        /*if ( centresArray[i] == oldCentresArray[i] )
        {
          if ( fieldStrength[p][i][0] != temp1 )
          {
            cout << centresArray[i] << "  ";
            cout << i << "  " << fieldStrength[p][i][0] << "  " << temp1 
                 << endl;
          }
        }*/
      }

      // now multiply in the gains, these are not multiplied into the 
      // random walk terms because they can vary in all servers even if
      // only one server is moved

      for ( i = 0; i < numberOfServers; i++ )
      {
        totalField[p][i] = 0.0;
        for ( k = 0; k < numberOfSectors; k++ )
          //totalField[p][i] += fabs( gain[i][k] ) 
                            //* fabs(fieldStrength[p][i][k])/ TwoK; 
                            //* qsqr(fabs(fieldStrength[p][i][k])/ TwoK); 
          totalField[p][i] += qsqr( cabs(fieldStrength[p][i][k])/ TwoK ); 
        totalField[p][i] = maximumPower[i] * totalField[p][i];
      }

      // order the best servers 1st, 2nd, ...
      sort( totalField[p], bestServerMatrix[p], numberOfServers );

      bestServer = bestServerMatrix[p][0];

      // always choose the 1st server to be the best initially
      // offloading a server is the only thing that can change this value
      currentBestServer[p] = 0;

      newSIR[p] = SIR( totalField[p], numberOfServers, bestServer, sensitivity,
                       totalInterference[p], pathLoss[p] ); 

      // determine which server is the best server at r(m,n)

      coutput << "SIR: " << newSIR[p] << " >  target: " << targetSIR[p] << "?" << endl; 
      if ( targetSIR[p] <= newSIR[p] )
      {
        l++;
        if ( writeBestServer == true )
        {
          if ( capacityGrid[p] == 0.0 )
            pathLossGrid.setPathLoss( -1, m, n );
          else
            pathLossGrid.setPathLoss( bestServer+1, m, n );
        }
      }
      totalCapacity[bestServer] += capacityGrid[p];
    
      p++;
    }
  }

  cout << l << " / " << p << " met target SIR ";

  // see if we exceed the capacity requirement for current server allocation
  isCapacityExceeded = false;
  for ( i = 0; i < numberOfServers; i++ )
  {
    if ( totalCapacity[i] > btsObject.serverCapacity() )
    {
      cout << " capacity exceeded at server: " << i << " ( " << totalCapacity[i] << " > " 
           << btsObject.serverCapacity() << " ) " << endl;
      isCapacityExceeded = true;
      /*offloaded = offloadServer( bestServerMatrix, capacityGrid, 
                                 sensitivity, totalCapacity, 
                                 currentBestServer, totalField, targetSIR,
                                 btsObject.serverCapacity(),
                                 numberOfServers, numberOfCapPoints, i );
      if ( offloaded == true )
      {
        cout << "(offloaded)";
        isCapacityExceeded = false;
      }*/
    }
  }
  cout << endl;
  
  coutput.close();
  phi = 0.8;

  p = 0;
  for ( n = 0; n < capacityGrid.yNumberOfPoints(); n++ )
  {
    for ( m = 0; m < capacityGrid.xNumberOfPoints(); m++ )
    {
      // if the best server has changed from being the first best server
      // to some other, then we must modify the objective function
      // accordingly, otherwise we do not get an accurate evaluation of
      // the gradient (giving up our updated search direction )
      if ( currentBestServer[p] != 0 )
      {
        currentServer = currentBestServer[p];
        newBestServer = bestServerMatrix[p][currentServer]; 
        newSIR[p] = SIR( totalField[p], numberOfServers, newBestServer,
                         sensitivity, totalInterference[p], pathLoss[p] ); 
      }
      
      objectiveFunctiona  = pathLoss[p];
      objectiveFunction1 += objectiveFunctiona;
      objectiveFunctionT  = t * objectiveFunctiona;
      if ( targetSIR[p] > newSIR[p] )
      {
        objectiveFunctionb  = ( targetSIR[p] * totalInterference[p]
                            - pathLoss[p] );
        objectiveFunction2 += objectiveFunctionb;
        objectiveFunctionT += objectiveFunctionb;
      }
      if ( objectiveFunctionT > minimax[bestServer] )
         minimax[bestServer] = objectiveFunctionT;
      p++;
    }
  }

  // define the minisum expression
  objectiveFunction = phi * ( t * objectiveFunction1 + objectiveFunction2 )/p;

  // define and add the minimax expression
  for ( i = 0; i < numberOfServers; i++ )
    objectiveFunction += (1.0-phi) * 1/numberOfServers * minimax[i];

  //cout << "obj: " << objectiveFunction << "  ";

  stringstream ss;
  ss << btsObject.numberOfReflections();

  if ( writeBestServer == true )
    matlabObject.pathLoss( pathLossGrid, bestServerFileName + btsObject.name() + "-r" + ss.str() + ".txt" );

  // clean up memory allocation
  storageObject.DeleteDoubleVector( totalCapacity );
  storageObject.DeleteDoubleVector( minimax );
  storageObject.DeleteComplexMatrix( totalField, numberOfCapPoints );
  storageObject.DeleteIntegerVector( bestServerIndices );
  storageObject.DeleteIntegerMatrix( bestServerMatrix, numberOfCapPoints );

  storageObject.DeletePoint3dVector( centresArray );
  storageObject.DeletePoint3dVector( oldCentresArray );

  storageObject.DeleteIntegerVector( currentBestServer );
  storageObject.DeleteDoubleVector( pathLoss );
  storageObject.DeleteDoubleVector( newSIR );
  storageObject.DeleteDoubleVector( totalInterference );

  return objectiveFunction;
} 

List<CPoint3d>& CDownlink::vectorToCentres(const CVector<double>& vcentres)
{
  List<CPoint3d>  *centres;
  int             i, k, N;
  double          x, y, z;
  CPoint3d        p;

  centres = new List<CPoint3d>;
  N       = int(vcentres.size()) / 3;      

  for ( i = 0; i < N; i++ )
  {
    k = i * 3;
    x = vcentres[k];
    y = vcentres[k+1];
    z = vcentres[k+2];
    centres->append( CPoint3d( x, y, z ) );
  }

  return *centres;
}

CVector<double> CDownlink::convertCentresToVector( 
                                   List<CPoint3d>& centres )
{
  CVector<double> XYZnVector;
  int             i, k;
  CPoint3d        p;

  XYZnVector = CVector<double>( centres.size() * 3 );

  for ( i = 0; i < centres.size(); i++ )
  {
    p = centres.GetItem(i);
    k = i * 3;
    XYZnVector.set( p.x, k );
    XYZnVector.set( p.y, k+1 );
    XYZnVector.set( p.z, k+2 );
  }

  return XYZnVector;
}

CVector<double> CDownlink::gradient( 
                                       double            smallerSize,
                                       double            t,
                                       int               numberOfSectors,
                                       CVector<double>&  oldCentres,
                                       CBaseTransceiver& btsObject,
                                 const CBuilding&        buildingObject,
                                 const CCapacity&        capacityGrid,
                                       double*           targetSIR,
                                 const SystemParameters& parameterObject,
                                       complex           ***fieldStrength )
{
  CVector<double> gradfx;
  CVector<double> newCentres, prevCentres;
  double          delta, fold, fnew;
  int             i, l;
  boolean         isCapacityExceeded;
  cout << "calculate gradient at " << oldCentres.size() << " points" << endl;

  prevCentres = oldCentres;

  delta  = 0.075/qsqr(smallerSize);
  //delta  = 0.0375/qsqr(smallerSize);
  gradfx = CVector<double>( oldCentres.size() );
  fold = objective( isCapacityExceeded, l, t, numberOfSectors, oldCentres, 
                    prevCentres, btsObject, buildingObject, capacityGrid,
                    targetSIR, parameterObject, false, fieldStrength );

  newCentres = prevCentres;

  for ( i = 0; i < oldCentres.size(); i++ )
  {
    // for each item in the variable list change one variable by a value
    // delta and do not change any other, because we need the derivative
    // in each variable.
  //if ( fmod( double(i), 3.0 ) != 0.0 )
  {
    prevCentres = newCentres;
    newCentres  = oldCentres;
    newCentres.set( oldCentres[i] + delta, i ); 
 
    fnew = objective( isCapacityExceeded, l, t, numberOfSectors, newCentres, 
                      prevCentres, btsObject, buildingObject, capacityGrid, 
        targetSIR, parameterObject, false, fieldStrength ); 

    if ( isCapacityExceeded == false ) 
    {
      gradfx.set( (fnew-fold)/delta, i );
    }
  }
  }
  newCentres.clear();
  prevCentres.clear();

  return gradfx;
}

boolean CDownlink::annealing(      double            smallerSize,
                                       double            t,
                                       int               numberOfSectors,
                                       CVector<double>&  oldCentres,
                                       CBaseTransceiver& btsObject,
                                 const CBuilding&        buildingObject,
                                 const CCapacity&        capacityGrid,
                                       double*           targetSIR,
                                 const SystemParameters& parameterObject,
                                       complex           ***fieldStrength )
                                       
{
  CVector<double> newCentres, prevCentres;
  double          delta, fnew;
  int             i, l;
  boolean         isCapacityExceeded;
  cout << "calculate gradient at " << oldCentres.size() << " points" << endl;

  delta  = 0.075/qsqr(smallerSize);
  //delta  = 0.0375/qsqr(smallerSize);

  newCentres = oldCentres;

  for ( i = 0; i < oldCentres.size(); i++ )
  {
    // for each item in the variable list change one variable by a value
    // delta and do not change any other, because we need the derivative
    // in each variable.
    //if ( fmod( double(i), 3.0 ) != 0.0 )
  {
    prevCentres = newCentres;
    newCentres  = oldCentres;
    newCentres.set( oldCentres[i] + delta, i ); 
 
    fnew = objective( isCapacityExceeded, l, t, numberOfSectors, newCentres, 
                      prevCentres, btsObject, buildingObject, capacityGrid, 
        targetSIR, parameterObject, false, fieldStrength ); 

    if ( isCapacityExceeded == false ) 
    {
      oldCentres = newCentres;
      return false;
    }
    }
  }
  newCentres.clear();
  prevCentres.clear();

  return true;
}

CVector<double> CDownlink::steepestDescentSUMT(
                                      List<CPoint3d>&     centres,
                                      List<CBoundingBox>& cells,
                                      CBaseTransceiver&   btsObject,
                                      int                 numberOfSectors,
                                const CBuilding&          buildingObject,
                                const CCapacity&          capacityGrid,
                                const SystemParameters&   parameterObject,
                                      double              stoppingPercentage )
{
  CVector<double>  V, Vold, x, gradfx, xPlusVr;
  CBoundingBox     area;
  CPoint3d         pt;
  double           t, r, Vsd, rOriginal;
  int              xyorzcomp, numberOfServers;
  int              e_i, cellIndex, numberOfIterations = 2000;
  int              i, ii, m, n, k, l, p, kk, testConvergence;
  int              numberOfCapPoints;
  double           fnextPoint, fcurrent, fprevious;
  double           beta = 0.5, alpha = 0.3, eta = 1.0e-15;
  double           epsilon = 1.0e-2, mu = 3.0, t0 = 1.0e-4;
  double           stoppingFactor;
  boolean          fFeasible, isCapacityExceeded, metOurInitialTarget;
  boolean          writeBestServerFile, dontWriteBestServerFile;
  ofstream         coutput( "errorcheck.txt" );
  CMatrix<double>  A;
  CVector<double>  p0, c, b, p1;
  CVector<double>  xprev, xzero;
  double           *targetSIR;
  complex          ***fieldStrength;
  CLinearProgram   linearProgrammingObject;
  CMemoryStorage   storageObject;
  int              convexNumber;

  srand( (unsigned) time( NULL ) );
  //updateLargestSIR( btsObject, 0, x );

  numberOfCapPoints = capacityGrid.numberOfPoints();
  stoppingFactor = stoppingPercentage / 100.0;
  targetSIR = storageObject.CreateDoubleVector( numberOfCapPoints );

  setTargetSIR( capacityGrid, parameterObject, targetSIR );

  // set up sizes of variables
  numberOfServers = centres.size();
  n = numberOfServers * 3;
  m = capacityGrid.numberOfPoints();
  //xpath = zeros(2,numberOfIterations);
  fieldStrength     = storageObject.CreateComplex3dMatrix( numberOfCapPoints,
                                                           numberOfServers,
                                                           numberOfSectors );
  // set initial values
  xzero = CVector<double>(n);
  xprev = CVector<double>(n);
  x = convertCentresToVector( centres );
  t = t0;

  // initial vector sizes
  V = CVector<double>( n );
  gradfx = CVector<double>( n );

  writeBestServerFile     = true;
  dontWriteBestServerFile = false;

  cout << " f(x)    iteration location[0]: ";
  fnextPoint   = objective( isCapacityExceeded, l, t, numberOfSectors, x,
                            xprev, btsObject, buildingObject, capacityGrid, 
                            targetSIR, parameterObject, dontWriteBestServerFile,
                            fieldStrength );
  updateLargestSIR( btsObject, l, x );

  ii = 1;
  if ( isCapacityExceeded == true )
  {
    do
    {
      isCapacityExceeded = annealing( 1.0/ii, t, numberOfSectors, x, btsObject,
                                      buildingObject, capacityGrid, targetSIR,
                                      parameterObject, fieldStrength );
      ii++;
    } while ( isCapacityExceeded == true );
  }
  
  metOurInitialTarget = false;

  // sequential part of SUMT
  r = 0.0; 
  for ( i = 0; i < numberOfIterations; i++ )
  {
    gradfx.setOnes();

    fFeasible = true;
    testConvergence = 0;
    k = 0;
    while ( gradfx.abs() > eta )
    {
      cout << "gradient: " << gradfx.abs() << endl;
      xprev = x;
      x = x+V*r;
      fcurrent = fnextPoint;
      p = l;
      // use an L_infinity-norm to find the steepest descent direction
      // this direction is in the direction of maximum increase
      // define the diagonal term d

      gradfx = gradient( double(fmod(k,3)+1), t, numberOfSectors, x, 
                         btsObject, buildingObject, capacityGrid, targetSIR,
                         parameterObject, fieldStrength );
      Vsd    = gradfx.linfinitynorm(e_i); // the L_infinity-norm of grad f

      cout << "Vsd : " << Vsd << endl;
      if ( Vsd == 0.0 ) 
      {
        ii = 1; 
        do 
        {
          gradfx = gradient( 1.0/sqrt(2.0)/ii, t, numberOfSectors, x,
                             btsObject, buildingObject, capacityGrid, 
                             targetSIR, parameterObject, fieldStrength );
          Vsd = gradfx.linfinitynorm(e_i);
          ii++;
        } while ( Vsd == 0.0 );
      }

      if ( Vsd == 0.0 )
      {
        cout << "did not converge properly, could have problems later" << endl;
      }

      V = CVector<double>(n);
      V.set( -gradfx[e_i], e_i );
      cellIndex  = int( e_i / 3.0 );
      xyorzcomp  = int( fmod( double(e_i), 3.0 ) );
      area       = cells.GetItem( cellIndex );

      // different direction methods can be applied
      //if ( k > 0 )
        //V = V + Vold * qsqr( V.euclideanNorm() / Vold.euclideanNorm() );
      //Vold = V;
  
      // backtracking line search algorithm restricted to boxes
      r = 1.0;
      if ( -gradfx[e_i] > 0 )
      {
        if ( xyorzcomp == 0 )
          r = ( area.xNorthEast_ - x[e_i] ) / ( -gradfx[e_i] * 1.2 );
        else if ( xyorzcomp == 1 )
          r = ( area.yNorthEast_ - x[e_i] ) / ( -gradfx[e_i] * 1.2 );
        else if ( xyorzcomp == 2 )
          r = ( area.zNorthEast_ - x[e_i] ) / ( -gradfx[e_i] * 1.2 );
      }
      else
      {
        if ( xyorzcomp == 0 )
          r = ( x[e_i] - area.xSouthWest_ ) / ( gradfx[e_i] * 1.2 );
        else if ( xyorzcomp == 1 )
          r = ( x[e_i] - area.ySouthWest_ ) / ( gradfx[e_i] * 1.2 );
        else if ( xyorzcomp == 2 )
          r = ( x[e_i] - ( area.zSouthWest_ + 1.5 ) ) / ( gradfx[e_i] * 1.2 );
      }

      for ( ii = 0; ii < n; ii++ )
        coutput << x[ii] << " ";
      coutput << endl;
      xPlusVr    = x+V*r;
      cout << "V:  ";
      for ( ii = 0; ii < n; ii++ )
        cout << " " << ii << ":" << V[ii];
      cout << endl;

      cout << "  iteration f(x+d) gives:  ";
      fnextPoint = objective( isCapacityExceeded, l, t, numberOfSectors, 
                              xPlusVr, xzero, btsObject, buildingObject,
                              capacityGrid, targetSIR, parameterObject,
                              dontWriteBestServerFile, fieldStrength );
      fFeasible = testFeasibility( xPlusVr, cellIndex, area, pt );

      if ( double(l)/double(m) > stoppingFactor && p <= m 
           && isCapacityExceeded == false )
      {
        cout << "met " << double(l)*100.0/double(m) << "% of total SIR" << endl;
        cout << l << " / " << m << endl;
        updateLargestSIR( btsObject, l, x );
        break;
      }

      coutput << " f(x+Vr) iteration location[" << i << "]: ";
      for ( ii = 0; ii < n; ii++ )
        coutput << x[ii] << " ";
      coutput << endl;
      fprevious = fcurrent + alpha * r * ( gradfx * V );

      kk = 0;
      while ( fnextPoint > fprevious || fFeasible == false 
              || isCapacityExceeded == true )
      {
        // if there is an increase in the number of SIR point met then break
        if ( l > p && fFeasible == true && isCapacityExceeded == false )
        {
          p = l;
          cout << "return because improved: " << l << " / " << m << endl;
          updateLargestSIR( btsObject, l, x );
          break;
        }

        r         *= beta;
        xPlusVr    = x+V*r;
        cout << "  looping over f(x+d) calculated: ";
        fnextPoint = objective( isCapacityExceeded, l, t, numberOfSectors,
                                xPlusVr, x, btsObject, buildingObject, 
                                capacityGrid, targetSIR, parameterObject,
                                dontWriteBestServerFile, fieldStrength );

        // if we reach an acceptable SIR requirement without breaking
        // our capacity requirements then break out of while loop
        if ( double(l)/double(m) > stoppingFactor && l <= m
              && isCapacityExceeded == false )
        {
          p = l;
          cout << "met " << double(l)*100.0/double(m) << "% of total SIR" 
               << endl;
          cout << l << " / " << m << endl;
          updateLargestSIR( btsObject, l, x );
          break;
        }

        fFeasible  = testFeasibility( xPlusVr, cellIndex, area, pt );

        // if we find that we are inside a wall then need to reduce step-size
        if ( buildingObject.locateConvexNumber( pt, convexNumber ) == false )
          continue;

        // if iterating with the same number of SIR values for a long time
        // then break
        if ( kk > 3 )
        {
          cout << "returned since looping too much" << endl;
          break;   
        }

        // update the iterator saying that the number of SIR points is unchanged
        if ( l >= p && fFeasible == true && isCapacityExceeded == false )
        {
          kk += 1;
        }

        fprevious  = fcurrent + alpha * r * ( gradfx * V );

        // if value of r is getting very small then stepping is far too small
        if ( r < 1.e-8 )
        {
          cout << "we need to cut down the number of iterations here" << endl;
          if ( buildingObject.locateConvexNumber( pt, convexNumber ) == false )
          {
            cout << "point inside wall so we need to get out of it" << endl;
            r = 1.0;
            xPlusVr    = x-V*r*2.0;
          }
          else 
            break;
        }

        coutput << " while iteration location[" << i << "]: ";
        for ( ii = 0; ii < n; ii++ )
          coutput << x[ii] << " ";
        coutput << endl;
      }

      // if we have completed one full loop and number of points
      // 
      if ( p > l && metOurInitialTarget == true )
      {
        l = p;
        cout << "f(x) gave better SIR target so revert to it" << endl;
        r = 0.0;
        break;
      }
      if ( r < 1.e-8 )
      {
        cout << "we need to cut down the number of iterations here 2" << endl;
        break;
      }
      k++;

      if ( double(l)/double(m) > stoppingFactor && l <= m )
      {
        break;
      }
    }

    if ( double(l)/double(m) > stoppingFactor && l <= m )
    {
      p = l;
      cout << "met " << double(l)*100.0/double(m) << "% of total SIR" 
           << endl;
      cout << l << " / " << m << endl;
      break;
    }

    coutput << " met target iteration location[" << i << "]: ";
    for ( ii = 0; ii < n; ii++ )
      coutput << x[ii] << " ";
    coutput << "  *************" << t << "**************" << endl;
    metOurInitialTarget = true;
  
    // condition for stopping algorithm
    if ( m / t < epsilon ) 
      break;

    // we have met all SIR targets so stop since no reason to continue
    if ( p == m )
      break;
  
    t *= mu;
    break;

    for ( ii = 0; ii < n; ii++ )
      cout << x[ii] << " ";
    cout << endl;
    cout << "**** t = " << t << " must be " << m / epsilon << " to finish ****"
         << endl;
    cout << "writing best server results to a file" << endl;
    
    objective( isCapacityExceeded, l, t, numberOfSectors, x+V*r, x, btsObject,
               buildingObject, capacityGrid, targetSIR, parameterObject, 
               writeBestServerFile, fieldStrength );
  }

  coutput.close();
      
  ofstream bslocations("locations.txt");
  for ( ii = 0; ii < n; ii++ )
    bslocations << x[ii] << " ";
  bslocations << endl;

  xprev = x;
  x = x + V*r;
  cout << "calculating exact number Of constraints: " << endl;
  objective( isCapacityExceeded, l, t, numberOfSectors, x, xprev, btsObject, 
             buildingObject, capacityGrid, targetSIR, parameterObject, 
             writeBestServerFile, fieldStrength );

  cout << "Initial value of positions is: " << endl;
  for ( i = 0; i < x.size(); i++ )
    cout << "  " << x[i] << endl;

  cout << "creating Convex Optimisation constraint matrix" << endl;
  useObjectiveToCreateConvexConstraintMatrix( A, b, p0, isCapacityExceeded,
                                              l, t, numberOfSectors, x, 
                                              btsObject, buildingObject,
                                              capacityGrid, targetSIR,
                                              parameterObject, 
                                              writeBestServerFile );
  cout << "Initial value of Powers is: " << endl;
  for ( i = 0; i < p0.size(); i++ )
    cout << "  " << p0[i] << endl;

  if ( isCapacityExceeded == true )
  {
    cout << "you exceeded the capacity" << endl;
    exit(1);
  }
  c = CVector<double>( p0.size() );


  // define cost to be a 1 vector
  c.setOnes();

  //eta = 1.0e-6;
  mu  = 5;
  t0  = 1.0e-4;
  epsilon = 1.0e-20;
  cout << "Starting Steepest Descent Convex Optimisation of powers: " << endl;
  p1 = linearProgrammingObject.steepestDescentSUMT( beta, alpha, eta, epsilon,
                                                    mu, t0, A.rowSize(),
                                                    A.columnSize(), c, A, 
                                                    p0, b );
  for ( i = 0; i < p1.size(); i++ )
    cout << "end result:  " << p1[i] << endl;

  cout << "recalculating SIR on whole region to see it hasn't decreased" 
       << endl;

  objective( isCapacityExceeded, l, t, numberOfSectors, x, xzero, p1,
             btsObject, buildingObject, capacityGrid, targetSIR, 
             parameterObject, writeBestServerFile, fieldStrength );

  //cout << "Powers not included but changed to random walk" << endl;
  //btsObject.setRandomWalk( true );
  objective( isCapacityExceeded, l, t, numberOfSectors, x, xzero, btsObject, 
             buildingObject, capacityGrid, targetSIR, parameterObject, 
             writeBestServerFile, fieldStrength );

  xPlusVr.clear();
  gradfx.clear();
  Vold.clear();
  V.clear();
  A.clear();
  c.clear();
  b.clear();
  p0.clear();
  storageObject.DeleteDoubleVector( targetSIR );
  storageObject.DeleteComplex3dMatrix( fieldStrength, numberOfCapPoints,
                                       numberOfServers );

  return x;
}

void CDownlink::updateLargestSIR( const CBaseTransceiver& btsObject, int l, CVector<double>& x )
{
  stringstream ss;
  ss << btsObject.numberOfReflections();
  string fileName = "largestSIR-" + btsObject.name() + "-r" + ss.str() +  ".txt";
  string pathFollowed = "pathFollowed-" + btsObject.name() + "-r" + ss.str() +  ".txt";

  if ( l == 0 )
  {
    ofstream coutput( fileName.c_str() );
    coutput << "0" << endl;
    coutput.close();

    ofstream coutput2( pathFollowed.c_str() );
    coutput2 << l << "  ";
    for ( int i = 0; i < x.size(); i++ )
      coutput2 << x[i] << "  ";
    coutput2 << endl;
    coutput2.close();

    return;
  }
  else
  {
    ifstream cinput( fileName.c_str() );
    int      l2;

    cinput >> l2;
    cinput.close();

    if ( l2 < l )
    {
      ofstream coutput( fileName.c_str() );
    
      coutput << l << "  ";
      for ( int i = 0; i < x.size(); i++ )
        coutput << x[i] << "  ";
      coutput << endl;
      coutput.close();

      ofstream coutput2( pathFollowed.c_str(), ofstream::out | ofstream::app );
     coutput2 << l2 << "  ";
      for ( int i = 0; i < x.size(); i++ )
        coutput2 << x[i] << "  ";
      coutput2 << endl;
      coutput2.close();
    }
  }
}

boolean CDownlink::testFeasibility( CVector<double>& x, int cellIndex,
                                        CBoundingBox& area, CPoint3d& pt )
{
  pt  = CPoint3d( x[cellIndex*3], x[cellIndex*3+1], x[cellIndex*3+2] );
  return area.intersection( pt );
}

void CDownlink::useObjectiveToCreateConvexConstraintMatrix( 
                                 CMatrix<double>& A, CVector<double>& b,
                                 CVector<double>& p0,
                                 boolean& isCapacityExceeded,
                                 int&     numberOfConstraints, 
                                 double   t, int      numberOfSectors, 
                                 const    CVector<double>&  vcentres,
                                          CBaseTransceiver& btsObject,
                                 const    CBuilding&  buildingObject,
                                 const    CCapacity&  capacityGrid,
                                          double*     targetSIR,
                                 const    SystemParameters& parameterObject,
                                 const    boolean&    writeBestServer )
                             
{
  CPathLoss          bestServerGrid;
  double             *maximumPower;
  CRayTracing        rayTracingObject;
  CPoint3d           userCapacityPoint;
  CMemoryStorage     storageObject;
  List<CRayPath>     allRayPaths;
  List<CPoint3d>     centres;
  complex            **G;
  complex            *Rtotal, **Rmin, ***R;
  double             sectorAngle, TwoK;
  complex            *totalGains;
  complex            totalCheck;
  int                i, j, k, l, M, N;
  int                numberOfServers;

  centres = vectorToCentres( vcentres );

  // find maximum power and gains for the base stations
  numberOfServers = centres.size();
  sectorAngle     = TwoPi / numberOfSectors;
  TwoK            = 2.0 * btsObject.wavenumber();

  R    = storageObject.CreateComplex3dMatrix( numberOfServers, numberOfServers,
                                              numberOfSectors );
  Rmin = storageObject.CreateComplexMatrix( numberOfServers, numberOfSectors );
  Rtotal = storageObject.CreateComplexVector( numberOfServers );
  G    = storageObject.CreateComplexMatrix( numberOfServers, numberOfSectors );
  totalGains   = storageObject.CreateComplexVector( numberOfServers );
  maximumPower = storageObject.CreateDoubleVector( numberOfServers );

  for ( i = 0; i < numberOfServers; i++ )
  {
    btsObject.setPosition( centres.GetItem(i) );
    for ( j = 0; j < numberOfServers; j++ )
    {
      if ( i != j )
      {
        rayTracingObject.MimoRayTracing( allRayPaths, buildingObject, btsObject,
                                         centres.GetItem(j), LOSSLESS ); 
    
        //meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, R[i][j] );
        if ( btsObject.randomWalk() == true )
          randomWalkMean( allRayPaths, numberOfSectors, sectorAngle, R[i][j] );
        else
          absSum( allRayPaths, numberOfSectors, sectorAngle, R[i][j] );
          //sumAbs( allRayPaths, numberOfSectors, sectorAngle, R[i][j] );

        for ( k = 0; k < numberOfSectors; k++ )
        {
          if ( cabs(Rmin[i][k]) < cabs(R[i][j][k]) )
            Rmin[i][k] = R[i][j][k];
        }
        allRayPaths.clearAndClearSubList();
      }
    }

    for ( k = 0; k < numberOfSectors; k++ )
    {
      Rtotal[i] += Rmin[i][k];
      //cout << "max path loss for server" << i << " sector " << k 
           //<< ": " << Rmin[i][k] << endl;
    }
  }

  // generate maximum power EIRP
  for ( i = 0; i < numberOfServers; i++ )
  {
    maximumPower[i] = parameterObject.maximumPermissiblePower( btsObject, 
                                                               Rtotal[i] );
    //cout << "total path loss for server" << i << ": " << Rtotal[i] << endl;
    //cout << "server " << i << ": " << centres.GetItem(i) << " power: "
         //<< maximumPower[i] << endl;
  } 
  
  for ( i = 0; i < numberOfServers; i++ )
  {
    totalCheck = 0.0;
    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( Rmin[i][k] != 0 )
      {
        G[i][k] = 1.0 / numberOfSectors * Rtotal[i] / Rmin[i][k];
      }
      else
      {
        cout << "no fields in one of the sectorized antennas" << endl;
      }
    }
    totalGains[i] = 0.0;
    for ( k = 0; k < numberOfSectors; k++ )
      totalGains[i] += G[i][k];

    for ( k = 0; k < numberOfSectors; k++ )
      G[i][k] = G[i][k] / cabs(totalGains[i]);

    maximumPower[i] *= cabs(totalGains[i]);
  }

  M = numberOfConstraints;
  N = numberOfSectors * numberOfServers;

  // compute matrix size and initial value of p;

  p0   = CVector<double>(N);
  A    = CMatrix<double>(M+numberOfServers*numberOfSectors*2,N);
  b    = CVector<double>(M+numberOfServers*numberOfSectors*2);

  // set up powers in sectors for starting convex optimisation
  j = 0;
  for ( i = 0; i < numberOfServers; i++ )
  {
    for ( k = 0; k < numberOfSectors; k++ )
    {
      p0.set( maximumPower[i], j );
      j++;
    }
  }


  // test the objective function and count how many points meet target SIR
  formConvexOptimisationMatrix( A, b, isCapacityExceeded,
                       l, t, maximumPower, G, numberOfServers, numberOfSectors, 
                       centres, buildingObject, btsObject, parameterObject,
                       capacityGrid, targetSIR, LOSSLESS, writeBestServer );
  
  if ( l != numberOfConstraints )
  {
    cout << "Error: number of constraints is not equal to actual amount"
         << endl;
    cout << "l= " << l << "   numberOfConstraints= " << numberOfConstraints << endl;
    //exit(1);
  }

  k = l;
  for ( i = 0; i < numberOfServers; i++ )
  {
    for ( j = 0; j < numberOfSectors; j++ )
    {
      // set the upper bound on the powers to be 20dB = 10^(20/10) = 100
      A.set(   1.0, k, k-l );
      b.set( 100.0, k );
      // set the lower bound on the powers to be zero (AP < b )
      // -P_i < 0 equivalent to P_i > 0
      A.set( -1.0, k+numberOfServers*numberOfSectors, k-l );
      b.set(  0.0, k+numberOfServers*numberOfSectors );
      k++;
    }
  }

  // delete allocated memory
  storageObject.DeleteComplex3dMatrix( R, numberOfServers, numberOfServers );
  storageObject.DeleteComplexMatrix( Rmin, numberOfServers );
  storageObject.DeleteComplexMatrix( G, numberOfServers );
  storageObject.DeleteComplexVector( Rtotal );
  storageObject.DeleteComplexVector( totalGains );
  storageObject.DeleteDoubleVector( maximumPower );
  centres.clear();
} 

void CDownlink::formConvexOptimisationMatrix( 
                            CMatrix<double>& A, CVector<double>& b,
                            boolean& isCapacityExceeded,
                            int&   l,                double t,
                            double *maximumPower,    complex **gain,
                            int    numberOfServers,  int     numberOfSectors,
                                   List<CPoint3d>&   centres,
                            const  CBuilding&        buildingObject,
                                   CBaseTransceiver& btsObject,
                            const  SystemParameters& parameterObject,
                            const  CCapacity&        capacityGrid,
                                   double*           targetSIR,
                            const  mediaType&        media,
                            const  boolean&          writeBestServer )
{
  double         objectiveFunction, objectiveFunction1, objectiveFunction2;
  complex        **fieldStrength, **totalField, tempTotal;
  double         interferingField;
  double         newSIR, TwoK, sectorAngle, pathLoss;
  double         userCapacity, sensitivity, totalInterference;
  int            i, k, m, n, p, bestServer;
  //boolean        offloaded;
  ofstream       coutput( "checkSIR.txt" );
  CPathLoss      pathLossGrid;
  CMatlab        matlabObject;
  CRayTracing    rayTracingObject;
  List<CRayPath> allRayPaths;
  CMemoryStorage storageObject;
  CPoint3d       userCapacityPoint;
  int            **bestServerMatrix, numberOfCapPoints;
  int            *currentBestServer;
  double         *totalCapacity;

  TwoK              = 2.0 * btsObject.wavenumber();
  objectiveFunction = 0.0;
  objectiveFunction1 = 0.0;
  objectiveFunction2 = 0.0;
  numberOfCapPoints  = capacityGrid.numberOfPoints();
  sectorAngle   = TwoPi / numberOfSectors;
  fieldStrength = storageObject.CreateComplexMatrix( numberOfServers,
                                                     numberOfSectors );
  totalCapacity = storageObject.CreateDoubleVector( numberOfServers );
  totalField        = storageObject.CreateComplexMatrix( numberOfCapPoints,
                                                         numberOfServers );
  //bestServerIndices = storageObject.CreateIntegerVector( numberOfServers );
  //minimax           = storageObject.CreateDoubleVector( numberOfServers );
  bestServerMatrix  = storageObject.CreateIntegerMatrix( numberOfCapPoints,
                                                         numberOfServers );
  currentBestServer = storageObject.CreateIntegerVector( numberOfCapPoints );


  if ( writeBestServer == true )
    pathLossGrid = CPathLoss( capacityGrid.xSouthWest(),
                              capacityGrid.ySouthWest(),
                              capacityGrid.xStepSize(),
                              capacityGrid.yStepSize(),
                              capacityGrid.xNumberOfPoints(),
                              capacityGrid.yNumberOfPoints() );

  // weighted objective function for optimisation with more weight added
  // as the target SIR is met
  sensitivity = btsObject.receiverSensitivityUE();
  p = 0;
  l = 0;
  for ( n = 0; n < capacityGrid.yNumberOfPoints(); n++ )
  {
    for ( m = 0; m < capacityGrid.xNumberOfPoints(); m++ )
    {
      // extract the user capacity requirement at location r(m,n)
      userCapacityPoint = capacityGrid.getPoint( p, capacityGrid.zSouthWest() );
      userCapacity      = capacityGrid.getCapacity( m, n );
      //bestServerIndex   = int( bestServerGrid.getPathLoss( m, n ) - 1 );
  
      // calculate the field strength at that point r(m,n) due to all
      // base stations individually
      for ( i = 0; i < numberOfServers; i++ )
      {
        btsObject.setPosition( centres.GetItem(i) );
        rayTracingObject.MimoRayTracing( allRayPaths, buildingObject, 
                                         btsObject, userCapacityPoint, media );
        //meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, gain[i],
                    //fieldStrength[i] );
        if ( btsObject.randomWalk() == true )
          randomWalkMean( allRayPaths, numberOfSectors, sectorAngle, 
                          fieldStrength[i] );
        else
          absSum( allRayPaths, numberOfSectors, sectorAngle, fieldStrength[i] );
          //sumAbs( allRayPaths, numberOfSectors, sectorAngle, gain[i],
           //       fieldStrength[i] );
        allRayPaths.clearAndClearSubList();
      }
 
      // determine which server is the best server at r(m,n)

      for ( i = 0; i < numberOfServers; i++ )
      {
        tempTotal = 0.0;
        for ( k = 0; k < numberOfSectors; k++ )
          tempTotal += maximumPower[i] * cabs( gain[i][k] )
                     * qsqr( cabs( fieldStrength[i][k] / TwoK ) ); 
        totalField[p][i] = tempTotal;
      }

      sort( totalField[p], bestServerMatrix[p], numberOfServers );

      bestServer = bestServerMatrix[p][0];

      currentBestServer[p] = 0;

      for ( i = 1; i < numberOfServers; i++ )
      {
        if ( cabs(totalField[p][bestServer]) < cabs(totalField[p][i]) )
          bestServer = i;
      }

      newSIR = SIR( totalField[p], numberOfServers, bestServer, sensitivity,
                    totalInterference, pathLoss ); 

      if ( newSIR >= targetSIR[p] )
      {
        interferingField = 0.0;
        for ( i = 0; i < numberOfServers; i++ )
        {
          if ( i == bestServer )
          {
            for ( k = 0; k < numberOfSectors; k++ )
              A.set( //fabs( gain[i][k] ) *
                     -qsqr( cabs( fieldStrength[i][k] / TwoK ) ), 
                       l, i*numberOfSectors+k );
          }
          else
          {
            for ( k = 0; k < numberOfSectors; k++ )
              A.set( targetSIR[p] //* fabs( gain[i][k] ) 
                     * qsqr( cabs( fieldStrength[i][k] / TwoK ) ), 
                     l, i*numberOfSectors+k );
          }
        }
        b.set( -btsObject.receiverSensitivityUE() * targetSIR[p], l );
        if ( writeBestServer == true )
          pathLossGrid.setPathLoss( bestServer+1, m, n );
        l++;
      }
      else
      {
        cout << "Target not met at index: " << p << endl;
      }
    
      totalCapacity[bestServer] += userCapacity;
    
      p++;
    }
  }

  // see if we exceed the capacity requirement for current server allocation
  isCapacityExceeded = false;
  for ( i = 0; i < numberOfServers; i++ )
  {
    if ( totalCapacity[i] > btsObject.serverCapacity() )
    {
      cout << " capacity exceeded";
      isCapacityExceeded = true;
      /*offloaded = offloadServer( bestServerMatrix, capacityGrid, 
                                 sensitivity, totalCapacity, 
                                 currentBestServer, totalField, targetSIR,
                                 btsObject.serverCapacity(),
                                 numberOfServers, numberOfCapPoints, i );
      if ( offloaded == true )
      {
        cout << "(offloaded)";
        isCapacityExceeded = false;
      }*/
    }
  }
  cout << endl;

  cout << "Convex matrix gives: " << l << " / " << p 
       << " met target SIR" << endl;

  coutput.close();

  if ( writeBestServer == true )
    matlabObject.pathLoss( pathLossGrid, "bestserver3.txt" );
  cout << "printed bestserver " << endl;

  // clean up memory allocation
  storageObject.DeleteComplexMatrix( fieldStrength, numberOfServers );
  storageObject.DeleteComplexMatrix( totalField, numberOfCapPoints );
  storageObject.DeleteIntegerMatrix( bestServerMatrix, numberOfCapPoints );
  storageObject.DeleteDoubleVector( totalCapacity );
  storageObject.DeleteIntegerVector( currentBestServer );
} 

void CDownlink::absSum(  List<CRayPath>& allRayPaths,
                             int             numberOfSectors,
                             double          sectorAngle,
                             complex*        Rsectors )
{
  double   AOD;
  int      k, l;
  complex  fieldStrength;
  CRayPath *rayPath;

  for ( k = 0; k < numberOfSectors; k++ )
    Rsectors[k] = 0.0;

  for ( l = 0; l < allRayPaths.size(); l++ )
  {
    rayPath        = &allRayPaths.GetItem(l);
    fieldStrength  = rayPath->fieldStrength_[2].z;

    AOD = rayPath->angleOfDeparture();

    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( AOD <= ( k + 1 ) * sectorAngle )
      {
        Rsectors[k] += fieldStrength;
        k = numberOfSectors;
      }
    }
  }
}

void CDownlink::absSum(  List<CRayPath>& allRayPaths,
                             int             numberOfSectors,
                             double          sectorAngle,
                             complex*        gain,
                             complex*        Rsectors )
{
  double   AOD;
  int      k, l;
  complex  fieldStrength;
  CRayPath *rayPath;

  for ( k = 0; k < numberOfSectors; k++ )
    Rsectors[k] = 0.0;

  for ( l = 0; l < allRayPaths.size(); l++ )
  {
    rayPath        = &allRayPaths.GetItem(l);
    fieldStrength  = rayPath->fieldStrength_[2].z;

    AOD = rayPath->angleOfDeparture();

    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( AOD <= ( k + 1 ) * sectorAngle )
      {
        Rsectors[k] += gain[k] * fieldStrength;
        k = numberOfSectors;
      }
    }
  }
}

void CDownlink::meanAbsSum( List<CRayPath>& allRayPaths,
                                int             numberOfSectors,
                                double          sectorAngle,
                                complex*        Rsectors )
{
  double   AOD;
  int      i, k, l;
  complex  fieldStrength;
  complex  expjtest;
  complex  *temp;
  CRayPath *rayPath;
  int      numberOfIterations = 100;

  for ( k = 0; k < numberOfSectors; k++ )
    Rsectors[k] = 0.0;

  temp = new complex [numberOfSectors];

  for ( i = 0; i < numberOfIterations; i++ )
  {
    for ( k = 0; k < numberOfSectors; k++ )
      temp[k] = 0.0;
    for ( l = 0; l < allRayPaths.size(); l++ )
    {
      rayPath        = &allRayPaths.GetItem(l);
      fieldStrength  = rayPath->fieldStrength_[2].z;

      AOD = rayPath->angleOfDeparture();
  
      for ( k = 0; k < numberOfSectors; k++ )
      {
        if ( AOD <= ( k + 1 ) * sectorAngle )
        {
          expjtest = CMaths::expj( TwoPi * ( double(rand()) / double(RAND_MAX) ) );
          temp[k] += fieldStrength * expjtest;
          k = numberOfSectors;
        }
      }
    }
    for ( k = 0; k < numberOfSectors; k++ )
      Rsectors[k] += cabs( temp[k] );
  }

  for ( k = 0; k < numberOfSectors; k++ )
  {
    Rsectors[k] = Rsectors[k] / numberOfIterations;
  }
  delete [] temp;
}

void CDownlink::meanAbsSum( List<CRayPath>& allRayPaths,
                                int             numberOfSectors,
                                double          sectorAngle,
                                complex*        gain,
                                complex*        Rsectors )
{
  double   AOD;
  int      i, k, l;
  complex  fieldStrength;
  complex  expjtest;
  complex  *temp;
  CRayPath *rayPath;
  int      numberOfIterations = 100;

  temp = new complex [numberOfSectors];

  for ( k = 0; k < numberOfSectors; k++ )
    Rsectors[k] = 0.0;

  for ( i = 0; i < numberOfIterations; i++ )
  {
    for ( k = 0; k < numberOfSectors; k++ )
      temp[k] = 0.0;
    for ( l = 0; l < allRayPaths.size(); l++ )
    {
      rayPath        = &allRayPaths.GetItem(l);
      fieldStrength  = rayPath->fieldStrength_[2].z;

  
      AOD = rayPath->angleOfDeparture();
  
      for ( k = 0; k < numberOfSectors; k++ )
      {
        if ( AOD <= ( k + 1 ) * sectorAngle )
        {
          expjtest = CMaths::expj( TwoPi * ( double(rand()) / double(RAND_MAX) ) );
          temp[k] += gain[k] * fieldStrength * expjtest;
          k = numberOfSectors;
        }
      }
    }
    for ( k = 0; k < numberOfSectors; k++ )
      Rsectors[k] += cabs( temp[k] );
  }

  for ( k = 0; k < numberOfSectors; k++ )
  {
    Rsectors[k] = Rsectors[k] / numberOfIterations;
  }
  delete [] temp;
}

void CDownlink::sumAbs( List<CRayPath>& allRayPaths,
                            int             numberOfSectors,
                            double          sectorAngle,
                            complex*        Rsectors )
{
  double   AOD;
  int      k, l;
  complex  fieldStrength;
  CRayPath *rayPath;

  for ( k = 0; k < numberOfSectors; k++ )
    Rsectors[k] = 0.0;

  for ( l = 0; l < allRayPaths.size(); l++ )
  {
    rayPath        = &allRayPaths.GetItem(l);
    fieldStrength  = rayPath->fieldStrength_[2].z;

    AOD = rayPath->angleOfDeparture();

    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( AOD <= ( k + 1 ) * sectorAngle )
      {
        Rsectors[k] += cabs(fieldStrength);
        k = numberOfSectors;
      }
    }
  }
}

void CDownlink::sumAbs( List<CRayPath>& allRayPaths,
                            int             numberOfSectors,
                            double          sectorAngle,
                            complex*        gain,
                            complex*        Rsectors )
{
  double   AOD;
  int      k, l;
  complex  fieldStrength;
  CRayPath *rayPath;

  for ( k = 0; k < numberOfSectors; k++ )
    Rsectors[k] = 0.0;

  for ( l = 0; l < allRayPaths.size(); l++ )
  {
    rayPath        = &allRayPaths.GetItem(l);
    fieldStrength  = rayPath->fieldStrength_[2].z;

    AOD = rayPath->angleOfDeparture();

    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( AOD <= ( k + 1 ) * sectorAngle )
      {
        Rsectors[k] += cabs(gain[k] * fieldStrength);
        k = numberOfSectors;
      }
    }
  }
}

double CDownlink::gaussianQuadrature4( double *E, int    N, double r, 
                                          double a,  double stepsize )
{
  double t[4], w[4];
  double zeta, zeta1, Wn, Jproduct, integ, a1, b1, c, m;
  int    k, n, i;

  //gaussian weights
  t[0] = -0.3399810436;
  t[1] = -t[0];
  t[2] = -0.8611363116;
  t[3] = -t[2];
  w[0] = 0.65214515;
  w[1] = w[0];
  w[2] = 0.34785485;
  w[3] = w[2];

  zeta = a;
  Wn   = 0.0;

  // integrate over zeta to obtain the PDF
  for ( i = 1; i < 100000; i++ )
  {
    a1 = zeta;
    b1 = zeta+stepsize;

    c  = ( b1 + a1 ) / 2.0;
    m  = ( b1 - a1 ) / 2.0;

    // calculate the integral at the 4 weighted positions
    for ( n = 0; n < 4; n++ )
    {
      Jproduct = 1.0;

      zeta1 = c + m * t[n];

      // calculate the product of Jo(zeta * E )
      for ( k = 0; k < N; k++ )
        Jproduct = Jproduct * j0( zeta1 * E[k] );
      
      integ = zeta1 * j0( zeta1 * r ) * Jproduct * m;
      Wn = Wn + w[n] * integ;
    }

    // if the integral has converged for increasing step then break
    if ( fabs(integ)/Wn < 1.0e-8 )
      break;

    zeta = zeta + stepsize;
  }
  if ( i == 100000 ) 
    cout << "there are too many gaussian quadrature integrations" << endl;

  return Wn;
}

double CDownlink::trapezoidal( double *E, int    N, double r, 
                                   double a,  double stepsize )
{
  double zeta, startStep, integ1, integ2, a1, b1, m1;
  double stepsize2, totalIntegral;
  int    m, M, i;

  totalIntegral = 0.0;
  a1            = a;
  integ1        = 0.0;

  // integrate over zeta to obtain the PDF
  for ( i = 1; i < 100000; i++ )
  {
    b1        = a1 + stepsize;
    stepsize2 = stepsize / 2.0;
    m1        = ( b1 + a1 ) / 2.0;

    M = 1;
    // set up initial trapezoidal integration rule
    integ2  = WnFunction( E, N, r, a1, stepsize2 / 2.0 );
    integ2 += WnFunction( E, N, r, m1, stepsize2 );
    integ2 += WnFunction( E, N, r, b1, stepsize2 / 2.0 );

    // calculate the integral by using an adaptive version of rule
    while ( fabs( ( integ1 - integ2 ) / integ2 ) > 1.0e-3 )
    {
      integ1    = integ2;
      M         = M * 2;
      stepsize2 = stepsize2 / 2.0;
      startStep = a1 - stepsize2;
      integ2    = integ1 / 2.0;
      for ( m = 1; m <= M; m++ )
      {
        zeta = startStep + m * stepsize2 * 2.0;
        integ2 = integ2 + WnFunction( E, N, r, zeta, stepsize2 );
      }
    }
    totalIntegral = totalIntegral + integ2;

    if ( fabs( integ2 / totalIntegral ) < 1.0e-3 )
      break;

    a1 = b1;
  } 

  return totalIntegral;
}

double CDownlink::WnFunction( double *E, int    N, double r, 
                                  double zeta, double stepsize )
{
  double Jproduct;
  int    k;

  Jproduct = 1.0;

  // calculate the product of Jo(zeta * E )
  for ( k = 0; k < N; k++ )
  {
    Jproduct = Jproduct * j0( zeta * E[k] );
  }

  return r * r * zeta * j0( zeta * r ) * Jproduct * stepsize;
}

double CDownlink::besselJ0( double x )
{
  if ( x > 3.0 )
    return SqrtTwoOverPi / sqrt(x) * cos( x - PiOver4 );
  else
    return j0( x );
}

boolean   CDownlink::offloadServer( int     **bestServerMatrix, 
                                        const   CCapacity& capacityGrid,
                                                double     sensitivity,
                                                double     *totalCapacity,
                                                int        *currentBestServer,
                                                complex    **totalField,
                                                double     *targetSIR,
                                                double     serverCapacity,
                                                int        numberOfServers,
                                                int        numberOfCapPoints,
                                                int        bestServer )
{
  int    i, serverNumber, newServerToTakeHandover;
  double newSIR, totalInterference, pathLoss;
   
  for ( i = 0; i < numberOfCapPoints; i++ )
  {
    serverNumber = currentBestServer[i];

    if ( bestServerMatrix[i][serverNumber] == bestServer )
    {
      if ( serverNumber - 1 < numberOfServers )
      {
        serverNumber++;
        newServerToTakeHandover = bestServerMatrix[i][serverNumber];
      //cout << "new server[" << i << "]: " << newServerToTakeHandover << endl;
        newSIR = SIR( totalField[i], numberOfServers, serverNumber, 
                      sensitivity, totalInterference, pathLoss );

        if ( newSIR > targetSIR[i] 
          && totalCapacity[newServerToTakeHandover] + capacityGrid[i] <
             serverCapacity )
        {
          totalCapacity[bestServer]              -= capacityGrid[i];
          totalCapacity[newServerToTakeHandover] += capacityGrid[i];
          currentBestServer[i]                    = serverNumber;
        }
      }
      if ( totalCapacity[bestServer] < serverCapacity )
      {
        return true;
      }
    }
  }

  return false;
}

double CDownlink::SIR( complex *totalField, int    numberOfServers,
                           int     bestServer,  double sensitivity,
                           double& totalInterference, double& pathLoss )
{
  int    i;

  totalInterference = 0.0;
  for ( i = 0; i < numberOfServers; i++ )
  {
    if ( i != bestServer )
      totalInterference += cabs(totalField[i]);
  }
  pathLoss = cabs( totalField[bestServer] );
  totalInterference += sensitivity;
  return pathLoss / totalInterference;
}

void CDownlink::setTargetSIR( const   CCapacity&        capacityGrid,
                                  const   SystemParameters& parameterObject,
                                  double* targetSIR ) 
{
  int              m, n, p;
  double           userCapacity, EbNo, fadeMargin, processingGain;

  p = 0;
  for ( n = 0; n < capacityGrid.yNumberOfPoints(); n++ )
  {
    for ( m = 0; m < capacityGrid.xNumberOfPoints(); m++ )
    {
      userCapacity = capacityGrid[p];
      parameterObject.inputs( userCapacity, EbNo, fadeMargin, processingGain );
      targetSIR[p] = EbNo / processingGain;
      p++;
    }
  }
}

void   CDownlink::randomWalkMean( List<CRayPath>& allRayPaths, 
                                      int             numberOfSectors,
                                      double          sectorAngle, 
                                      complex         *Rsectors )
{
  int            N, i, j, k, size2, *Ntotal;
  double         **E;
  double         stepsize1, stepsize2;
  double         randomWalkMean, integ2, r, Wn, AOD, *sumE;
  complex        fieldStrength, **Ec, *Rsector2;
  double         *sumA, *sumB, *sqrSum;
  CRayPath       *rayPath;
  CMemoryStorage storageObject;

  N  = allRayPaths.size();

  // nothing to do if no ray-paths
  if ( N == 0 )
  {
    for ( k = 0; k < numberOfSectors; k++ )
      Rsectors[k] == 0.0;
    return;
  }

  Ec = storageObject.CreateComplexMatrix( numberOfSectors, N );
  E  = storageObject.CreateDoubleMatrix(  numberOfSectors, N );
  sumE = storageObject.CreateDoubleVector( numberOfSectors );
  sumA = storageObject.CreateDoubleVector(  numberOfSectors );
  sumB = storageObject.CreateDoubleVector(  numberOfSectors );
  sqrSum  = storageObject.CreateDoubleVector(  numberOfSectors );
  Rsector2  = storageObject.CreateComplexVector(  numberOfSectors );
  Ntotal    = storageObject.CreateIntegerVector(  numberOfSectors );

  // convert data into real valued data in each sector of the server
  for ( i = 0; i < N; i++ )
  {
    rayPath       = &allRayPaths.GetItem(i);
    fieldStrength = rayPath->fieldStrength_[2].z;
  
    AOD = rayPath->angleOfDeparture();
  
    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( AOD <= ( k + 1 ) * sectorAngle )
      {
        j          = Ntotal[k];
        sumA[k]   += fieldStrength.real();
        sumB[k]   += fieldStrength.imag();
        sqrSum[k] += qsqr( fieldStrength.real() ); // variance with zero mean

        E[k][j] = cabs( fieldStrength );
        sumE[k] = sumE[k] + E[k][j];
        Ntotal[k]++;

        k          = numberOfSectors;
      }
    }
  }

  // if rician distributed (LOS large) can not use integral formula
  // so use same mean instead and return
  for ( k = 0; k < numberOfSectors; k++ )
  {
    if ( E[k][0] / sumE[k] > 0.95 && sumE[k] != 0.0 )
    {
      meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, Rsectors );
      return;
    }
  }

  // if rayleigh distributed then perform integral formula for random walk
  // mean value, do this for each sectorized antenna in the current server
  for ( k = 0; k < numberOfSectors; k++ )
  {
    // check for gaussian distributed
    if ( sumA[k] / sumB[k] > 0.98 && sumA[k] / sumB[k] < 1.020408 
      && N > 20 )
    {
      cout << "used gaussian random variable" << endl;
      Rsectors[k] = sqrt( sqrSum[k] * PiOver2 / double(N) );
      cout << "mean gauss value: R: " << Rsectors[k] << endl;
      meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, Rsector2 );
      cout << "mean numer value: R: " << Rsector2[k] << endl;
      continue;
    }

    size2     = 1000;
    stepsize1 = 20.0;
    stepsize2 = 0.05;
    
    randomWalkMean = 0.0;
    integ2         = 1.0;
    r              = stepsize2;
    
    // integrate over r to obtain the CDF
    for ( i = 1; i < size2; i++ )
    {
      Wn = trapezoidal( E[k], Ntotal[k], r, 0, stepsize1 );
    
      integ2 = Wn * stepsize2;
      randomWalkMean    = randomWalkMean + integ2;
    
      r = r + stepsize2;
    
      if ( fabs(integ2/randomWalkMean) < 1.0e-3 ) 
        break;
    }
    if ( i == 1000 ) 
      cout << "there are too many ungained mean integrations" << endl;
  
    if ( randomWalkMean < 0.0 )
    {
      //ofstream coutput( "Pray.txt" );
  
      //if ( size2 == 1000 ) 
        //cout << " gain there are too many euler integrations" << endl;
      //for ( i = 0; i < N; i++ )
        //coutput << Ec[i] << endl;
  
      //coutput.close();
      //cout << "negative value in integral, first Value %: " 
           //<< E[k][0] / sumE[k] << endl;
      meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, Rsectors );
      return;
    }
    Rsectors[k] = randomWalkMean;
  }

  storageObject.DeleteDoubleMatrix( E, numberOfSectors );
  storageObject.DeleteComplexMatrix( Ec, numberOfSectors );
  storageObject.DeleteComplexVector( Rsector2 );
  storageObject.DeleteDoubleVector( sumE );
  storageObject.DeleteDoubleVector( sumA );
  storageObject.DeleteDoubleVector( sumB );
  storageObject.DeleteDoubleVector( sqrSum );
  storageObject.DeleteIntegerVector( Ntotal );

}

void   CDownlink::randomWalkMean( List<CRayPath>& allRayPaths, 
                                      int             numberOfSectors,
                                      double          sectorAngle,
                                      complex*        gain,
                                      complex         *Rsectors )
{
  int            N, i, j, k, size2, *Ntotal;
  double         **E, stepsize1, stepsize2, randomWalkMean, integ2;
  double         r, Wn, AOD, *sumE;
  complex        fieldStrength, temp, **Ec, *Rsector2;
  double         *sumA, *sumB, *sqrSum;
  CRayPath       *rayPath;
  CMemoryStorage storageObject;

  N  = allRayPaths.size();

  // nothing to do if no ray-paths
  if ( N == 0 )
  {
    for ( k = 0; k < numberOfSectors; k++ )
      Rsectors[k] == 0.0;
    return;
  }

  Ec = storageObject.CreateComplexMatrix( numberOfSectors, N );
  E  = storageObject.CreateDoubleMatrix(  numberOfSectors, N );
  sumE = storageObject.CreateDoubleVector(  numberOfSectors );
  sumA = storageObject.CreateDoubleVector(  numberOfSectors );
  sumB = storageObject.CreateDoubleVector(  numberOfSectors );
  sqrSum  = storageObject.CreateDoubleVector(  numberOfSectors );
  Rsector2  = storageObject.CreateComplexVector(  numberOfSectors );
  Ntotal    = storageObject.CreateIntegerVector(  numberOfSectors );

  for ( i = 0; i < N; i++ )
  {
    rayPath       = &allRayPaths.GetItem(i);
    fieldStrength = rayPath->fieldStrength_[2].z;
  
    AOD = rayPath->angleOfDeparture();
  
    for ( k = 0; k < numberOfSectors; k++ )
    {
      if ( AOD <= ( k + 1 ) * sectorAngle )
      {
        temp       = gain[k] * fieldStrength;
        j          = Ntotal[k];
        sumA[k]   += temp.real();
        sumB[k]   += temp.imag();
        sqrSum[k] += qsqr( temp.real() ); // variance with zero mean

        E[k][j] = cabs( temp );
        sumE[k] = sumE[k] + E[k][j];
        Ntotal[k]++;

        k     = numberOfSectors;
      }
    }
  }

  for ( k = 0; k < numberOfSectors; k++ )
  {
    // check for Rician distributed
    if ( E[k][0] / sumE[k] > 0.95 && sumE[k] != 0.0 )
    {
      meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, gain, Rsectors );
      return;
    }
  }

  for ( k = 0; k < numberOfSectors; k++ )
  {
    if ( Ntotal[k] == 0 )
      continue;

    // check for gaussian distributed
    if ( sumA[k] / sumB[k] > 0.98 && sumA[k] / sumB[k] < 1.020408 
      && N > 20 )
    {
      cout << "used gaussian random variable" << endl;
      Rsectors[k] = sqrt( sqrSum[k] * PiOver2 / double(N) );
      cout << "mean gauss value: R: " << Rsectors[k] << endl;
      meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, gain, Rsector2 );
      cout << "mean numer value: R: " << Rsector2[k] << endl;
      continue;
    }
    size2     = 10000;
    stepsize1 = 20.0;
    stepsize2 = 0.05;
  
    randomWalkMean = 0.0;
    integ2         = 1.0;
    r              = stepsize2;
    
    // integrate over r to obtain the CDF
    for ( i = 1; i < size2; i++ )
    {
      Wn = trapezoidal( E[k], Ntotal[k], r, 0, stepsize1 );
    
      integ2 = Wn * stepsize2;
      randomWalkMean    = randomWalkMean + integ2;
    
      //fprintf( 1, 'r: %f, randomWalkMean: %15.12f\n', r, randomWalkMean );
      r = r + stepsize2;
    
      if ( fabs(integ2/randomWalkMean) < 1.0e-3 ) 
        break;
    }
  
  
    if ( randomWalkMean < 0.0 )
    {
      //cout << "negative value in integral, first Value %: " 
           //<< E[k][0] / sumE[k] << endl;
      meanAbsSum( allRayPaths, numberOfSectors, sectorAngle, gain, Rsectors );
      return;
    }

    if ( i == size2 ) 
      cout << " gain there are too many euler integrations" << endl;

    Rsectors[k] = randomWalkMean;
  }
      
  storageObject.DeleteDoubleMatrix( E, numberOfSectors );
  storageObject.DeleteComplexMatrix( Ec, numberOfSectors );
  storageObject.DeleteComplexVector( Rsector2 );
  storageObject.DeleteDoubleVector( sumE );
  storageObject.DeleteDoubleVector( sumA );
  storageObject.DeleteDoubleVector( sumB );
  storageObject.DeleteDoubleVector( sqrSum );
  storageObject.DeleteIntegerVector( Ntotal );
}

void   CDownlink::sort( complex* totalField,     int* reorderedIndices,
                        int     numberOfServers )
{
  int            i, j, k, l;
  CMemoryStorage storageObject;
  double         *dTotalField;

  dTotalField = storageObject.CreateDoubleVector( numberOfServers );

  for ( i = 0; i < numberOfServers; i++ )
  {
    reorderedIndices[i] = i;
    dTotalField[i]      = cabs( totalField[i] );
  }

  for ( i = 0; i < numberOfServers; i++ )
  {
    //for ( j = 0; j < numberOfServers; j++ )
    for ( j = 0; j < i; j++ )
    {
      k = reorderedIndices[i];
      l = reorderedIndices[j];
      if ( dTotalField[k] > dTotalField[l] )
      {
        reorderedIndices[i] = l;
        reorderedIndices[j] = k;
      }
    }
  }

  storageObject.DeleteDoubleVector( dTotalField );
}

