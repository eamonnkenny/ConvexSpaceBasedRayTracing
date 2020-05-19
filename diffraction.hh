//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    2nd December 2001
// Last Modification:     Dated below "Added dd/mm/yy"
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef DIFFRACTION_HH
#define DIFFRACTION_HH

#include <iostream>
#include "building.hh"
#include "reflection.hh"
#include "plane.hh"
#include "bts.hh"
#include "edgelist.hh"
#include "corners.hh"
#include "matlab.hh"

class CDiffraction
//==========================================================================
// = FUNCTION 
//     diffraction.hh
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     The definition of diffracting edges, and reflections of diffractions 
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
    CDiffraction() 
    {
      // default constructor for declarations
    }

    CDiffraction(       List<CRayPath>&   rayPathList,
                        List<CPointInfo>& diffractionInfo,
                        List<CImage>      *transmitterImages,
                        CEdgeList         *diffractionImages,
			CCorners&         cornerDiffractionPoints,
                  const CBuilding&        buildingObject,
                  const CBaseTransceiver& btsObject,
                  const int&              transmitterConvexIndex,
                  const CPoint3d&         transmitterPoint,
                  const CPoint3d&         receiverPoint );
    //
    // Added: 04/12/01
    // constructor of diffraction edges and images
    //

    void createImagesOfDiffractionEdges( 
                        CEdgeList         *diffractionEdges,
                  const CBuilding&        buildingObject,
                  const CBaseTransceiver& btsObject,
                        List<CPointInfo>& diffractionInfo,
                        List<CPlane>&     reflectionPlanes,
			CCorners&         cornerDiffractionPoints );
    // Added: 04/12/01
    // create all diffraction edges and their images by reflecting in 
    // all possible reflection planes in the building.
    //

    void findEdges( const CBuilding&        buildingObject,
                          List<CPointInfo>& diffractionInfo,
                          CEdgeList&        edgeList );
    //
    // for a given building return the possible diffraction edges.
    // we call these Edges the list of order 0 images.
    //

    void findCornerDiffractionPoints( CEdgeList& edgeList, CCorners& corners );
    //
    // calculate the position of the corners from which corner diffraction
    // is possible.
    //

    boolean constructDiffractionPoint( const CPoint3d&   imagePoint,
                                       const CSegment3d& generatingEdge,
                                       const CSegment3d& imageEdge,
                                       const CPoint3d&   receiverPoint,
                                             CPoint3d&   pointOfContact );
    //
    // Added: 06/12/01
    // This routine calculates the diffraction point on the generating edge
    // by unfolding the ray joining the image point to the receiver point.
    //

    boolean constructVirtualDiffractionPoint(
                                 const CPoint3d&   pointOfContact,
                                 const CSegment3d& generatingEdge,
                                       CCorners&   cornerDiffractionPoints );
    //
    // Added: 04/12/02
    // Construct virtual diffraction points if the virtual point is colinear
    // to the generating edge, and if the closed segment extremity is a
    // corner diffraction point
    //

    void makeArrayOfDiffractionImages(
                                 CEdge      *diffractionEdges,
                           const CEdge&     aDiffractionEdge,
                                 CEdgeList  *diffractionImages,
                                 int&       diffractionEdgeConvex,
                                 int        numberOfReflections );
    //
    // Added: 07/12/01
    // This routine calculates the images of a diffraction edge by 
    // traversing backward through the images of edges tree
    //

    void makeArrayOfImages(        CPoint3d  *pointsInEdges,
                             const CPoint3d& diffractionPoint,
                                   CEdge     *diffractionEdges,
                             const CPoint3d& receiverPoint,
                                   int       numberOfReflections );
    //
    // Added: 07/12/01
    // This routine uses the diffraction point on the generating edge
    // to make an image of the diffraction point along all diffract edge
    // images.
    //

    void createDiffractionImagesOf( CEdgeList&    previousList,
                                    List<CPlane>& reflectablePlanes,
                                    CEdgeList&    nextList );
    //
    // Added: 04/12/01
    // use the previously ordered list of images to make a new list of
    // images. so generate order i images from order i-1 images.
    //
};

#endif
