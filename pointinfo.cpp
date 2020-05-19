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
#include "pointinfo.hh"

CPointInfo::CPointInfo( const CPoint3d& node1, int  convexNumber1, 
                        int   b1, contactType contact1 )
{
  // constructor
  if ( contact1 != DIFFRACTION && contact1 != VIRTUAL_DIFFRACTION )
  {
    node              = node1;
    convexNumber      = convexNumber1;
    boundaryNumber1   = b1;
    contact           = contact1;
  }
  else
  {
    cerr << "can't use point contact constructor for: " << contact1 << endl;
    exit(1);
  }
}

CPointInfo::CPointInfo( const CPoint3d& node1, int convexNumber1, 
                        int   b1, int b2, int edgeNumber1,
                        contactType contact1 )
{
  // constructor Added 13/12/01
  if ( contact1 == DIFFRACTION || contact1 == VIRTUAL_DIFFRACTION )
  {
    node              = node1;
    convexNumber      = convexNumber1;
    boundaryNumber1   = b1;
    boundaryNumber2   = b2;
    edgeNumber        = edgeNumber1;
    contact           = contact1;
  }
  else
  {
    cerr << "can't use diffraction contact constructor for: " << contact1;
    cerr << endl;
    exit(1);
  }
}

CPointInfo& CPointInfo::operator=( const CPointInfo& otherPointInfo )
{
  if ( this != &otherPointInfo )
  {
    if ( otherPointInfo.contact == DIFFRACTION
      || otherPointInfo.contact == VIRTUAL_DIFFRACTION )
    {
      node              = otherPointInfo.node;
      convexNumber      = otherPointInfo.convexNumber;
      edgeNumber        = otherPointInfo.edgeNumber;
      boundaryNumber1   = otherPointInfo.boundaryNumber1;
      boundaryNumber2   = otherPointInfo.boundaryNumber2;
      contact           = otherPointInfo.contact;
    }
    else
    {
      node              = otherPointInfo.node;
      convexNumber      = otherPointInfo.convexNumber;
      boundaryNumber1   = otherPointInfo.boundaryNumber1;
      contact           = otherPointInfo.contact;
    }
  }

  return *this;
}

ostream& operator<<( ostream& s, const CPointInfo& pi )
{
  s << "( " << pi.node << " )  ";

  s << pi.convexNumber << "  ";

  if ( pi.contact == DIFFRACTION || pi.contact == VIRTUAL_DIFFRACTION )
  {
    s << pi.boundaryNumber1 << "  ";
    s << pi.boundaryNumber2 << "  ";
  }
  else
    s << pi.boundaryNumber1 << "  ";

  s << pi.edgeNumber << "  ";

  if ( pi.contact == DIFFRACTION )
    s << " DIFFRACTION " << endl;
  else if ( pi.contact == REFLECTION )
    s << " REFLECTION " << endl;
  else if ( pi.contact == TRANSMITTER )
    s << " TRANSMITTER " << endl;
  else if ( pi.contact == RECEIVER )
    s << " RECEIVER " << endl;
  else if ( pi.contact == TRANSMISSION )
    s << " TRANSMISSION " << endl;
  else if ( pi.contact == VIRTUAL_DIFFRACTION )
    s << " VIRTUAL_DIFFRACTION " << endl;

  return s;
}
