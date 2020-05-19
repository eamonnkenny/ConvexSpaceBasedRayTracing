//=========================================================================
// COPYRIGHT (c) 2015-2017 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  
// First Modification:    August 2015
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG, IP2000 STIWRO
// Current  Project:      
// Software Tool Version: 2.0
//=========================================================================
#include <iostream>
#include "configuration.hh"

int main( int argc, char *argv[] )
{
  CProperties ConfigurationObject;

  ConfigurationObject.read("directory.xml");
} 
