//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    December 13th 2001
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef CONTACT_HH
#define CONTACT_HH

enum contactType 
{
  REFLECTION, 
  TRANSMISSION, 
  DIFFRACTION, 
  TRANSMITTER,
  RECEIVER,
  VIRTUAL_DIFFRACTION  // added 04/12/02
};

enum mediaType // Added 20/12/01
{
  PEC,
  LOSSLESS,
  LOSSY
};

enum polarisationType // Added 16/04/02
{
  X_POLARISATION,
  Y_POLARISATION,
  Z_POLARISATION
};

#endif
