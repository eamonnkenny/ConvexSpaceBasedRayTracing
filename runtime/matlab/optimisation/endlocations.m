%=========================================================================
% COPYRIGHT (c) 2000-2003 by Trinity College Dublin, Dublin 2, IRELAND
% All rights reserved  
%=========================================================================
% Author:                Eamonn M. Kenny
% Principal Researcher:  
% First Modification:    Sep 2015
% Last Modification:     Dated Below "Added dd/mm/yy"
% Previous Projects:     Storms, FMW/ARG, IP2000 STIWRO
% Current  Project:      
% Software Tool Version: 1.2
%=========================================================================

function y = endlocations( fileName )

fid   = fopen( fileName );
NumberOfCells = fscanf( fid, '%lf\n', 1 );

centres = zeros( NumberOfCells, 3 );

for n = 1:NumberOfCells
  for m = 1:3
    centres(n,m) = fscanf( fid, '%lf\n', 1 );
  end;

  plot3( centres(n,1), centres(n,2), centres(n,3), 'go' );
end;

fclose( fid );
