%=========================================================================
% COPYRIGHT (c) 2000-2003 by Trinity College Dublin, Dublin 2, IRELAND
% All rights reserved  
%=========================================================================
% Author:                Eamonn M. Kenny
% Principal Researcher:  Dr. Peter J. Cullen
% First Modification:    July 2002
% Last Modification:     Dated Below "Added dd/mm/yy"
% Previous Projects:     none
% Current  Project:      IP2000 STIWRO
% Software Tool Version: 1.0
%=========================================================================
function y = gauss2( E, r, a, stepsize )

  %gaussian weights
  t2 = 1.0/sqrt(3.0);
  t1 = -t2;

  zeta = a;
  Wn   = 0.0;

  % integrate over zeta to obtain the PDF
  for k = 1:100000 
    a1 = zeta;
    b1 = zeta+stepsize;

    c  = ( b1 + a1 ) / 2.0;
    m  = ( b1 - a1 ) / 2.0;

    Jproduct = 1.0;

    zeta1 = c + m * t1;

    % calculate the product of Jo(zeta * E )
    for k = 1:size(E,2)
      Jproduct = Jproduct * besselj( 0, zeta1 * E(k) );
    end;
    
    integ = zeta1 * besselj( 0, zeta1 * r ) * Jproduct * m;
    Wn = Wn + integ;

    Jproduct = 1.0;
    zeta2 = c + m * t2;

    % calculate the product of Jo(zeta * E )
    for k = 1:size(E,2)
      Jproduct = Jproduct * besselj( 0, zeta2 * E(k) );
    end;
    
    integ = zeta2 * besselj( 0, zeta2 * r ) * Jproduct * m;
    Wn = Wn + integ;

    % if the integral has converged for increasing step then break
    if ( abs(integ)/Wn < 1.0e-8 )
      break;
    end;

    zeta = zeta + stepsize;
  end;
y = Wn;

