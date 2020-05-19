//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    December 2001
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#include "diffraction.hh"

void CDiffraction::createImagesOfDiffractionEdges( 
                      CEdgeList         *diffractionImages,
                const CBuilding&        buildingObject,
                const CBaseTransceiver& btsObject,
                      List<CPointInfo>& diffractionInfo,
                      List<CPlane>&     reflectablePlanes,
                      CCorners&         corners )
{
  int      i;

  // root of the tree is the diffraction edges themselves
  findEdges( buildingObject, diffractionInfo, diffractionImages[0] );

  //cout << "Number of Diffraction Edges: " << (diffractionImages[0].listOfEdges).size() << endl;

  if ( btsObject.numberOfReflections() > 0 )
    findCornerDiffractionPoints( diffractionImages[0], corners );

  // create a new list of images from the previous list, remembering that
  // the first order images can give us the reflectable boundaries of the
  // building
  for ( i = 1; i <= btsObject.numberOfReflections()-1; i++ )
  {
    createDiffractionImagesOf( diffractionImages[i-1], reflectablePlanes,
                               diffractionImages[i]);
  }
}

void CDiffraction::findEdges( const CBuilding&        buildingObject,
                                    List<CPointInfo>& diffractionInfo,
                                    CEdgeList&        edgeList )
{
  int        i, j, j1, k, l, m, n, kk;
  CConvex    *currentConvex, *adjacentConvexInAi, *adjacentConvexInAj;
  CBoundary  *Bi, *Bj, *adjacentBoundaryInAi, *adjacentBoundaryInAj;
  CBoundary  *newBoundary1, *newBoundary2;
  CPointInfo currentPointInfo;
  CPoint3d   origin;
  List<int>  Ai, Aj;
  CSegment3d seg, newSeg2, newSegment;
  boolean    isContained;

  origin   = CPoint3d( 0.0, 0.0, 0.0 );

  for ( k = 0; k < buildingObject.numberOfConvexs_; k++ )
  {
    currentConvex = &buildingObject.convexArray_[k];
    // only continue if we have encountered a filled convex space.
    if ( currentConvex->conductivity_ != 0.0 )
    {
      cout << "Assessing edges of a conductive box with index: " << k << endl;
      // there are 12 possible combinations of 2 boundaries containing
      // an edge which we can consider in this algorithm
      // now extract the 2 boundaries and the contained segment edge

      for ( kk = 0; kk < 12; kk++ )
      {
        switch( kk )
        {  
          case  0:
            i   = 0;
            j   = 1;
            Bi  = &currentConvex->facet[0];
            Bj  = &currentConvex->facet[1];
            seg = CSegment3d( Bi->vertex_[0], Bi->vertex_[3] );
            break;
          case  1: 
            i   = 0;
            j   = 2;
            Bi  = &currentConvex->facet[0];
            Bj  = &currentConvex->facet[2];
            seg = CSegment3d( Bi->vertex_[3], Bi->vertex_[2] );
            break;
          case  2: 
            i   = 0;
            j   = 4;
            Bi  = &currentConvex->facet[0];
            Bj  = &currentConvex->facet[4];
            seg = CSegment3d( Bi->vertex_[2], Bi->vertex_[1] );
            break;
          case  3: 
            i   = 0;
            j   = 3;
            Bi  = &currentConvex->facet[0];
            Bj  = &currentConvex->facet[3];
            seg = CSegment3d( Bi->vertex_[1], Bi->vertex_[0] );
            break;
          case  4: 
            i   = 3;
            j   = 1;
            Bi  = &currentConvex->facet[3];
            Bj  = &currentConvex->facet[1];
            seg = CSegment3d( Bi->vertex_[0], Bi->vertex_[1] );
            break;
          case  5: 
            i   = 1;
            j   = 2;
            Bi  = &currentConvex->facet[1];
            Bj  = &currentConvex->facet[2];
            seg = CSegment3d( Bi->vertex_[1], Bi->vertex_[2] );
            break;
          case  6: 
            i   = 2;
            j   = 4;
            Bi  = &currentConvex->facet[2];
            Bj  = &currentConvex->facet[4];
            seg = CSegment3d( Bi->vertex_[1], Bi->vertex_[2] );
            break;
          case  7: 
            i   = 4;
            j   = 3;
            Bi  = &currentConvex->facet[4];
            Bj  = &currentConvex->facet[3];
            seg = CSegment3d( Bi->vertex_[0], Bi->vertex_[1] );
            break;
          case  8: 
            i   = 5;
            j   = 1;
            Bi  = &currentConvex->facet[5];
            Bj  = &currentConvex->facet[1];
            seg = CSegment3d( Bi->vertex_[0], Bi->vertex_[1] );
            break;
          case  9: 
            i   = 5;
            j   = 2;
            Bi  = &currentConvex->facet[5];
            Bj  = &currentConvex->facet[2];
            seg = CSegment3d( Bi->vertex_[1], Bi->vertex_[2] );
            break;
          case 10: 
            i   = 5;
            j   = 4;
            Bi  = &currentConvex->facet[5];
            Bj  = &currentConvex->facet[4];
            seg = CSegment3d( Bi->vertex_[2], Bi->vertex_[3] );
            break;
          case 11: 
            i   = 5;
            j   = 3;
            Bi  = &currentConvex->facet[5];
            Bj  = &currentConvex->facet[3];
            seg = CSegment3d( Bi->vertex_[3], Bi->vertex_[0] );
            break;
          default:
            cerr << "None of the 12 permutations was used" << endl;
            exit(1);
        }
        //cout << seg << endl;

        // see if there are any adjacent convex spaces to Bi and Bj
        for ( l = 0; l < Bi->numberOfAdjacentConvexs_; l++ )
        {
          m = Bi->adjacentConvexIndex_[l];
          if ( buildingObject.convexArray_[m].conductivity_ == 0.0 )
            Ai.append(m);
        }
  
        for ( l = 0; l < Bj->numberOfAdjacentConvexs_; l++ )
        {
          n = Bj->adjacentConvexIndex_[l];
          if ( buildingObject.convexArray_[n].conductivity_ == 0.0 )
            Aj.append(n);
        }

        //cout << "Adjacent convex numbers: " << Ai.size() << " and " << Aj.size() << endl;
        // Let Cm be a free convex space of Ai attached to Bi.
        // If there is an adjacent convex space Cn contained in Aj
        // then intersect Cn with the edge to get a diffraction edge.
        for ( m = 0; m < Ai.size(); m++ )
        {
          adjacentConvexInAi   = &buildingObject.convexArray_[Ai.GetItem(m)];
          adjacentBoundaryInAi = &adjacentConvexInAi->facet[j];
          for ( l = 0; l < adjacentBoundaryInAi->numberOfAdjacentConvexs_; l++ )
          {
            j1 = adjacentBoundaryInAi->adjacentConvexIndex_[l];
            for ( n = 0; n < Aj.size(); n++ )
            {
              if ( j1 == Aj.GetItem(n) )
              {
                isContained  = false;
                newBoundary1 = &buildingObject.convexArray_[j1].facet[5-j];
                isContained  = newBoundary1->intersection( seg, newSegment );

                if ( isContained == true )
                {
                  newBoundary2 = &adjacentConvexInAi->facet[5-i];
                  isContained  = newBoundary2->intersection( newSegment, 
                                                            newSeg2 );
                }

                if ( isContained == true && newSeg2.start() != newSeg2.end() )
                  if ( newBoundary1->equation_ 
                  != adjacentConvexInAi->facet[5-j].equation_ 
                  && buildingObject.convexArray_[j1].facet[5-i].equation_ 
                  != newBoundary2->equation_ )
                {
                  edgeList.listOfEdges.append( CEdge( newSeg2, -1 ) );
                  currentPointInfo = CPointInfo( origin, k, i, j, -1,
                                                 DIFFRACTION );
                  diffractionInfo.append( currentPointInfo );
                }
              }
            }
          }
        }

        // Let Cm be a free convex space of Aj attached to Bj.
        // If there is an adjacent convex space Cn contained in Ai
        // then intersect Cn with the edge to get a diffraction edge.
        for ( m = 0; m < Aj.size(); m++ )
        {
          adjacentConvexInAj   = &buildingObject.convexArray_[Aj.GetItem(m)];
          adjacentBoundaryInAj = &adjacentConvexInAj->facet[i];
          for ( l = 0; l < adjacentBoundaryInAj->numberOfAdjacentConvexs_; l++ )
          {
            j1 = adjacentBoundaryInAj->adjacentConvexIndex_[l];
            for ( n = 0; n < Ai.size(); n++ )
            {
              if ( j1 == Ai.GetItem(n) )
              {
                isContained  = false;
                newBoundary1 = &buildingObject.convexArray_[j1].facet[5-i];
                isContained  = newBoundary1->intersection( seg, newSegment );
                if ( isContained == true )
                {
                  newBoundary2 = &adjacentConvexInAj->facet[5-j];
                  isContained  = newBoundary2->intersection( newSegment, 
                                                            newSeg2 );
                }
                if ( isContained == true && newSeg2.start() != newSeg2.end() )
                  if ( newBoundary1->equation_ 
                  != adjacentConvexInAj->facet[5-i].equation_ 
                  && buildingObject.convexArray_[j1].facet[5-j].equation_ 
                  != newBoundary2->equation_ )
                {
                  currentPointInfo = CPointInfo( origin, k, i, j, -1, 
                                                 DIFFRACTION );
                  diffractionInfo.append( currentPointInfo );
                  edgeList.listOfEdges.append( CEdge( newSeg2, -1 ) );
                }
              }
            }
          }
        }

        Ai.clear();
        Aj.clear();
      }
    }
  }

  for ( i = 0; i < edgeList.listOfEdges.size(); i++ )
  {
    seg = edgeList.listOfEdges.GetItem(i).edge_;
    m   = diffractionInfo.GetItem(i).convexNumber;

    for ( j = i+1; j < edgeList.listOfEdges.size(); j++ )
    {
      if ( seg.attachTo( edgeList.listOfEdges.GetItem(j).edge_, newSegment ) )
      {
        n = diffractionInfo.GetItem(j).convexNumber;
        // only if the convex spaces m and n are the same should we combine
        if ( m == n )
        {
          edgeList.listOfEdges.Delete(j); // delete larger index j first
          edgeList.listOfEdges.Delete(i); // then delete smaller index i
          edgeList.listOfEdges.append( CEdge( newSegment, -1 ) );

          diffractionInfo.Delete(j);
          currentPointInfo = diffractionInfo.GetAndDelete(i);
          diffractionInfo.append( currentPointInfo );
          j = j-1; // decrease indexing since we don't want to miss an edge
        }
      }
    }
  }

  // delete multiple copies of an edge
  for ( i = 0; i < edgeList.listOfEdges.size(); i++ )
  {
    seg = edgeList.listOfEdges.GetItem(i).edge_;

    for ( j = i+1; j < edgeList.listOfEdges.size(); j++ )
    {
      if ( seg == edgeList.listOfEdges.GetItem(j).edge_ )
      {
        edgeList.listOfEdges.Delete(j);
          diffractionInfo.Delete(j);
        j = j-1;
      }
    }
  }

  //for ( i = 0; i < edgeList.listOfEdges.size(); i++ )
    //cout << "\t " << i << ": " << edgeList.listOfEdges.GetItem(i).edge_
         //<< endl;

  //cout << endl << "Diffraction Image Tree Order 0 size: " 
       //<< edgeList.listOfEdges.size() << endl;

  //return *edgeList;
}

void CDiffraction::findCornerDiffractionPoints( CEdgeList& edgeList, 
                                         CCorners&  cornerDiffractionPoints )
{
  int        i, j, k, cornerCount;
  CSegment3d edgeI, edgeJ, edgeK;

  for ( i = 0; i < edgeList.size(); i++ )
  {
    edgeI = edgeList.listOfEdges.GetItem(i).edge_;
    cornerCount = 0;

    for ( j = i+1; j < edgeList.size(); j++ ) 
    {
      edgeJ = edgeList.listOfEdges.GetItem(j).edge_;

      if ( edgeI.intersection( edgeJ.start() ) )
      {
        for ( k = 0; k < edgeList.size(); k++ )
        {
          edgeK = edgeList.listOfEdges.GetItem(k).edge_;
          if ( edgeK.intersection( edgeJ.start() ) )
          {
            if ( i != k && j != k && i != j )
            {
              cornerDiffractionPoints.append( edgeJ.start(), edgeI, edgeJ,
                                              edgeK, cornerCount );
            }
          }
          if ( cornerCount == 2 ) 
            k = edgeList.size();
        }
      }
      else if ( edgeI.intersection( edgeJ.end() ) ) 
      {
        for ( k = 0; k < edgeList.size(); k++ )
        {
          edgeK = edgeList.listOfEdges.GetItem(k).edge_;
          if ( edgeK.intersection( edgeJ.end() ) )
          {
            if ( i != k && j != k && i != j )
            {
              cornerDiffractionPoints.append( edgeJ.end(), edgeI, edgeJ,
                                              edgeK, cornerCount );
            }
          }
          if ( cornerCount == 2 ) 
            k = edgeList.size();
        }
      }
      if ( cornerCount == 2 ) 
        j = edgeList.size();
    }
  }
}

void CDiffraction::createDiffractionImagesOf( 
                                         CEdgeList&   previousList,
                                         List<CPlane>& reflectablePlanes,
                                         CEdgeList&   currentImageList )
{
  int         k, m;
  //CEdgeList   *currentImageList;
  CSegment3d  currentSegment, newSegmentImage;
  boolean     isValid = false, isRepeated;

  // create new empty image of edges list
  //currentImageList = new CEdgeList;

  //for all previous image calculate new images in convex

  for ( k = 0; k < previousList.listOfEdges.size(); k++ )
  {
    currentSegment = previousList.listOfEdges.GetItem(k).edge_;

    for ( m = 0; m < reflectablePlanes.size(); m++ )
    {
      isValid = reflectablePlanes.GetItem(m).validImage( currentSegment, 
                                                         newSegmentImage );

      // we want to get the image of a previous image edge now, and insert
      // it into the list of edges.

      isRepeated = false;
      /*for ( i = 0; i < currentImageList.listOfEdges.size(); i++ )
        if ( currentImageList.listOfEdges.GetItem(i).edge_ == newSegmentImage )
          isRepeated = true;*/

      if ( isValid == true && isRepeated == false )
        currentImageList.listOfEdges.append( CEdge( newSegmentImage, k ) ); 
    }
  }

  //return *currentImageList;
}

CDiffraction::CDiffraction( 
                        List<CRayPath>&   rayPathList,
                        List<CPointInfo>& diffractionInfo,
                        List<CImage>      *transmitterImages,
                        CEdgeList         *diffractionImages,
                        CCorners&         corners,
                  const CBuilding&        buildingObject,
                  const CBaseTransceiver& btsObject,
                  const int&              transmitterConvexIndex,
                  const CPoint3d&         transmitterPoint,
                  const CPoint3d&         receiverPoint )
{
  CMemoryStorage   StorageObject;
  CPointInfo       currentPointInfo;
  boolean          isInBuilding, isValid = false, isReflectionsValid, virtualValid = false;
  CPoint3d         reflectionPoint, diffractionPoint;
  CImage           currentImage, previousImage, aTransmitterImage;
  int              j, k, m, n;
  int              receiverConvexIndex;
  int              edgeNumber;
  int              transmissionCounter, trans1;
  List<CPointInfo> currentPathList;
  CReflection      reflectionObject;
  CEdge            diffImageEdge, aDiffractionEdge;
  CEdge            *diffractionEdgeArray;
  CPoint3d         *transmitterImageArray, *imagesOfDiffractionPoints;

  isInBuilding = buildingObject.locateConvexNumber( receiverPoint,
                                                    receiverConvexIndex );

  //diffractionEdgeConvex = StorageObject.CreateIntegerVector( 
                                     //diffractionImages[0].listOfEdges.size() );

  //buildingObject.locateConvexNumber( diffractionImages[0].listOfEdges,
                                     //diffractionEdgeConvex );

  // traverse the tree of images for all orders
  for ( m = 1; m <= btsObject.numberOfReflections(); m++ )
  {
    for ( n = 0; n <= m-1; n++ )
    {
      cout << "Reflection/Diffraction Path: Tx -> " << m-n-1 << "R -> 1D -> " 
           << n << "R -> Rx" << endl;

      for ( k = 0; k < diffractionImages[n].listOfEdges.size(); k++ )
      {
        aDiffractionEdge = diffractionImages[n].listOfEdges.GetItem(k);

        diffractionEdgeArray = StorageObject.CreateEdgeVector( n+1 );

        if ( n == 0 ) 
          edgeNumber = k;

        makeArrayOfDiffractionImages( diffractionEdgeArray, aDiffractionEdge, 
                                      diffractionImages, edgeNumber, n );

        for ( j = 0; j < transmitterImages[m-n-1].size(); j++ )
        {
          aTransmitterImage = transmitterImages[m-n-1].GetItem(j);

          //construct a diffraction point in the generating diffraction edge
          isValid = constructDiffractionPoint( 
                     aTransmitterImage.point_, diffractionEdgeArray[0].edge_,
                     diffractionEdgeArray[n].edge_, receiverPoint,
                     diffractionPoint );
		     
          if ( btsObject.cornerDiffraction() == true )
          {
            virtualValid = false;
            if ( isValid == false )
            {
              virtualValid = constructVirtualDiffractionPoint( diffractionPoint,
                              diffractionEdgeArray[n].edge_, corners ); 
            }
          }

          if ( isValid == true || virtualValid == true )
          {
            // makes an array of diffraction points images 
            imagesOfDiffractionPoints = StorageObject.CreatePoint3dVector(n+2);

            makeArrayOfImages( imagesOfDiffractionPoints, diffractionPoint, 
                               diffractionEdgeArray, receiverPoint, n );
  
            transmissionCounter = 0;
            if ( virtualValid == true && isValid == false )
            {
              isReflectionsValid = reflectionObject.getReflections( 
                                    currentPathList, RECEIVER, buildingObject,
                                    btsObject, imagesOfDiffractionPoints, n, 
                                    VIRTUAL_DIFFRACTION, transmissionCounter );
            }
            else
            {
              isReflectionsValid = reflectionObject.getReflections( 
                                    currentPathList, RECEIVER, buildingObject,
                                    btsObject, imagesOfDiffractionPoints, n,
                                    DIFFRACTION, transmissionCounter );
            }

            // makes an array of images of transmitter using an image
            transmitterImageArray = StorageObject.CreatePoint3dVector( m-n+1 );
            trans1 = transmissionCounter;

            if ( isReflectionsValid == true )
            {
              // add the diffraction point ot the ray containing points
              currentPointInfo            = diffractionInfo.GetItem(edgeNumber);
              currentPointInfo.node       = diffractionPoint;
              currentPointInfo.edgeNumber = edgeNumber;
              if ( virtualValid == true && isValid == false )
                currentPointInfo.contact = VIRTUAL_DIFFRACTION;
              currentPathList.push( currentPointInfo );

              reflectionObject.makeArrayOfImages(
                                 transmitterImageArray, diffractionPoint,
                                 aTransmitterImage, transmitterImages, m-n-1 );

              if ( virtualValid == true && isValid == false )
              {
                isReflectionsValid = reflectionObject.getReflections(
                                      currentPathList, VIRTUAL_DIFFRACTION,
                                      buildingObject, btsObject, 
                                      transmitterImageArray, 
                                      m-n-1, TRANSMITTER, 
                                      transmissionCounter );
              }
              else
              {
                isReflectionsValid = reflectionObject.getReflections(
                                      currentPathList, DIFFRACTION,
                                      buildingObject, btsObject, 
                                      transmitterImageArray, 
                                      m-n-1, TRANSMITTER, 
                                      transmissionCounter );
              }

              if ( transmissionCounter <= btsObject.numberOfTransmissions()
                && isReflectionsValid )
              {    
                rayPathList.append( CRayPath( currentPathList,
                                              transmissionCounter, m-1, 1 ) );
              }

            }

            StorageObject.DeletePoint3dVector( imagesOfDiffractionPoints );
            StorageObject.DeletePoint3dVector( transmitterImageArray );
          }
          currentPathList.clear();
        }
        StorageObject.DeleteEdgeVector( diffractionEdgeArray );
      }
    }
  }
  //StorageObject.DeleteIntegerVector( diffractionEdgeConvex );
}


void CDiffraction::makeArrayOfDiffractionImages(
                                CEdge     *diffractionEdges,
                          const CEdge&    aDiffractionEdge,
                                CEdgeList *diffractionImages,
                                int&      generatingEdgeNumber,
                                int       numberOfReflections )
{
  int l;

  diffractionEdges[numberOfReflections] = aDiffractionEdge;

  for ( l = numberOfReflections-1; l >= 0; l-- )
  {
    generatingEdgeNumber = diffractionEdges[l+1].previousImageIndex_;
    diffractionEdges[l]  = diffractionImages[l].listOfEdges.GetItem(
                                             generatingEdgeNumber );
  }
}

void CDiffraction::makeArrayOfImages(       CPoint3d  *pointsInEdges,
                                      const CPoint3d& diffractionPoint,
                                            CEdge     *diffractionEdges,
                                      const CPoint3d& receiverPoint, 
                                            int       numberOfReflections )
{
  int      l;
  double   weight, lengthToStartPoint, totalLength;
  CPoint3d unitVectorOnEdge;

  l                 = numberOfReflections;
  pointsInEdges[l+1]= receiverPoint;
  lengthToStartPoint=(diffractionPoint-diffractionEdges[0].edge_.start()).abs();
  totalLength       = diffractionEdges[0].edge_.length();
  weight            = lengthToStartPoint / totalLength;

  //if ( weight < -TOL || weight > 1+TOL )
  //{
    //cout << "you picked the wrong diffraction point, somehow!!!" << endl;
    //exit(1);
  //}


  for ( l = numberOfReflections; l >= 0; l-- )
  {
    unitVectorOnEdge = diffractionEdges[l].edge_.unitVector();
    pointsInEdges[l] = diffractionEdges[l].edge_.start() 
                     + unitVectorOnEdge * lengthToStartPoint;
    //cout << "number of reflections: " << numberOfReflections << "   " << 
    //pointsInEdges[l] << endl;
    if ( pointsInEdges[l] == CPoint3d( -16.5, -16.5, 1.5 ) )
    {
      cout << "got you mother" << endl;
    }
    if ( pointsInEdges[l] == CPoint3d( -16.5, 37.5, 1.5 ) )
    {
      cout << "got you mother 2" << endl;
    }
  }
}

boolean CDiffraction::constructVirtualDiffractionPoint(
                                          const CPoint3d&   pointOfContact,
                                          const CSegment3d& generatingEdge,
                                                CCorners&   corners )
{
  double p1, q1;
  p1 = (generatingEdge.start()).abs( pointOfContact );
  q1 = (generatingEdge.end()).abs( pointOfContact );

  if ( p1 < q1 )
  {
    if ( corners.isPointACorner( generatingEdge.start() ) ) 
      return true; 
  }
  else
  {
    if ( corners.isPointACorner( generatingEdge.end() ) )
      return true;
  }
  return false;
}

boolean CDiffraction::constructDiffractionPoint(
                                          const CPoint3d&   transmitterImage,
                                          const CSegment3d& generatingEdge,
                                          const CSegment3d& imageEdge,
                                          const CPoint3d&   receiverPoint,
                                                CPoint3d&   pointOfContact )
{
  CPoint3d v1, v2, l1vector, l2vector, Ad, Bd, pointOfContact2;
  double   l1, l2, d, d1, d2;

  v1 = generatingEdge.unitVector();
  v2 = imageEdge.unitVector();

  // cross product to get shortest distance
  l1vector = ( transmitterImage - generatingEdge.start() ) ^ v1;  
  l2vector = ( receiverPoint    - imageEdge.start() ) ^ v2;

  // l1 and l2 are those shortest distances
  l1 = l1vector.abs();
  l2 = l2vector.abs();

  // subtract cross product vector from points to get point of contact
  // of closest point of contact on edges.
  d1 = ( transmitterImage - generatingEdge.start() ) * v1;
  d2 = ( receiverPoint    - imageEdge.start()      ) * v2;

  Ad = generatingEdge.start() + v1 * d1;
  Bd = imageEdge.start()      + v2 * d2;

  // Now get the total distance between the two points if they where
  // on the one line.
  d  = d2 - d1;

  // Then use the law of similar triangles to generate where between Ad and
  // Bd the point of contact exists.
  if ( l1 == 0.0 && l2 == 0.0 )
  {
    cerr << "divide by zero about to happen" << endl;
    cout << "Transmitter image: " << transmitterImage << endl;
    cout << "Diffraction edge: " << generatingEdge << endl;
    cout << "Receiver    point: " << receiverPoint << endl;
    exit(1);
  }

  pointOfContact = Ad + v1 * ( d * l1 / ( l1 + l2 ));

  pointOfContact2 = Bd + v2 * ( d * l2 / ( l1 + l2 ));

  if ( generatingEdge.intersection( pointOfContact ) )
  {
    d1 = CSegment3d( pointOfContact, transmitterImage ).angle( v1 );
    d2 = CSegment3d( pointOfContact2, receiverPoint).angle( v2 );
    if ( fabs( d1 - d2 ) > TOL )
    {
      cout << "angle1: " << d1 << endl;
      cout << "angle2: " << d2 << endl;
      cout << "pointOfContact: " << pointOfContact << endl;
      cout << "pointOfContact2: " << pointOfContact2 << endl;
      cout << "transmitterImage: " << transmitterImage << endl;
      cout << "receiverPoint: " << receiverPoint << endl;
      cout << generatingEdge << endl;
      cout << imageEdge << endl;
    }
  }

  // if the point of contact is contained in the line segment, then return
  // it to the calling module, otherwise return false.

  if ( generatingEdge.intersection( pointOfContact ) )
    return true;
  else
    return false;
}
