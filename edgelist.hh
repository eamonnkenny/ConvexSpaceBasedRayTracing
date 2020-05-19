//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    December 3rd 2001
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef LIST_OF_EDGES_HH
#define LIST_OF_EDGES_HH

#include <iostream>
#include "edge.hh"
#include "list.hh"

class CEdgeList
//==========================================================================
// = FUNCTION 
// 	edgelist.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      define a list of images which can be part of a tree of images.
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
     List<CEdge> listOfEdges;

     CEdgeList()
     {
       // default constructor
     }

     inline int size() { return listOfEdges.size(); }

     ~CEdgeList()
     {
       listOfEdges.clear();
     }
};

#endif
