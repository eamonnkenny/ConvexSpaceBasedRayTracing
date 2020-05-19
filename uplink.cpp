//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    3rd March 2003
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include "uplink.hh"

int CUplink::generateRandomPopulation(
                             List<CPoint3d>&   btsPositions, 
                             CBaseTransceiver& btsObject,
                       const CBuilding&        buildingObject,
                       const SystemParameters& uplinkParameters )
{
  int            numberOfServers, makeSeed = 1, serversMeetingSIR;
  CRandomNumber  randomObject;
  double         randomNumber, newCapacity;
  int            i, numberOfUEs;
  double         *userCapacity, *targetSIR, *totalServerCapacity;
  int            *serverConnections;
  boolean        doesBestServerMeetSIR, addedConnection, successful;
  List<CUnitEquipment> equipmentList;
  CUnitEquipment unitEquipment;
  CPoint3d       p;
  ofstream       coutput( "randompoints.txt" );
  ofstream       coutput2( "bts.txt" );
  int            totalNumberOfConnections = 0;

  multiuserDetection_ = true;
  numberOfServers     = btsPositions.size();
  numberOfUEs         = numberOfServers * 8;
  // set new seed for random number generator
  randomNumber        = randomObject.randomVariable( makeSeed );
  userCapacity        = new double [numberOfUEs];
  targetSIR           = new double [numberOfUEs];
  serverConnections   = new int [numberOfServers];
  totalServerCapacity = new double [numberOfServers];

  for ( i = 0; i < numberOfServers; i++ )
  {
    serverConnections[i]   = 0;
    totalServerCapacity[i] = 0.0;
    coutput2 << btsPositions.GetItem(i) << endl;
  }
  coutput2.close();

  // calculate number of possible UE's = 8 * numberOfServers
  //cout << "Number Of UE's initially: " << numberOfUEs << endl;
  serversMeetingSIR = 0;
  for ( i = 0; i < numberOfUEs; i++ )
  {
    successful = false;
    p = generateNewPoint( buildingObject, newCapacity );
    userCapacity[i] = newCapacity;
    unitEquipment = pathLossToEachServer( p, btsObject, btsPositions, 
                                          buildingObject, numberOfServers );
    updateSIR( unitEquipment, equipmentList, btsObject, userCapacity[i], 
               numberOfUEs, numberOfServers );
    targetSIR[i] = setTargetSIR( i, userCapacity, uplinkParameters,
                                 unitEquipment.processingGain_ );
    doesBestServerMeetSIR = calculateBestServer( unitEquipment, targetSIR[i] );
    addedConnection = addConnection( equipmentList, unitEquipment, 
                                     serverConnections, totalServerCapacity,
                                     doesBestServerMeetSIR );
    if ( addedConnection == true )
    {
      successful = updateExistingConnections( equipmentList, serverConnections,
                             totalServerCapacity, btsObject, numberOfServers );
      if ( successful == true ) totalNumberOfConnections++;
    }
    /*else
      cout << "Did not add point to list with Capacity: " 
           << unitEquipment.capacity << endl;*/
  }
  coutput.close();

  //print( equipmentList );

  delete [] userCapacity;
  delete [] targetSIR;
  delete [] serverConnections;
  delete [] totalServerCapacity;

  return totalNumberOfConnections;
}

void CUplink::print( List<CUnitEquipment>& equipmentList )
{
  CUnitEquipment unitEquipment;

  for ( int i = 0; i < equipmentList.size(); i++ )
  {
     unitEquipment = equipmentList.GetItem(i);

     cout << "i: " << i << " BS: " << unitEquipment.bestServer() << " cap: "
          << unitEquipment.capacity << endl;
  }
}

CPoint3d CUplink::generateNewPoint( const CBuilding& buildingObject, 
                                    double& capacity)
{
  ifstream      cinput;
  CRandomNumber randomObject;
  CPoint3d      startPoint, endPoint;
  double        randomNum, x1, y1, z1;
  boolean       isInFreeConvex;
  int           seedNotNeeded = 0, convexNumber;

  startPoint = buildingObject.boundingBox_.southWest();
  endPoint   = buildingObject.boundingBox_.northEast();

  cinput.open( "dataprob.dat" );
  readDataProbabilities( cinput );

  do 
  {
    randomNum  = randomObject.randomVariable( seedNotNeeded );
    x1         = startPoint.x * randomNum + endPoint.x * ( 1.0 - randomNum );
    randomNum  = randomObject.randomVariable( seedNotNeeded );
    y1         = startPoint.y * randomNum + endPoint.y * ( 1.0 - randomNum );
    randomNum  = randomObject.randomVariable( seedNotNeeded );
    z1         = startPoint.z * randomNum + endPoint.z * ( 1.0 - randomNum );
    isInFreeConvex = buildingObject.locateConvexNumber( CPoint3d( x1, y1, z1 ),
                     convexNumber );
    //if ( isInFreeConvex == false )
      //cout << "Point was found to be inside a wall: " << CPoint3d( x1, y1, z1 );
    randomNum  = randomObject.randomVariable( seedNotNeeded );
    capacity   = generateCapacity( randomNum );
  }
  while ( isInFreeConvex == false );

  z1 = buildingObject.convexArray_[convexNumber].facet[0].vertex_[0].z;
  if ( capacity < 0.128 )
    return CPoint3d( x1, y1, z1+1.5 );
  else
    return CPoint3d( x1, y1, z1+1.0 );
}

void CUplink::readDataProbabilities( ifstream& cinput )
{
  cinput >> voiceProbability_;
  cinput >> nonRealTimeDataProbability_;
  cinput >> realTimeDataProbability_;
}

double CUplink::generateCapacity( double randomNum )
{
  if ( randomNum < voiceProbability_ )
    return 0.0122;
  else if ( randomNum < nonRealTimeDataProbability_ )
    return 0.128;
  else 
    return 0.256;
}

CUnitEquipment CUplink::pathLossToEachServer( 
                                   CPoint3d&         unitEquipmentPosition,
                                   CBaseTransceiver& btsObject,
                                   List<CPoint3d>&   baseStationLocations, 
                             const CBuilding&        buildingObject,
                             const int&              numberOfServers )
{
  complex        *rayPathLoss;
  double         *unitEquipmentPower;
  double         *doublePathLoss;
  CUnitEquipment unitEquipment;
  int            i, btsIndex = -1;
  List<CRayPath> allRayPaths;
  CRayTracing    rayTracingObject;
  CDownlink      downlinkObject;

  rayPathLoss        = new complex [numberOfServers];
  unitEquipmentPower = new double [numberOfServers];
  doublePathLoss     = new double [numberOfServers];

  for ( i = 0; i < numberOfServers; i++ )
  {
    btsObject.setPosition( baseStationLocations.GetItem(i) );
    rayTracingObject.MimoRayTracing( allRayPaths, buildingObject, 
                             btsObject, unitEquipmentPosition, LOSSLESS );
    downlinkObject.meanAbsSum( allRayPaths, 1, 360.0, &rayPathLoss[i] );
    doublePathLoss[i]     = fabs( rayPathLoss[i] );
    unitEquipmentPower[i] = doublePathLoss[i] 
                          * btsObject.receiverSensitivityBTS();
    allRayPaths.clearAndClearSubList();
  }

  unitEquipment = CUnitEquipment( unitEquipmentPosition, doublePathLoss,
                                  unitEquipmentPower, numberOfServers );

  delete [] rayPathLoss;
  delete [] unitEquipmentPower;
  delete [] doublePathLoss;

  return unitEquipment;
}

void  CUplink::updateSIR(          CUnitEquipment&       unitEquipment, 
                                   List<CUnitEquipment>& equipmentList,
                          const    CBaseTransceiver&     btsObject,
                                   double                capacity, 
                                   int                   numberOfUEs,
                                   int                   numberOfServers )
{
  double         *SIR, interferers;
  int            i, j;
  CUnitEquipment currentItem;

  SIR = new double [numberOfServers];
  for ( j = 0; j < numberOfServers; j++ )
  {
    interferers = btsObject.receiverSensitivityBTS();
    for ( i = 0; i < equipmentList.size(); i++ )
    {
      currentItem = equipmentList.GetItem(i);
      if ( multiuserDetection_ == true )
        if ( currentItem.bestServer != unitEquipment.bestServer )
          interferers = interferers 
                      + currentItem.power[j] * currentItem.rayPathLoss[j];
      else
        interferers = interferers 
                    + currentItem.power[j] * currentItem.rayPathLoss[j];
    }
    SIR[j] = unitEquipment.power[j] * unitEquipment.rayPathLoss[j]/interferers; 
  }

  unitEquipment.setSIR( SIR );
  unitEquipment.setCapacity( capacity );
}

double CUplink::setTargetSIR( int index, double* capacity, 
                              const   SystemParameters& uplinkParam,
			      double& processingGain )
{
  double EbNo, fadeMargin, targetSIR;

  uplinkParam.inputs( capacity[index], EbNo, fadeMargin, processingGain );
  targetSIR = EbNo;
  if ( processingGain < 2 ) processingGain = 16 * 1000;
  processingGain = 16;

  return targetSIR;
}

boolean CUplink::calculateBestServer( CUnitEquipment& unitEquipment,
                                      double          targetSIR )
{
  int i;
  int *bestServerIndex;
  int bestServer;

  bestServerIndex = new int [unitEquipment.numberOfServers];
  for ( i = 0; i < unitEquipment.numberOfServers; i++ )
    bestServerIndex[i] = i;

  sort( unitEquipment.SIR, bestServerIndex, unitEquipment.numberOfServers );

  unitEquipment.setBestServer( bestServerIndex );
  unitEquipment.setTargetSIR(  targetSIR );
  bestServer = bestServerIndex[0];
  // set the actual best server as the first in the list 
  unitEquipment.setBestServer( bestServer );

  delete [] bestServerIndex;

  if ( unitEquipment.SIR[bestServer] * unitEquipment.processingGain() 
       > targetSIR )
    return true;
  else
  {
    //cout << "Somehow didn't meet SIR with processing gain" << endl;
    //cout << unitEquipment.SIR[0] << " " << unitEquipment.SIR[1] << "  "
         //<< unitEquipment.SIR[2] << " " << unitEquipment.targetSIR << endl;
    return false;
  }
}

boolean CUplink::addConnection( List<CUnitEquipment>& equipmentList,
                                CUnitEquipment&       unitEquipment,
                                int*                  serverConnections,
                                double*               totalServerCapacity,
                                boolean               doesBestServerMeetSIR )
{
  // must be able to meet SIR
  if ( doesBestServerMeetSIR == true )
  {
    if ( serverConnections[unitEquipment.bestServer()] < 8 )
    {
      if ( checkCapacity( unitEquipment, totalServerCapacity ) == true )
      {
        // if SIR met and capacity met then add new connection to best server
        // updating UE list, total number of connections to each server
        // and total capacity in each server
        equipmentList.append( unitEquipment );
        serverConnections[unitEquipment.bestServer()]++;
        totalServerCapacity[unitEquipment.bestServer()]
                                               += unitEquipment.capacity;
        return true;
      }
      else
      {
        //cout << "didn't meet capacity requirements" << endl;
      }
    }
    else if ( serverConnections[unitEquipment.secondBestServer()] < 8 )
    {
      unitEquipment.setBestServer( unitEquipment.secondBestServer() );
      // make 2nd best server the new best server and add connection

      if ( checkCapacity( unitEquipment, totalServerCapacity ) == true 
        && unitEquipment.SIR[unitEquipment.bestServer()] *
	unitEquipment.processingGain() > unitEquipment.targetSIR )
      {
        // if SIR met and capacity met then add new connection to best server
        equipmentList.append( unitEquipment );
        serverConnections[unitEquipment.bestServer()]++;
        totalServerCapacity[unitEquipment.bestServer()] 
                                               += unitEquipment.capacity;
        return true;
      }
      else
      {
        //cout << "didn't meet capacity requirements" << endl;
      }
    }
  }
  return false;
}

boolean CUplink::updateExistingConnections( 
                            List<CUnitEquipment>& equipmentList,
                            int*                  serverConnections,
                            double*               totalServerCapacity,
                            const CBaseTransceiver& btsObject,
                            int   numberOfServers )
{
  int            i, j;
  double         interference, noise, signal, SIR;
  CUnitEquipment newUE, *unitEquipment;
  double         bestServerPower, bestServerPowerJ, pathLoss;

  newUE           = equipmentList.last();
  bestServerPower = newUE.power[newUE.bestServer()];
  noise           = btsObject.receiverSensitivityBTS();

  // first test to see that the new connection is viable, if not remove it
  for ( i = 0; i < equipmentList.size() - 1; i++ )
  {
    unitEquipment = &equipmentList.GetItem(i);
    bestServerPowerJ = unitEquipment->power[unitEquipment->bestServer()];

    //for ( j = 0; j < numberOfServers; j++ )
    j = unitEquipment->bestServer();
    {
      if ( multiuserDetection_ == true )
      {
        if ( j != newUE.bestServer() )
        {
          pathLoss      = unitEquipment->rayPathLoss[j];
          signal        = bestServerPowerJ * pathLoss;
          interference  = signal / unitEquipment->SIR[j] - noise; 
	  //cout << "Server( " << i << ") -> BS(" << j << "): ";
	  //cout << "SIR: " << unitEquipment->SIR[j] << endl;
          // update the interference to include newest connection
          interference += bestServerPower * newUE.rayPathLoss[j];
	  //cout << "Server( " << i << ") -> BS(" << j << "): ";
          SIR           = signal * unitEquipment->processingGain() 
	                / ( interference + noise );
	  //cout << "SIR: " << SIR << " " << unitEquipment->targetSIR << endl;

          if ( SIR < unitEquipment->targetSIR )
          {
            //cout << "-------------- Did not introduce new point " 
	         //<< "--------------" << endl;
            serverConnections[newUE.bestServer()]--;
            totalServerCapacity[newUE.bestServer()] -= newUE.capacity;
            equipmentList.Pop();  // pop the last point off the list
            return false;
          }
          //unitEquipment->SIR[j] = SIR; // update the SIR
        }
      }
      else
      {
        pathLoss      = unitEquipment->rayPathLoss[j];
        signal        = bestServerPowerJ * pathLoss;
        interference  = signal / unitEquipment->SIR[j] - noise; 
        // update the interference to include newest connection
        interference += bestServerPower * newUE.rayPathLoss[j];
        SIR           = signal * unitEquipment->processingGain()
	              / ( interference + noise );

        if ( SIR < unitEquipment->targetSIR )
        {
          cout << "**** Did not introduce new point ******" << endl;
          serverConnections[newUE.bestServer()]--;
          totalServerCapacity[newUE.bestServer()] -= newUE.capacity;
          equipmentList.Pop();  // pop the last point off the list
          return false;
        }
        //unitEquipment->SIR[j] = SIR; // update the SIR
      }
    }
  }

  // now that the new connection is viable, include it.
  for ( i = 0; i < equipmentList.size() - 1; i++ )
  {
    unitEquipment = &equipmentList.GetItem(i);
    bestServerPowerJ = unitEquipment->power[unitEquipment->bestServer()];

    //for ( j = 0; j < numberOfServers; j++ )
    j = unitEquipment->bestServer();
    {
      if ( multiuserDetection_ == true )
      {
        if ( j != newUE.bestServer() )
        {
          pathLoss      = unitEquipment->rayPathLoss[j];
          signal        = bestServerPowerJ * pathLoss;
          interference  = signal / unitEquipment->SIR[j] - noise; 
          // update the interference to include newest connection
          interference += bestServerPower * newUE.rayPathLoss[j];
          SIR           = signal * unitEquipment->processingGain()
	                / ( interference + noise );
          unitEquipment->SIR[j] = SIR; // update the SIR
        }
      }
      else
      {
        pathLoss      = unitEquipment->rayPathLoss[j];
        signal        = bestServerPowerJ * pathLoss;
        interference  = signal / unitEquipment->SIR[j] - noise; 
        // update the interference to include newest connection
        interference += bestServerPower * newUE.rayPathLoss[j];
        SIR           = signal * unitEquipment->processingGain()
	              / ( interference + noise );
        unitEquipment->SIR[j] = SIR; // update the SIR
      }
    }
  }
  //cout << "====================== new point ======================" << endl;
  return true;
}

boolean CUplink::checkCapacity( CUnitEquipment& unitEquipment,
                                double* totalServerCapacity )
{
  if ( totalServerCapacity[unitEquipment.bestServer()] 
     + unitEquipment.capacity < 2.0 )
  {
    // if SIR met and capacity met then add new connection to best server
    return true;
  }

  unitEquipment.setCapacity( unitEquipment.capacity / 2.0 );
  if ( totalServerCapacity[unitEquipment.bestServer()] 
       + unitEquipment.capacity < 2.0 )
  {
    return true;
  }

  return false;
}

void   CUplink::sort( double* rayPathLoss,     int* reorderedIndices,
                      int     numberOfServers )
{
  int            i, j, k, l;
  
  for ( i = 0; i < numberOfServers; i++ )
  {
    reorderedIndices[i] = i;
  }

  for ( i = 0; i < numberOfServers; i++ )
  {
    for ( j = 0; j < i; j++ )
    {
      k = reorderedIndices[i];
      l = reorderedIndices[j];
      if ( rayPathLoss[k] > rayPathLoss[l] )
      {
        reorderedIndices[i] = l;
        reorderedIndices[j] = k;
      }
    }
  }
}

