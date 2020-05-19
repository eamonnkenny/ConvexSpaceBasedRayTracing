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
function y = trap2( E, a, stepsize )

  Wn     = 0.0;
  integ1 = 0.0;
  a1     = a+0.0001;
  % integrate over zeta to obtain the PDF

  totalIntegral = 0.0;
  for n = 1:10000
    b1 = a1 + stepsize;
    m1 = ( a1 + b1 ) / 2.0;

    stepsize2 = stepsize / 2.0; 
    N = 1;

    % define simple trapezoidal rule
    integ2 =          trap( E, a1, 0, 10);
    integ2 = integ2 + trap( E, m1, 0, 10 );
    integ2 = integ2 + trap( E, b1, 0, 10 );
  
  
    while ( abs( ( integ1 - integ2 ) / integ2 ) > 1.0e-3 )
      integ1    = integ2;
      N         = N * 2;
      stepsize2 = stepsize2 / 2.0;
      startStep = a1 - stepsize2;
      integ2    = integ1 / 2.0;
      for m = 1:N
        zeta = startStep + m * stepsize2 * 2.0;
        integ2 = integ2 + trap( E, zeta, 0, 10 );
      end;
      fprintf(1, 'integ1: %12.10f integ2: %12.10f\n', integ1, integ2 );
  
    end;
    totalIntegral = totalIntegral + integ2;
    fprintf(1, 'mean Integral: %12.10f\n', totalIntegral );
    if ( abs( integ2 / totalIntegral ) < 1.0e-4 )
      break;
    end;
    a1 = b1;
  end;
  
  %fprintf(1, 'integral %5.4f  ', b1 );
  y = totalIntegral;
