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

s = load( 'Pray.txt' );
for k = 1:size(s,1)
  E(k) = abs( s(k,1) + j * s(k,2) );
  %E(k) = ( s(k,1) + j * s(k,2) );
end;

size2     = 10000;
stepsize1 = 1.0;
stepsize2 = 0.005;

cdf2 = zeros( size2 / stepsize2 + 1, 1 );

cdf    = 0.0;
integ2 = 1.0;
r      = stepsize2;

% integrate over r to obtain the CDF
for l = 1:size2
  Wn = gauss4( E, r, 0, stepsize1 );

  rPDF = r * r * Wn;
  integ2 = rPDF * stepsize2;
  cdf  = cdf + integ2;

  fprintf( 1, 'r: %f, cdf: %15.12f\n', r, cdf );
  cdf2(l) = cdf;
  r = r + stepsize2;

  if ( abs(integ2/cdf) < 1.0e-4 )
    break;
  end;

end;
