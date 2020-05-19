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
function integ = WnFunc( E, r, zeta, stepsize )

Jproduct = 1.0;

% calculate the product of Jo(zeta * E )
for k = 1:size(E,2)
  Jproduct = Jproduct * besselj( 0, zeta * E(k) );
end;
  
integ = r * r * zeta * besselj( 0, zeta * r ) * Jproduct * stepsize;
