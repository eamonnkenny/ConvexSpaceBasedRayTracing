//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Project Manager:       Dr. Peter J. Cullen
// Last Modification:     October 2001
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef MEMORY_STORAGE_HH
#define MEMORY_STORAGE_HH

#include <iostream>

#include "userconst.hh"
#include "ComplexVector.hh"
#include "convex.hh"
#include "room.hh"
#include "story.hh"
#include "floor.hh"
#include "image.hh"
#include "edge.hh"
#include "edgelist.hh"

#ifdef __GNUG__
  #define EXPORT_CLASS_DECLSPEC class 
#else
  #define EXPORT_CLASS_DECLSPEC class __declspec(dllexport)
#endif

EXPORT_CLASS_DECLSPEC  CMemoryStorage
//=========================================================================
// = FUNCTION
//     storage.hh
//
// = AUTHOR
//     Liam O'Brien, Eamonn M. Kenny
//
// = DESCRIPTION
//     This class is used for dynamically allocating memory to 
//     multidimensional arrays
//
// = VERSION
//     1.0
//=========================================================================
{
   public:
     int**       CreateIntegerMatrix( int, int ); 
     int**       CreateIntegerMatrix( int, int* ); 
     double**    CreateDoubleMatrix( int, int );
     boolean**   CreateBoolMatrix( int, int );
     complex**   CreateComplexMatrix( int, int );
     complex**   CreateComplexvarMatrix( int, int* );
     CPoint3d**  CreatePoint3dMatrix( int, int );
     CPoint3d*** CreatePoint3dtripleMatrix( int, int, int );
     double***   CreateDouble3dMatrix( int, int, int );
     complex***  CreateComplex3dMatrix( int, int, int );
     CComplexVector**   CreateComplexVectorMatrix( int, int );
     //
     // Creation of Integer, Double, Bool, Complex and Point3d matrices
     //
     
     void        DeleteIntegerMatrix( int**, int );
     void        DeleteDoubleMatrix( double**, int );
     void        DeleteBoolMatrix( boolean**, int );
     void        DeleteComplexMatrix( complex**, int );
     void        DeleteComplexvarMatrix( complex**, int   );
     void        DeletePoint3dMatrix( CPoint3d**, int );
     void        DeletePoint3dtripleMatrix( CPoint3d***, int, int );
     void        DeleteDouble3dMatrix( double***, int, int );
     void        DeleteComplex3dMatrix( complex***, int, int );
     void        DeleteComplexVectorMatrix( CComplexVector**, int );
     //
     // Deletion of Integer, Double, Bool, Complex and Point3d matrices
     //

     int*            CreateIntegerVector(   int Num );
     boolean*        CreateBoolVector(      int Num );
     double*         CreateDoubleVector(    int Num ); 
     complex*        CreateComplexVector(   int Num );
     CComplexVector* CreateComplex3dVector( int Num );
     CPoint3d*       CreatePoint3dVector(   int Num );
     CConvex*        CreateConvexVector(    int Num );
     CRoom*          CreateRoomVector(      int Num );
     CStory*         CreateStoryVector(     int Num );
     CFloor*         CreateFloorVector(     int Num );
     CImage*         CreateImageVector(     int Num );
     CEdge*          CreateEdgeVector(      int Num );
     CEdgeList*      CreateEdgeList(        int Num );
     List<CImage>*   CreateImageList(       int Num );
     //
     // Creation of Integer, Double, Bool, and Complex vectors
     //

     void     DeleteIntegerVector(   int*            v );
     void     DeleteBoolVector(      boolean*        v );
     void     DeleteDoubleVector(    double*         v );
     void     DeleteComplexVector(   complex*        v );
     void     DeleteComplex3dVector( CComplexVector* v );
     void     DeletePoint3dVector(   CPoint3d*       v );
     void     DeleteConvexVector(    CConvex*        v );
     void     DeleteRoomVector(      CRoom*          v );
     void     DeleteFloorVector(     CFloor*         v );
     void     DeleteStoryVector(     CStory*         v );
     void     DeleteImageVector(     CImage*         v );
     void     DeleteEdgeVector(      CEdge*          v );
     void     DeleteEdgeList(        CEdgeList*      v );
     void     DeleteImageList(       List<CImage>*   v );
     //
     // Deletion of Integer, Double, Bool, and Complex vectors
     //
};

#endif
