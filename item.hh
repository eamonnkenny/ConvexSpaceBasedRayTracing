//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    November 19th 2000
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#ifndef TCD_ITEM_HH
#define TCD_ITEM_HH

#include "userconst.hh"

template<class Type>
class TCDListItem
//=========================================================================
// = FUNCTION
//     item.hh
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
//
// = COPYRIGHT
//     COPYRIGHT (c) 1995-1999 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//=========================================================================
{
   public:
       Type  T;
       TCDListItem<Type> *next;

       //TCDListItem( const Type& T1 ) : T( T1 ), next(0) { }
       TCDListItem( const Type& T1 ) { T = T1 ; next = 0; }

       TCDListItem( const TCDListItem<Type>& it )
       {
         if ( (&T) == NULL )
           cerr << " NULL pointer " << endl;
         T = it.T;
         next = 0;
       }
    
       TCDListItem<Type>& operator=( const TCDListItem<Type>& it )
       {
         if ( this == NULL )
           cerr << " NULL TCDListItem " << endl;
         if ( (&T) == NULL )
           cerr << " NULL pointer " << endl;
         T = it.T;
         next = 0;
         return *this;
       }
    
       ~TCDListItem() {}
       //friend class List<Type>;
};
    
#endif
