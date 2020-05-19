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

#include "raytracingff.hh"

#include <time.h>
int firstSeed=1;

CRayTracing::CRayTracing( const CBuilding&        buildingObject,
                          const CBaseTransceiver& btsObject,
                                CPathLoss&        pathLossObject,
                                CPoint3d*         receiverPoints,
                                int               height,
                          const mediaType&        media )
{
  List<CPlane>     reflectionPlanes;
  List<CRayPath>   validRayPath;
  List<CPointInfo> diffractionInfo;
  CComplexVector   **totalFieldPathLoss;
  CEdgeList        *diffractionImages;
  List<CImage>     *transmitterImages;

  int              i, j, t0, M, N, transConvexIndex;
  CComplexVector   fieldRayPathLoss[3], complexZeroValue;
  boolean          isInBuilding;
  CPoint3d         transmitterPoint;
  CComplexVector   Ei[3], Er[3];

  CReflection      reflectionObject;
  CDiffraction     diffractionObject;
  CIntrinsic       intrinsicObject;
  CMemoryStorage   storageObject;
  CMatlab          matlabObject;
  CCorners         cornerDiffractionPoints;
  ofstream         coutput;

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

  cout << "Ray-tracing Starting" << endl;
  // calculate possible reflection planes
  //reflectionPlanes  = buildingObject.calculateReflectionPlanes();
  buildingObject.calculateReflectionPlanes( reflectionPlanes );

  transmitterPoint = btsObject.position();
  isInBuilding     = buildingObject.locateConvexNumber( transmitterPoint,
                                                        transConvexIndex );

  // create all images of the transmitter up to a given maximum order
  t0 = intrinsicObject.StartClock();
  reflectionObject.CreateImagesOfTransmitter(
                       transmitterImages, buildingObject, btsObject,
                       reflectionPlanes, transmitterPoint,  transConvexIndex );
  intrinsicObject.ClockTime( t0, "Time to build point image tree" );

  // create diffraction image tree and return information about which
  // convex space and boundaries each edge touches.
  t0 = intrinsicObject.StartClock();
  diffractionObject.createImagesOfDiffractionEdges( diffractionImages, 
                                    buildingObject, btsObject, diffractionInfo,
                                    reflectionPlanes, cornerDiffractionPoints );
  intrinsicObject.ClockTime( t0, "Time to build edge diffraction tree" );

  cout << "Number of diffraction images: " << diffractionImages[0].size() <<
  endl;
  
  totalFieldPathLoss = storageObject.CreateComplexVectorMatrix( 
                                     pathLossObject.numberOfPoints(), 3 );

  cout << "Calculating reflection:     0 of " << setw(4)
       << pathLossObject.numberOfPoints();
  t0 = intrinsicObject.StartClock();
  // calculate all possible reflection paths first

  complexZeroValue = CComplexVector( 0.0, 0.0, 0.0 );

  for ( j = 4; j < 8; j++ )
  {
    cout << "edgelist: ";
    cout << diffractionImages[0].listOfEdges.GetItem(j).edge_ << endl;
  }

  fieldRayPathLoss[0] = complexZeroValue;  
  fieldRayPathLoss[1] = complexZeroValue;  
  fieldRayPathLoss[2] = complexZeroValue;  

  coutput.open("diffractionfield.txt");
  for ( j = 0; j < pathLossObject.numberOfPoints(); j++ )
  {
    //if ( validRayPath.GetItem(i).numberOfDiffractions() == 0 )
    totalField( j, fieldRayPathLoss[2], 
                diffractionImages[0], cornerDiffractionPoints,
                buildingObject, btsObject, media, receiverPoints[j], 
                Z_POLARISATION );

    totalFieldPathLoss[j][0] += fieldRayPathLoss[0];
    totalFieldPathLoss[j][1] += fieldRayPathLoss[1];
    totalFieldPathLoss[j][2] += fieldRayPathLoss[2];

    validRayPath.clearAndClearSubList();
    coutput << j << "   " << 20.0 * log10( fieldRayPathLoss[2].euclideanNorm() )
            << endl;
    //exit(1);
  }
  coutput.close();

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

  storageObject.DeleteImageList( transmitterImages );
  storageObject.DeleteEdgeList( diffractionImages );
  cout << endl << "Ray-tracing Ending" << endl;
  intrinsicObject.ClockTime( t0, "Time for ray-trace" );
}

void CRayTracing::totalField( int j, CComplexVector&    Et,
                              CEdgeList&         diffractionImages,
                              CCorners&          cornerDiffractionPoints,
                        const CBuilding&         buildingObject,
                        const CBaseTransceiver&  btsObject,
                        const mediaType&         media,
			const CPoint3d&          receiverPoint,
                        const polarisationType&  polarisation )
{
  List<CPointInfo> pointList, reflectionPath;
  int              i, k, currentIndex;
  double           k1, pathLength, totalPathLength;
  boolean          isValid;
  CSegment3d       diffractingEdge, si;
  CPointInfo       startPoint, reflectionPoint, nextPoint, previousPoint;
  CPoint3d         receiver, ehat, diffractionPt;
  CMemoryStorage   storageObject;
  CComplexVector   Ei;
  double           beta0;

  Et = CComplexVector();
  for ( i = 4; i < 8; i++ )
  {
    Ei = CComplexVector();

    diffractingEdge = diffractionImages.listOfEdges.GetItem(i).edge_;
    ehat            = diffractingEdge.unitVector();
    beta0 = acos( receiverPoint * ehat );

    if ( i == 4 )
    {
      diffractionPt = CPoint3d( 1.0 / tan( beta0 ), -1, 0.0 ); 
    }
    else if ( i == 5 )
    {
      diffractionPt = CPoint3d( 1.0, 1.0 / tan( beta0 ), 0.0 ); 
    }
    else if ( i == 6 )
    {
      diffractionPt = CPoint3d( -1.0 / tan( beta0 ), 1, 0.0 ); 
    }
    else 
    {
      diffractionPt = CPoint3d( -1.0, -1.0 / tan( beta0 ), 0.0 ); 
    }

    if ( j == 0 )
    {
      cout << "diffraction Point: " << diffractionPt << endl;
      cout << "beta0: " << beta0 / Pi * 180.0 << endl;
      cout << "ehat: " << ehat << endl;
      cout << "receiverPoint: " << receiverPoint << endl;
    }
    if ( j == 45 )
    {
      cout << "diffraction Point: " << diffractionPt << endl;
      cout << "beta0: " << beta0 / Pi * 180.0 << endl;
      cout << "ehat: " << ehat << endl;
      cout << "receiverPoint: " << receiverPoint << endl;
    }
    if ( j == 90 )
    {
      cout << "diffraction Point: " << diffractionPt << endl;
      cout << "beta0: " << beta0 / Pi * 180.0 << endl;
      cout << "ehat: " << ehat << endl;
      cout << "receiverPoint: " << receiverPoint << endl;
    }

    isValid = cornerDiffraction( Ei, diffractionPt.unitVector(), receiverPoint, 
                                 diffractionPt.abs(), totalPathLength, 
                                 diffractionPt, diffractingEdge, 
				 buildingObject, btsObject, media, 
				 cornerDiffractionPoints, polarisation );
    Et = Et + Ei;
  }
  
  // point is contained in.
  // amend the incident field to allow for transmissions before

  // delete any created temporary lists
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
  double         k1, d,cosTheta_i, sinSquaredTheta_i, lengthInMedia, Eibefore;
  complex        k2, sqrtTerm, permittivity, l, t, Pd, Pt;
  CPoint3d       point1, point2;
  CPointInfo     pointInfo1, pointInfo2;
  CConvex        *convexSpace1, *convexSpace2;
  CBoundary      *boundary1, *boundary2;
  boolean        isValid;
  
  if ( media == PEC && lastTransmission - firstTransmission >= 0 )
  {
    Ei = CComplexVector( 0.0, 0.0, 0.0 );
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

    if ( isValid == false )
    {
      cout << "Line enters and leaves convex space at same point" << endl;
      cout << "implies no transmission coefficient contribution to field" 
           << endl;
    }
    else
    {
      boundary1     = &convexSpace1->facet[k];
      boundary2     = &convexSpace1->facet[j];
      lengthInMedia = point1.abs( point2 );
      cosTheta_i    = boundary1->cosAngle( si.reverse() );
      d             = cosTheta_i * lengthInMedia;
  
      permittivity      = convexSpace1->complexPermittivity( btsObject );
      sinSquaredTheta_i = 1.0 - qsqr( cosTheta_i );
      sqrtTerm          = sqrt( permittivity - sinSquaredTheta_i );
      k1                = btsObject.wavenumber();
      k2                = k1 * sqrt( permittivity );
  
      l = d * sqrt( permittivity ) / sqrtTerm;
      t = d * ( cosTheta_i * sqrtTerm + sinSquaredTheta_i ) / sqrtTerm;
      Pd = expminusj( k2 * l );
      Pt = expj( k1 * t );
  
      Eibefore = Ei.euclideanNorm();
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
    Ei = Ei * ( x * w + 1.0 );
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
  CPoint3d       nhat, sihat;
  CPoint3d       ei_perpendicular, ei_parallel;
  double         k1, p, cosTheta_i;
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
  p                = si.length();
  k1               = btsObject.wavenumber();

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
  else if ( media == LOSSLESS )
  {
    cosTheta_i     = sihat * nhat * -1.0;
    if ( cosTheta_i < 0.0 )
    { 
      cout << "cos theta is negative" << endl;
      cout << sihat << endl;
      cout << nhat << endl;
      cout << si << endl;
    }
    sqrtTerm       = sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
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
  CPoint3d       nhat, sihat;
  CPoint3d       ei_perpendicular, ei_parallel;
  double         k1, p, cosTheta_i;
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
  p                = si.length();
  k1               = btsObject.wavenumber();

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
    sqrtTerm       = sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Tparallel      = 2.0 * sqrt(permittivity) * cosTheta_i 
                   / ( permittivity * cosTheta_i + sqrtTerm );
    Tperpendicular =  complex( 2.0 * cosTheta_i ) / ( cosTheta_i + sqrtTerm );
  }
  else
  {
    permittivity   = convexSpace.complexPermittivity( btsObject );
    cosTheta_i     = sihat * nhat * -1.0;
    sqrtTerm       = sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Tparallel      = sqrt(permittivity) * cosTheta_i * 2.0
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
  CPoint3d       nhat, sihat;
  CPoint3d       ei_perpendicular, ei_parallel;
  double         k1, p, cosTheta_i;
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
  p                = si.length();
  k1               = btsObject.wavenumber();

  // convert incident field into the local coordinate system
  E_iparallel      = Ei * ei_parallel;
  E_iperpendicular = Ei * ei_perpendicular;
  if ( fabs( E_iperpendicular ) > BIG_PLUS )
    cout << "Problem 1 E_iperpendicular " << endl;
  if ( fabs( E_iparallel ) > BIG_PLUS )
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
    sqrtTerm       = sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Tparallel      = 2.0 * sqrt(permittivity) * sqrtTerm 
                   / ( permittivity * cosTheta_i + sqrtTerm );
    Tperpendicular =  sqrtTerm * 2.0 / ( cosTheta_i + sqrtTerm );
    if ( fabs( Tperpendicular ) > BIG_PLUS )
      cout << "Problem 1 Tperpendicular " << endl;
    if ( fabs( Tparallel ) > BIG_PLUS )
      cout << "Problem 1 Tparallel " << endl;
  }
  else
  {
    permittivity   = convexSpace.complexPermittivity( btsObject );
    cosTheta_i     = sihat * nhat;
    sqrtTerm       = sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Tparallel      = 2.0 * sqrt(permittivity) * sqrtTerm 
                   / ( permittivity * cosTheta_i + sqrtTerm );
    Tperpendicular =  sqrtTerm * 2.0 / ( cosTheta_i + sqrtTerm );
    if ( fabs( Tperpendicular ) > BIG_PLUS )
      cout << "Problem 2 Tperpendicular " << endl;
    if ( fabs( Tparallel ) > BIG_PLUS )
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
    cout << sqrt(permittivity) << endl;
    cout << 2.0 * sqrt(permittivity) * sqrtTerm << endl;
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
  CPoint3d       nhat, sihat;
  CPoint3d       ei_perpendicular, ei_parallel;
  double         k1, p, cosTheta_i;
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
  p                = si.length();
  k1               = btsObject.wavenumber();

  // convert incident field into the local coordinate system
  E_iparallel      = Ei * ei_parallel;
  E_iperpendicular = Ei * ei_perpendicular;
  if ( fabs( E_iperpendicular ) > BIG_PLUS )
    cout << "Problem 1 E_iperpendicular " << endl;
  if ( fabs( E_iparallel ) > BIG_PLUS )
    cout << "Problem 1 E_iparallel " << endl;

  // define the transmission matrix for different dielectric types
  if ( media == LOSSLESS )
  {
    permittivity     = convexSpace.permittivity_;
    sqrtPer          = sqrt( permittivity );
    cosTheta_i       = sihat * nhat;
    cosTheta_t       = complex(1.0) / sqrtPer
                     * sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Rperpendicular12 = ( cosTheta_i - sqrtPer * cosTheta_t ) 
                     / ( cosTheta_i + sqrtPer * cosTheta_t );
    Rperpendicular23 = ( -cosTheta_i + sqrtPer * cosTheta_t ) 
                     / ( cosTheta_i + sqrtPer * cosTheta_t );
    Rparallel12      = ( -sqrtPer * cosTheta_i + cosTheta_t ) 
                     / ( sqrtPer * cosTheta_i + cosTheta_t );
    Rparallel23      = ( -sqrtPer * cosTheta_i + cosTheta_t ) 
                     / ( sqrtPer * cosTheta_i + cosTheta_t );
                  
    expjkb           = expminusj( k1 * sqrtPer * slabWidth * cosTheta_t ); 
    denomPerp        = Rperpendicular12 * Rperpendicular23 * expjkb + 1.0;
    denomPara        = Rparallel12 * Rparallel23 * expjkb + 1.0;
  }
  else
  {
    permittivity     = convexSpace.complexPermittivity( btsObject );
    sqrtPer          = sqrt( permittivity );
    cosTheta_i       = sihat * nhat;
    cosTheta_t       = complex(1.0) / sqrtPer
                     * sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Rperpendicular12 = ( cosTheta_i - sqrtPer * cosTheta_t ) 
                     / ( cosTheta_i + sqrtPer * cosTheta_t );
    Rperpendicular23 = ( -cosTheta_i + sqrtPer * cosTheta_t ) 
                     / ( cosTheta_i + sqrtPer * cosTheta_t );
    Rparallel12      = ( -sqrtPer * cosTheta_i + cosTheta_t ) 
                     / ( sqrtPer * cosTheta_i + cosTheta_t );
    Rparallel23      = ( -sqrtPer * cosTheta_i + cosTheta_t ) 
                     / ( sqrtPer * cosTheta_i + cosTheta_t );
                  
    expjkb           = expminusj( k1 * sqrtPer * slabWidth * cosTheta_t ); 
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
  phase            = expminusj( k1 * s );
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
    sqrtTerm       = sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Rparallel      = ( -permittivity * cosTheta_i + sqrtTerm )
                   / (  permittivity * cosTheta_i + sqrtTerm );
    Rperpendicular = ( cosTheta_i - sqrtTerm ) / ( cosTheta_i + sqrtTerm );
  }
  else
  {
    sqrtTerm       = sqrt( permittivity - 1.0 + qsqr( cosTheta_i ) );
    Rparallel      = ( -permittivity * cosTheta_i + sqrtTerm )
                   / (  permittivity * cosTheta_i + sqrtTerm );
    Rperpendicular = ( cosTheta_i - sqrtTerm ) / ( cosTheta_i + sqrtTerm );
  }
}

boolean CRayTracing::cornerDiffraction(      CComplexVector&   Et,
                                       const CPoint3d&         sihat,
				       const CPoint3d&         srhat,
                                       const double&           pathLength,
                                       const double&           totalPathLength,
                                       const CPoint3d&         diffractionPt,
                                       const CSegment3d&       diffractingEdge,
                                       const CBuilding&        buildingObject,
                                       const CBaseTransceiver& btsObject,
                                       const mediaType&        media,
				             CCorners&         corners,
				       const polarisationType& polarisation )
{
  CConvex   *convexSpace;
  CBoundary *boundary0, *boundary1;
  CPoint3d  n0_hat, n1_hat, ehat, si_cross_e, sr_cross_e;
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
  boolean   isCorners;
  CSegment3d s_ic, s_rc;

  CDiffractionCoefficient diffractionObject;
  CCornerDiffraction      cornerDiffObject;
  CComplexVector          Ei;

  badPoint = CPoint3d( -0.99, -0.99, -0.99 );

  // define the boundaries of the convex space containing the diffraction edge

  //if ( n0_hat != CPoint3d( 0.0, 0.0, 1.0 )  )
    //cout << "n0_hat: " << n0_hat << "n1_hat: " << n1_hat << endl;

  n0_hat = CPoint3d( 0.0, 0.0, 1.0 );
  n1_hat = CPoint3d( 0.0, 0.0, -1.0 );

  ehat   = diffractingEdge.unitVector();
  t0_hat = n0_hat ^ ehat;
  for ( int i = 0; i < buildingObject.numberOfConvexs_; i++ )
  {
    convexSpace = &buildingObject.convexArray_[i];

    if ( convexSpace->conductivity_ != 0.0 )
    {
      boundary0 = &convexSpace->facet[5];
      i = buildingObject.numberOfConvexs_;
    }
  }

  // check just inside the boundary to see if t0_hat is contained.
  // if not, then we must ensure that it will be
  p1 = diffractionPt + t0_hat * TOL3;
  if ( p1.abs( boundary0->centre() ) 
     > diffractionPt.abs( boundary0->centre() ) )
  {
    ehat   = ehat * -1.0;
    t0_hat = n0_hat ^ ehat; 
    cout << "Got t0 the wrong way around in this instance" << endl;
  }

  /*if ( boundary0->intersection( diffractionPt + t0_hat * TOL3 ) == false )
  {
    cout << "t0 not contained in plane" << endl;
    cout << "t0: " << t0_hat << "  n0: " << n0_hat << endl;
    cout << "edge: " << ehat << endl;
    cout << "boundary: " << *boundary0 << endl;
    exit(1);
  }*/

  t1_hat = n1_hat ^ ehat;

  p1 = diffractionPt + t1_hat * TOL3;

  if ( p1.abs( boundary0->centre() ) 
     > diffractionPt.abs( boundary0->centre() ) )
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

  astigmatic = 1.0 / sqrt( p );
  //attenuation    = ( p * s * qsqr( sinBeta0 ) ) / ( p + s );
  L              = p * sinBeta0 * sinBeta0;
  Lc             = sqrt(2.0);
  phase          = 1.0;

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

  //if ( diffractionPt.contact != VIRTUAL_DIFFRACTION )
  //{
    //cout << "calculated diffraction " << diffractionPt << endl;
    //Et = ( CComplexVector( beta0_hat ) * Ed_beta0 * -1.0
       //- CComplexVector( phi_hat   ) * Ed_phi );
  //}

  isCorners = cornerDiffObject.checkIfCornerDiffractionExists(
                    corners, diffractingEdge, firstCorner, secondCorner );

  if ( firstCorner != badPoint && btsObject.cornerDiffraction() == true )
     //&& diffractionPt.contact == VIRTUAL_DIFFRACTION )
  {
    calculateAnglesAndLocalCoordinates( firstCorner, ehat, t0_hat, n0_hat,
                                        sihat, srhat, diffractingEdge,
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
    //cout << Ei_beta0_dash << "  " << Ei_phi_dash << endl;

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
    Et = ( CComplexVector( betaC_hat ) * Ed_beta0 +
          CComplexVector( phi_hat   ) * Ed_phi ) *  constTerm * -1.0
	  * expj( k1 * ( srhat * firstCorner ) );
  }

  if ( secondCorner != badPoint && btsObject.cornerDiffraction() == true  )
    //&& diffractionPt.contact == VIRTUAL_DIFFRACTION )
  {
    //cout << "diffraction point: " << diffractionPt << endl;
    calculateAnglesAndLocalCoordinates( secondCorner, ehat, t0_hat, n0_hat,
                                        sihat, srhat, diffractingEdge,
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
    Et = Et + ( CComplexVector( betaC_hat ) * Ed_beta0 +
         CComplexVector( phi_hat   ) * Ed_phi ) * constTerm * -1.0
	  * expj( k1 * ( srhat * secondCorner ) );
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
					const CPoint3d& si,
                                        const CPoint3d& sr,
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
  CPoint3d   e_cross_si, e_cross_sr, sihat, srhat, newEhat;
  double     sinBetaC, cosBeta0c, cosBetaC;
  
  s_ic   = CSegment3d( CPoint3d(0.0,0.0,0.0), cornerPoint );
  sihat  = s_ic.unitVector();
  srhat  = sr;
  absS_ic = s_ic.length(); 
  absS_rc = 1.0; 
  Lc      = sqrt(2.0);

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

  //cout << "corner Point: " << cornerPoint << endl;
  if ( cornerPoint == diffractingEdge.start() )
  {
    betaC   = acos( -( sihat * newEhat ) );
    beta0c  = acos( -( newEhat * srhat ) );
  }
  else
  {
    betaC   = acos( ( sihat * newEhat ) );
    beta0c  = acos( newEhat * srhat );
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
  if ( n0_hat != CPoint3d( 0.0, 0.0, 1.0 )  )
  {
    cout << "n0_hat: " << n0_hat << "n1_hat: " << n1_hat << endl;
  }

  if ( n0_hat != CPoint3d( 0.0, 0.0, 1.0 )
    && n1_hat != CPoint3d( 0.0, 0.0, 1.0 ) )
  {
    Ei = CComplexVector();
    return false;
  }

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
  Ei_beta0_dash = Ei * beta0_dash_hat;
  Ei_phi_dash   = Ei * phi_dash_hat;

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
  //attenuation    = ( p * s * qsqr( sinBeta0 ) ) / ( p + s );
  attenuation    = ( p * s ) / ( p + s );
  phase          = expminusj( k1 * s );

  //cout << "---------------------------------------------" << endl;
  //cout << "Ei = " << Ei << endl;
  // build soft (E-polarized) are hard (H-polarized) diffraction coefficients
  Dsoft = diffractionObject.softDiffraction( phi, phi_dash, n, k1, sinBeta0, 
                                             attenuation, Rperpendicular1, 
                                             Rperpendicular2 );

  Dhard = diffractionObject.hardDiffraction( phi, phi_dash, n, k1, sinBeta0,
                                             attenuation, Rparallel1, 
                                             Rparallel2 );
  //cout << "Dsoft: " << Dsoft << "  Dhard: " << Dhard << endl;

  // define the reflection matrix for different dielectric types
  // multiply out all terms to give the electric field
  Ed_beta0 = Dsoft * Ei_beta0_dash * astigmatic * phase;
  Ed_phi   = Dhard * Ei_phi_dash   * astigmatic * phase;

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
  cout << "phi   = " << phi << endl;
  cout << "Dsoft = " << Dsoft << endl;
  cout << "Dhard = " << Dhard << endl;
  cout << "Ed_beta0 = " << Ed_beta0 << endl;
  cout << "Ed_phi   = " << Ed_phi << endl;
  cout << "Es       = " << Ei << endl;*/

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
      Et   = btsObject.incidentField( si.length(), x_l, y_l, z_l,
                                      theta, phi );
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
      Et   = btsObject.incidentField( si.length(), x_l, y_l, z_l,
                                      theta, phi );
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
      Et   = btsObject.incidentFieldHertzian( si.length(), x_l, y_l, z_l, 
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
      Et   = btsObject.incidentFieldHertzian( si.length(), x_l, y_l, z_l,
                                              theta, phi );
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
      Et   = btsObject.incidentFieldHertzian( si.length(), x_l, y_l, z_l,
                                              theta, phi );
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
