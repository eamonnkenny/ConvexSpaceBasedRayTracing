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

s = load( 'Praya.txt' );

for k = 1:size(s,1)
  E(k) = abs( s(k,1) + j * s(k,2) );
end;

size2     = 10000;
stepsize2 = 0.005;

cdf    = 0.0;
r      = 0.0001;

% integrate over r to obtain the CDF
for l = 1:size2
  Wn = trap( E, r, 0, 20 );

  integ2 = Wn * stepsize2;
  cdf  = cdf + integ2;

  fprintf( 1, 'Wn: %f, r: %f, cdf: %15.12f\n', Wn, r, cdf );
  r = r + stepsize2;

  if ( abs(integ2 / cdf ) < 1.0e-3 )
    break;
  end;
end;
