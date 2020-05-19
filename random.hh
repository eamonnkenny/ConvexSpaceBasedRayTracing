//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Prof. Frank Boland
// First Modification:    3rd March 2003
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef RANDOM_NUMBER_HH
#define RANDOM_NUMBER_HH

#include <time.h>

class CRandomNumber
{
  public:
    CRandomNumber()
    {
      // default constructor
    }

    double randomVariable( int& firstSeed )
    {
      time_t   ltime; 
      long int Seed;
  
      if ( firstSeed == 1 )
      { 
        time( &ltime );
        Seed=ltime;
        firstSeed=0;
        srand( Seed );
      }
          
      return double(rand())/double(RAND_MAX);
    }
};

#endif
