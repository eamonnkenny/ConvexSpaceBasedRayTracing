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

#include "reflection.hh"

void CReflection::CreateImagesOfTransmitter( 
                                  List<CImage>  *imageList,
                            const CBuilding&    buildingObject,
                            const CBaseTransceiver& btsObject,
                                  List<CPlane>& reflectionPlanes,
                            const CPoint3d&     transmitter,
                            const int&          transConvexIndex )
{
  int     i;
  CImage  firstImage;
  CMatlab matlabObject;

  // create root of the tree with no convex space boundary or previous image
  // information
  firstImage = CImage( transmitter, -1 );
  imageList[0].append( firstImage );

  // create all first order images of the first node
  //imageList[1] = CreateFirstOrderImagesOf( firstImage.point_, 
                                           //reflectionPlanes );
  if ( btsObject.numberOfReflections() > 0 )
  CreateFirstOrderImagesOf( firstImage.point_, 
                                           reflectionPlanes,imageList[1] );
  matlabObject.images( transmitter, imageList[1] );

  // create a new list of images from the previous list, remembering that
  // the first order images can give us the reflection boundaries of the
  // building
  for ( i = 2; i <= btsObject.numberOfReflections(); i++ )
  {
    //imageList[i] = CreateImagesOf( imageList[i-1], reflectionPlanes );
    CreateImagesOf( imageList[i-1], reflectionPlanes, imageList[i] );
  }

  // plot a list of images to a file images.txt

}

//List<CImage>& CReflection::CreateFirstOrderImagesOf(
                                // const CPoint3d&     firstNode,
                                // List<CPlane>& reflectionPlanes )
void CReflection::CreateFirstOrderImagesOf(
                                const CPoint3d&     firstNode,
                                      List<CPlane>& reflectionPlanes,
                                      List<CImage>& imageList )
{
  int      i;
  boolean  isValidReflection;
  CPlane   aPlane;
  CPoint3d TxImage;
  //List<CImage> *imageList;

  //imageList    = new List<CImage>;

  //for all possibly reflection oriented planes 
  for ( i = 0; i < reflectionPlanes.size(); i++ )
  {
    
    aPlane = reflectionPlanes.GetItem(i);
    isValidReflection = aPlane.validImage( firstNode, TxImage );

    // find a valid reflection point, and if it exists, add it to the list
    if ( isValidReflection == true )
      imageList.append( CImage( TxImage, 0 ) ); 
      //imageList->append( CImage( TxImage, 0 ) ); 
  }

  //return *imageList;
}

void CReflection::CreateImagesOf( List<CImage>& previousList,
                                  List<CPlane>& reflectionPlanes,
                                  List<CImage>& currentImageList )
//List<CImage>& CReflection::CreateImagesOf( List<CImage>&   previousList,
                                           //List<CPlane>& reflectionPlanes )
{
  ///List<CImage>   *currentImageList;
  int          k, m;
  CPoint3d     Tx, TxImage;
  boolean      isValid;
  CImage       firstOrderImage;

  // create new empty image list
  //currentImageList = new List<CImage>;

  //for all previous image calculate new images in convex

  for ( k = 0; k < previousList.size(); k++ )
  {
    Tx      = previousList.GetItem(k).point_;

    for ( m = 0; m < reflectionPlanes.size(); m++ )
    {
      isValid = reflectionPlanes.GetItem(m).validImage( Tx, TxImage );

      // we want to get the image of a previous image point now, and insert
      // it into the list of images.

      if ( isValid == true )
        //currentImageList->append( CImage( TxImage, k ) ); 
        currentImageList.append( CImage( TxImage, k ) ); 
    }
  }

  //return *currentImageList;
}

//List<CRayPath>& CReflection::CreateValidRayList( 
CReflection::CReflection( List<CRayPath>& rayPathList,
                          List<CImage>    *transmitterImages,
                    const CBuilding&      buildingObject,
                    const CBaseTransceiver& btsObject,
                    const int&            transmitterConvexIndex,
                    const CPoint3d&       transmitterPoint,
                    const CPoint3d&       receiverPoint )
{
  CPointInfo       currentPointInfo;
  boolean          isValid, isInBuilding;
  CPoint3d         reflectionPoint;
  CImage           currentImage, previousImage;
  int              i, j;
  int              receiverConvexNumber, transmissionCounter;
  List<CPointInfo> currentPathList;
  //List<CRayPath>   *rayPathList;
  CPoint3d         *pointImages;
  CMemoryStorage   StorageObject;
  CRayPath         rayPath;

  //rayPathList = new List<CRayPath>;

  // code added 16/11/01 to building.hh
  isInBuilding = buildingObject.locateConvexNumber( receiverPoint,
                                                    receiverConvexNumber );
  if ( isInBuilding == false )
  {
    cout << "Warning: No receiver in building: " << receiverPoint << endl;
    return;
  }

  // first get path back from receiver to transmitter
  currentPointInfo = CPointInfo( receiverPoint, receiverConvexNumber,
                                 0, RECEIVER );

  transmissionCounter = 0;
  currentPathList.push( currentPointInfo );

  isValid = countTransmissionsToTx(  currentPointInfo, transmitterPoint,
                                     buildingObject, btsObject, 
				     transmitterConvexIndex,
                                     currentPathList,  transmissionCounter ); 

  currentPointInfo = CPointInfo( transmitterPoint, transmitterConvexIndex,
                                 -1, TRANSMITTER );
  currentPathList.push( currentPointInfo );

  if ( transmissionCounter <= btsObject.numberOfTransmissions() && isValid )
  {
    rayPath = CRayPath(currentPathList, transmissionCounter, 0,0);
    rayPathList.append( rayPath );
  }
  currentPathList.clear();
  //cout << "Reflection Path: Tx -> 0R -> Rx" << endl;

  // traverse the tree of images for all orders
  for ( i = 1; i <= btsObject.numberOfReflections(); i++ )
  {
    //cout << "Reflection Path: Tx -> " << i << "R -> Rx" << endl;

    // at each level in the tree we must see if we get a valid ray-path
    // of that order+2
    for ( j = 0; j < transmitterImages[i].size(); j++ )
    {
      // We create a ray-path which will only be added to the valid ray path
      // list if it meets certain criteria below. 

      currentImage = transmitterImages[i].GetItem(j);
      pointImages  = StorageObject.CreatePoint3dVector( i+2 );

      makeArrayOfImages( pointImages, receiverPoint, currentImage,
                         transmitterImages, i );

      transmissionCounter = 0;
      // traverse the ray-path to see if all the reflections are valid
      isValid = getReflections( currentPathList, RECEIVER,
                                //receiverConvexNumber,
                                buildingObject, btsObject, pointImages, i,
                                TRANSMITTER, //transmitterConvexIndex, 
                                transmissionCounter );

      // only if the reflection points are valid and the number of
      // maximum transmissions has not been reached, then create the ray path.
      if ( transmissionCounter <= btsObject.numberOfTransmissions() && isValid == true )
      {
        rayPath = CRayPath(currentPathList, transmissionCounter, i,0);
        rayPathList.append( rayPath );
      }

      // delete all temporary lists.
      currentPathList.clear();
      StorageObject.DeletePoint3dVector( pointImages );
    }
  }
  //return *rayPathList;
}

void CReflection::makeArrayOfImages( 
                             CPoint3d     *pointImages,
                       const CPoint3d&    endPoint,
                       const CImage&      imagePoint,
                             List<CImage> *transmitterImages,
                       const int&         numberOfReflections )
{
  CImage previousImage, currentImage;
  int    k;

  // build the possible images for the ray-path + transmitter and receiver

  k = numberOfReflections;
  //cout << "Dx to Tx: " << k << endl;

  pointImages[k+1] = endPoint;
  //cout << setw(4) << k+1 << ": " << pointImages[k+1] << endl;

  currentImage = imagePoint;

  pointImages[k] = currentImage.point_;

  //cout << setw(4) << k << ": " << pointImages[k] << endl;

  // finish creating the possible ray-path
  for ( k = numberOfReflections-1; k >= 0; k-- )
  {
    previousImage  = transmitterImages[k].GetItem(
                                 currentImage.previousImageIndex_ );
    pointImages[k] = previousImage.point_;
    currentImage   = previousImage;
    //cout << setw(4) << k << ": " << pointImages[k] << endl;
  }
}
  
void CReflection::makeArrayOfImages2( 
                             CPoint3d     *pointImages,
                       const CPoint3d&    endPoint,
                       const CImage&      imagePoint,
                             List<CImage> *transmitterImages,
                       const int&         numberOfReflections )
{
  CImage previousImage, currentImage;
  int    k;

  // build the possible images for the ray-path + transmitter and receiver

  k = numberOfReflections;
  //cout << "Dx to Tx: " << k << endl;

  pointImages[0] = endPoint;
  //cout << setw(4) << k+1 << ": " << pointImages[k+1] << endl;

  currentImage = imagePoint;

  pointImages[1] = currentImage.point_;

  //cout << setw(4) << k << ": " << pointImages[k] << endl;

  // finish creating the possible ray-path
  for ( k = numberOfReflections-1; k >= 0; k-- )
  {
    previousImage  = transmitterImages[k].GetItem(
                                 currentImage.previousImageIndex_ );
    pointImages[numberOfReflections+1-k] = previousImage.point_;
    currentImage   = previousImage;
    //cout << setw(4) << k << ": " << pointImages[k] << endl;
  }
}
  
boolean CReflection::getReflections( List<CPointInfo>& currentPathList,
                               const contactType&      lastPointType,
                               //const int&              lastConvexIndex,
                               const CBuilding&        buildingObject,
                               const CBaseTransceiver& btsObject,
                                     CPoint3d          *pointImages,
                               const int&              numberOfImages,
                               const contactType&      generatingPointType,
                               //const int&              generatingConvexIndex,
                                     int&              transmissionCounter )
{
  boolean    isValid;
  CPointInfo currentPointInfo, temporaryPointInfo;
  CPoint3d   reflectionPoint;
  CPoint3d   perturbation;
  int        nextConvexNumber, nextBoundaryNumber;
  int        i, reflectionPointConvexNumber;
  int        lastConvexIndex, generatingConvexIndex;

  i = numberOfImages;

  // shift start point into a convex space to find its starting point
  perturbation = (CSegment3d(pointImages[i+1], pointImages[i])).unitVector()
               / 1000.0 + pointImages[i+1];
  isValid = buildingObject.locateConvexNumber( perturbation,
                                               lastConvexIndex );
  if ( isValid == false )
    return false;

  // we just found a ray that goes straight into a wall, not a free convex

  if ( lastPointType == DIFFRACTION || lastPointType == VIRTUAL_DIFFRACTION )
  {
    currentPointInfo = CPointInfo( pointImages[i+1], lastConvexIndex,
                                   -1, -1, -1, lastPointType );
  }
  else 
    currentPointInfo = CPointInfo( pointImages[i+1], lastConvexIndex,
                                   -1, lastPointType );

  if ( lastPointType != DIFFRACTION && lastPointType != VIRTUAL_DIFFRACTION )
  {
    currentPathList.push( currentPointInfo );
  }

  nextConvexNumber = lastConvexIndex;

  isValid = true;

  if ( i >= 1 )
  {
    do
    {
      // Note that for a valid ray (next routine returns true) the
      // reflection point is returned and the current convex number in 
      // which that point exists. 
      // This method traces back along a line until the boundary 
      // specified by the current image is met. If the boundary is not
      // a filled convex space, or too many transmissions occur then the 
      // ray is invalid.
  
      isValid = validReflectionPoint( currentPointInfo,   pointImages[i],
                                      pointImages[i-1],   reflectionPoint,
                                      reflectionPointConvexNumber,
                                      buildingObject, btsObject, 
				      nextConvexNumber, nextBoundaryNumber,
				      currentPathList, transmissionCounter ); 
  
      // Assuming a valid trace to a point exists, we need to update
      // the current ray-path information and travel back to the 
      // previous ordered image, and continue until we meet the
      // transmitter
      if ( isValid == true )
      {
        currentPointInfo = CPointInfo( reflectionPoint,
                                       reflectionPointConvexNumber,
                                       nextBoundaryNumber, REFLECTION );
        currentPathList.push( currentPointInfo );
  
        i--;
      }
      else
      {
        return false;
      }
    } while ( i > 0 );
  }

  // add the current ray-path to the valid ray path list
  if ( isValid == true )
  {
    // must do this to start in correct convex space
    temporaryPointInfo              = currentPointInfo;
    temporaryPointInfo.convexNumber = nextConvexNumber;
    
    if ( numberOfImages > 0 )
      perturbation = (CSegment3d(pointImages[0], currentPointInfo.node )
                     ).unitVector() / 1000.0 + pointImages[0];
    else
      perturbation = (CSegment3d(pointImages[0], pointImages[1])).unitVector()
                   / 1000.0 + pointImages[0];
    isValid = buildingObject.locateConvexNumber( perturbation,
                                                 generatingConvexIndex );

    if ( isValid )
      isValid = countTransmissionsToTx( temporaryPointInfo, pointImages[0],
                                        buildingObject, btsObject, 
					generatingConvexIndex,
                                        currentPathList, transmissionCounter ); 

    if ( generatingPointType != DIFFRACTION
      && generatingPointType != VIRTUAL_DIFFRACTION && isValid == true )
    {
      currentPointInfo = CPointInfo( pointImages[0], generatingConvexIndex,
                                     -1, generatingPointType );
      currentPathList.push( currentPointInfo );
    }
  }

  return isValid;
}

boolean CReflection::validReflectionPoint( const CPointInfo& currentPointInfo,
                                           const CPoint3d&   imagePoint,
                                           const CPoint3d&   previousImage,
                                                 CPoint3d&   reflectionPoint,
                                                 int&        reflectionConvex,
                                           const CBuilding&  buildingObject,
                                           const CBaseTransceiver& btsObject,
                                                 int&        lastConvexNumber,
                                                 int&        lastBoundaryNumber,
                                           List<CPointInfo>& currentPathList,
                                                 int&     transmissionCounter )
{
  boolean   doesJoin, isReflectionPoint;
  int       boundaryNumber;
  CPoint3d  pointOnBoundary, transmissionPoint;
  CBoundary *currentBoundary;
  CConvex   *currentConvexSpace;
  CPlane    equationOfPlane;
  int       isignTx, isignRx;
  int       i, j = -1;

  transmissionPoint = currentPointInfo.node;

  //cout << "current Image Point: " << imagePoint.point_ << endl;

  // if Rx and Tx on the one side of the line then no reflection point exists
  // Amended for neatness: 27/11/01
  equationOfPlane = CPlane( previousImage, imagePoint );
  isignTx = equationOfPlane.isign( imagePoint );
  isignRx = equationOfPlane.isign( transmissionPoint );
  
  // so we must return no valid reflection point
  if ( isignRx == isignTx )
  {
    //cout << "IsignRx == iSignTx: return false" << endl;
    return false;
  }

  //lastConvexNumber   = currentPointInfo.convexNumber;

  // requires intersection of lines with boundaries of convex spaces
  // intersection() routines in boundary.hh and convex.hh added 16/11/01
  // travelling from known point until the reflection point is found.
  do
  {
    doesJoin          = false;
    //doesIntersect     = false;
    isReflectionPoint = false;

    currentConvexSpace = &buildingObject.convexArray_[lastConvexNumber];
    // if one point is in a room there is only one possible point of
    // intersection with the boundaries of a convex space.
    // if one of the points is on a particular boundary we don't want to
    // include that point as a possible intersection. Instead choose the
    // other one.
    //doesIntersect = currentConvexSpace->notIntersection( 
    currentConvexSpace->notIntersection( 
                                   transmissionPoint, imagePoint, 
                                   pointOnBoundary, boundaryNumber );

    //cout << "Checking convex: " << lastConvexNumber << " on boundary: " 
         //<< boundaryNumber << " point is: " << pointOnBoundary << endl;
    if ( boundaryNumber == 6 )
    {
      //cout << "very special case of reflection point in interior corner" <<
      //endl;
      // only occurs when transmissionPoint was equal to pointOnBoundary
      return false;
    }

    currentBoundary = &currentConvexSpace->facet[boundaryNumber];
    lastBoundaryNumber = 5 - boundaryNumber;

    // of all the adjacent convex spaces which one have we connected too, 
    // across the boundary of the current convex space?
    for ( i = 0; i < currentBoundary->numberOfAdjacentConvexs_ && doesJoin == false; i++ )
    {
      j = currentBoundary->adjacentConvexIndex_[i];
      doesJoin = buildingObject.convexArray_[j].facet[lastBoundaryNumber].intersection( pointOnBoundary );
    }

    if ( doesJoin == false )
    {
      // no more boundaries to intercept
      //cout << "Found no adjacent convex space, quit." << endl;
      return false;
    }

    // if we are on the reflection boundary then we can return true right now

    isignRx = equationOfPlane.isign( pointOnBoundary );

    if ( isignRx == 0 && doesJoin == true )
    {
      if ( currentConvexSpace->conductivity_ > 0.0 
        && buildingObject.convexArray_[j].conductivity_ > 0.0 )
      {
        // very special case: reflection point inside two walls
        return false;
      }
    }

    if ( isignTx == isignRx && isignRx > 0 )
    {
      // same side of line as image point: error
      return false;
    }

    // on the line: returns true else continue along line
    if ( isignRx == 0 ) 
    {
      if ( buildingObject.convexArray_[j].conductivity_== 0.0 )
      {
        // tricky case, due to reflection boundary only touching a free
    // convex space in an edge. This actually means that a reflection
    // plane was not valid in the first place. This then gives
    // reflections in free convex spaces, which is obviously not allowed. 
    // They can only be valid in filled convex spaces.
        return false;
      }
      else
      {
        reflectionConvex   = j;
        isReflectionPoint  = true;
        reflectionPoint    = pointOnBoundary;
	if ( ( reflectionPoint - imagePoint ).abs() < TOL )
	  return false;
      }
    }
    else
    {
      // We have travelled to the other side of the current convex space so 
      // update the information for the next loop
      lastConvexNumber   = j;
      transmissionPoint  = pointOnBoundary;

      // if we have passed to the wrong side of the line then we have failed
      // to find any reflection point
      if ( buildingObject.convexArray_[j].conductivity_ > 0.0 )
         transmissionCounter++;

      // too many transmissions?
      if ( transmissionCounter > btsObject.numberOfTransmissions() )
      {
        //cout << "to many transmissions" << endl;
        return false;
      }

      if ( ( reflectionPoint - imagePoint ).abs() < TOL )
        return false;

      // Added 27/11/01
      if ( buildingObject.convexArray_[j].conductivity_ > 0.0 )
      {
        currentPathList.push( CPointInfo( transmissionPoint,
                              lastConvexNumber, lastBoundaryNumber, 
                              TRANSMISSION ) );
      }
    }
  } while ( isReflectionPoint == false );

  //cout << "reached end of routine" << endl;
  //cout << "Last Convex   encountered: " << lastConvexNumber << endl; 
  //cout << "Last Boundary encountered: " << lastBoundaryNumber << endl;
  //cout << "Reflection point found   : " << reflectionPoint << endl;

  return isReflectionPoint;
}

boolean CReflection::countTransmissionsToTx(
                                const CPointInfo&       currentPointInfo,
                                const CPoint3d&         transmitter,
                                const CBuilding&        buildingObject,
                                const CBaseTransceiver& btsObject,
                                const int&              lastConvexNumber,
                                      List<CPointInfo>& currentPathList,
                                      int&              transmissionCounter )
{
  boolean    doesJoin, isTransmitter, onBoundary;
  int        boundaryNumber, currentConvexNumber, lastBoundaryNumber, onBoundaryNumber;
  CPoint3d   pointOnBoundary, transmissionPoint;
  CBoundary  *currentBoundary;
  CPointInfo tempPointInfo;
  CConvex    *currentConvexSpace;
  int        i, j = -1;

  // nothing to do since we are in the right convex space
  if ( currentPointInfo.convexNumber == lastConvexNumber )
  {
    return true;
  }

  transmissionPoint   = currentPointInfo.node;
  currentConvexNumber = currentPointInfo.convexNumber;

  if ( currentPointInfo.contact == DIFFRACTION 
    || currentPointInfo.contact == VIRTUAL_DIFFRACTION )
  {
    if ( locateAdjacentConvex( currentPointInfo, transmitter, buildingObject,
                               currentConvexNumber ) )
    {
      //cout << "Current Convex: " << currentConvexNumber << endl;
    }
  }

  if ( currentConvexNumber == -1 ) return true;

  currentConvexSpace = &buildingObject.convexArray_[currentConvexNumber];

  onBoundary = currentConvexSpace->intersection( transmissionPoint, transmitter, 
                                                 pointOnBoundary, onBoundaryNumber );
  if ( onBoundary == true and currentConvexSpace->conductivity_ > 0.0 )
  {
    //cout << "The point is on a solid object: don't even begin calculation" << endl;
    return false;
  }

  do
  {
    doesJoin      = false;
    //doesIntersect = false;
    isTransmitter = false;

    currentConvexSpace = &buildingObject.convexArray_[currentConvexNumber];

    //doesIntersect = currentConvexSpace->notIntersection( 
    currentConvexSpace->notIntersection( transmissionPoint, transmitter, 
                                         pointOnBoundary, boundaryNumber );
    if ( boundaryNumber == 6 )
    {
      onBoundary = currentConvexSpace->intersection( transmissionPoint, transmitter, 
                                                     pointOnBoundary, onBoundaryNumber );
      //if ( onBoundary == true )
        //cout << "We found a receiver on a boundary: " << transmissionPoint << " no. " << onBoundaryNumber << endl;
      if ( onBoundary == true and currentConvexSpace->conductivity_ == 0.0 )
      {
        boundaryNumber = onBoundaryNumber;
      }
      else
      {
        //cout << "Returning false since has conductivity and on boundary" << endl;
        return false; // Added: 27/11/01
      }
      // we have gone through a corner of a room. This is not allowed.
      // only found this out by analysis of the ray paths and crashing.
    }

    currentBoundary = &currentConvexSpace->facet[boundaryNumber];
    //cout << "Current Boundary is: " << *currentBoundary << endl;
    lastBoundaryNumber = 5 - boundaryNumber;

    // of all the adjacent convex spaces which one have we connected too, 
    // across the boundary of the current convex space?
    for ( i = 0; i < currentBoundary->numberOfAdjacentConvexs_
                 && doesJoin == false ; i++ )
    {
      j = currentBoundary->adjacentConvexIndex_[i];
      doesJoin = buildingObject.convexArray_[j].facet[lastBoundaryNumber].intersection( pointOnBoundary );
    }
    //cout << "Last Convex Number:  " << lastConvexNumber << " Current Convex Number: " << j << endl;

    if ( j == lastConvexNumber )
    {
      //cout << "hit Convex containing Tx" << endl;
      isTransmitter = true;
    }

    // We have travelled to the other side of the current convex space so 
    // update the information for the next loop
    currentConvexNumber = j;
    transmissionPoint   = pointOnBoundary;

    if ( transmissionPoint == transmitter )
      return true;

    // too many transmissions? Added 22/11/01
    if ( buildingObject.convexArray_[j].conductivity_ != 0.0 )
       transmissionCounter++;
    //cout << transmissionPoint << endl;
    //cout << pointOnBoundary << " convex number: " << currentConvexSpace->conductivity_ << endl;
    //cout << "Current convex number: " << currentConvexNumber << endl;
    //cout << "Current convex space: " << currentConvexSpace->facet[0] << endl;

    if ( transmissionCounter > btsObject.numberOfTransmissions() )
    {
      //cout << "too many transmissions" << endl;
      return false;
    }
    // Added 27/11/01
    if ( buildingObject.convexArray_[j].conductivity_ != 0.0 )
    {
      tempPointInfo = CPointInfo( transmissionPoint, currentConvexNumber,
                                  lastBoundaryNumber, TRANSMISSION );
      currentPathList.push( tempPointInfo );
    }
  } while ( isTransmitter == false );

  return isTransmitter;
}

boolean CReflection::locateAdjacentConvex( const CPointInfo& currentPointInfo,
                                           const CPoint3d&   transmitter,
                                           const CBuilding&  buildingObject,
                                                 int&        currentConvexIndex)
{
  CConvex  *currentConvex;
  CPoint3d intersectionPoint;
  int      currentBoundary, N, i, j, newBound;

  currentConvexIndex = currentPointInfo.convexNumber;
  currentConvex      = &buildingObject.convexArray_[currentConvexIndex];

  if ( currentConvex->notIntersection( currentPointInfo.node, transmitter,
       intersectionPoint, newBound ) )
    return true;

  for ( currentBoundary = 0; currentBoundary < 6; currentBoundary++ )
  {
    N = currentConvex->facet[currentBoundary].numberOfAdjacentConvexs_;

    for ( i = 0; i < N; i++ )
    {
      j = currentConvex->facet[currentBoundary].adjacentConvexIndex_[i];
      if ( buildingObject.convexArray_[j].notIntersection(
           currentPointInfo.node, transmitter, intersectionPoint, newBound ) )
      {
        currentConvexIndex = j; // this is the convex we travel through
        return true;
      }
    }

    if ( currentConvex->intersection( transmitter ) )
    {
      currentConvexIndex = -1; // there is nothing to do
      return true;
    }
  }

  // we are in trouble since we can't decide which convex space
  // adjacent to the current one, we should travel through

  return false;
}
