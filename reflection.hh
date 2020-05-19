//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    November 19th 2001
// Previous Projects:     None
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef RAY_TRACE_REFLECTIONS_HH
#define RAY_TRACE_REFLECTIONS_HH

#include <iostream>
#include "userconst.hh"
#include "building.hh"
#include "raypath.hh"
#include "point3d.hh"
#include "image.hh"
#include "plane.hh"
#include "matlab.hh"
#include "bts.hh"

class CReflection
//==========================================================================
// = FUNCTION 
//     reflection.hh
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     Raytracing algorithm for reflections and transmissions.
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//==========================================================================
{
  public:
    CReflection() 
    {
      // default constructor
    }

    CReflection(            List<CRayPath>& validRayPaths,
                            List<CImage>  *transmitterImages,
                      const CBuilding&    buildingObject,
					  const CBaseTransceiver& btsObject,
                      const int&          transConvexIndex,
                      const CPoint3d&     transmitterPoint,
                      const CPoint3d&     receiverPoint );
    //
    // given a building transmitter and receiver point, calculate a complete
    // set of 3rd order reflections and transmissions for the given points.
    //

    void CreateImagesOfTransmitter( List<CImage>  *imageList,
                              const CBuilding&    buildingObject,
							  const CBaseTransceiver& btsobject,
                                    List<CPlane>& reflectablePlanes,
                              const CPoint3d&     transmitter,
                              const int&          transmitterConvexIndex );
    //
    // Use the method of images to calculate the images of the transmitter
    // in all convex spaces up to some given maximum order
    //

    //List<CImage>& CreateFirstOrderImagesOf(  
                                   //const CPoint3d&     firstNode,
                                         //List<CPlane>& reflectablePlanes );
    void CreateFirstOrderImagesOf(  const CPoint3d&     firstNode,
                                          List<CPlane>& reflectablePlanes,
                                          List<CImage>& imageList );
    //
    // Calculate the first set of images for the transmitter remembering to 
    // delete copies of images. This occurs when we encounter coplanar
    // surfaces.
    //

    //List<CImage>& CreateImagesOf( List<CImage>&   previousList,
                                  //List<CPlane>& reflectablePlanes );
    void CreateImagesOf( List<CImage>& previousList,
                         List<CPlane>& reflectablePlanes,
                         List<CImage>& nextList );
    //
    // Using the previous image list and possibly reflectable convex spaces
    // list, we can calculate a new set of images.
    //

    //List<CRayPath>& CreateValidRayList( 
    /*void CreateValidRayList( 
                       List<CRayPath>& rayPathList,
                 const CBuilding&      buildingObject, 
                       List<CImage>    *transmitterImages, 
                 const CPoint3d&       transmitterPoint,
                 const CPoint3d&       receiverPoint,
                 const int&            transmitterConvexIndex );*/
    //
    // using the list of images of the transmitter we can calculate a
    // list of valid ray-paths.
    //

    boolean getReflections(       List<CPointInfo>& currentPathList,
                            const contactType&      lastConvexType,
                            //const int&              receiverConvexIndex,
                            const CBuilding&        buildingObject,
							const CBaseTransceiver& btsObject,
                                  CPoint3d          *pointImages,
                            const int&              numberOfImages,
                            const contactType&      generatingPointType,
                            //const int&              generatingConvexIndex,
                                  int&              transmissionCount );
    //
    // Added: 07/12/01 to simplify added reflections of diffractions
    // traverse back from a receiver point to the generating point given
    // a number of images to be traversed to see if a number of valid 
    // reflection points exist.
    //
    
    void makeArrayOfImages( CPoint3d      *imagePoints,
                      const CPoint3d&     endPoint,
                      const CImage&       currentImagePoint,
                            List<CImage>  *transmitterImages,
                      const int&          numberOfReflections );
    void makeArrayOfImages2( CPoint3d      *imagePoints,
                      const CPoint3d&     endPoint,
                      const CImage&       currentImagePoint,
                            List<CImage>  *transmitterImages,
                      const int&          numberOfReflections );
    //
    // starting at the current image point we can trace back to the
    // transmitter to create an array of images which can later be traversed
    // to create a valid ray-path
    //

    boolean validReflectionPoint( const CPointInfo&       currentPointInfo,
                                  const CPoint3d&         imagePoint,
                                  const CPoint3d&         previousImage,
                                        CPoint3d&         reflectionPoint,
                                        int&              reflectionConvex,
                                  const CBuilding&        buildingObject,
								  const CBaseTransceiver& btsObject,
                                        int&              lastConvexNumber,
                                        int&              lastBoundaryNumber,
                                        List<CPointInfo>& currentPathList,
                                        int&              transmissionCounter );
    //
    // calculating whether a reflection point is valid requires that we know
    // the first convex in which we start and travel towards the possible
    // wall reflection point. If we encounter it, we return true, the
    // reflection point and the convex in which we are currently positioned.
    // Added 20/11/01
    // transmissions updated 22/11/01
    //

    boolean countTransmissionsToTx( 
                                const CPointInfo&       currentPointInfo,
                                const CPoint3d&         transmitter,
                                const CBuilding&        buildingObject,
								const CBaseTransceiver& btsObject,
                                const int&              lastConvexNumber,
                                      List<CPointInfo>& currentPathList,
                                      int&              transmissionCounter );
    //
    // at this point we already know that we have found a valid reflection
    // point, but we still need to count the number of transmission joining
    // the first order reflection point to the transmitter. Hence this
    // routine.
    // Added 21/11/01
    // transmissions updated 22/11/01
    //

    boolean locateAdjacentConvex( const CPointInfo&  currentPointInfo,
                                  const CPoint3d&    transmitter,
                                  const CBuilding&   buildingObject,
                                        int&         convexNumber );
    //
    // Added: 11/12/01 corrected 12/12/01
    // if a point is on the corner of a convex space, we do not know how
    // to travel to the next convex space. This is a serious problem.
    //

    ~CReflection()
    {
      // default destructor
    }
};

#endif
