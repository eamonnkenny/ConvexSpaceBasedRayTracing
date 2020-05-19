%=========================================================================
% COPYRIGHT (c) 2000-2003 by Trinity College Dublin, Dublin 2, IRELAND
% All rights reserved  
%=========================================================================
% Author:                Eamonn M. Kenny
% Principal Researcher:  Dr. Peter J. Cullen
% First Modification:    May 2002
% Last Modification:     Dated Below "Added dd/mm/yy"
% Previous Projects:     Storms, FMW/ARG
% Current  Project:      IP2000 STIWRO
% Software Tool Version: 1.0
%=========================================================================

function y = cellcentres( fileName )

fid   = fopen( fileName );
NumberOfCells = fscanf( fid, '%lf\n', 1 );

cellsSW = zeros( NumberOfCells, 3 );
cellsNE = zeros( NumberOfCells, 3 );
centres = zeros( NumberOfCells, 3 );
rect    = zeros( 5, 3 );

for n = 1:NumberOfCells
  for m = 1:3
    cellsSW(n,m) = fscanf( fid, '%lf\n', 1 );
  end;

  for m = 1:3
    cellsNE(n,m) = fscanf( fid, '%lf\n', 1 );
  end;

  for m = 1:3
    centres(n,m) = fscanf( fid, '%lf\n', 1 );
  end;

  rect(1,:) = cellsSW(n,:);
  rect(2,:) = [ cellsNE(n,1), cellsSW(n,2), cellsSW(n,3) ];
  rect(3,:) = [ cellsNE(n,1), cellsNE(n,2), cellsSW(n,3) ];
  rect(4,:) = [ cellsSW(n,1), cellsNE(n,2), cellsSW(n,3) ];
  rect(5,:) = cellsSW(n,:);

  plot3( rect(1:4,1),  rect(1:4,2), rect(1:4,3), 'b-' );
  plot3( centres(n,1), centres(n,2), centres(n,3), 'mo' );
end;

fclose( fid );
