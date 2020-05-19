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
fout = fopen('capacity.txt','w');

% print size of matrix to file
xStart = 0.75;
yStart = 0.75;
zStart = 2.5;
xStep  = 2.95;
yStep  = 1.95;
M      = 11;
N      = 11;

fprintf( fout, 'StartPoint: %f %f %f \n', xStart, yStart, zStart );
fprintf( fout, 'StepSize: %f %f\n', xStep, yStep );
fprintf( fout, 'Dimensions: %d %d\n', M, N );

% set capacity requirements
capacity = zeros(N,M);
x = zeros(M,1);
y = zeros(N,1);

capacitySum = 0.0;

for n = 1:N
  y(n) = yStart + (n-1) * yStep;
end;

for m = 1:M
  x(m) = xStart + (m-1) * xStep;
  for n = 1:N
    if ( sqrt( (x(m) - 5.5)^2 + (y(n) - 6)^2 ) <= 4.0 )
      capacity(n,m) = 0.075;
    elseif ( sqrt( (x(m) - 15.5)^2 + (y(n) - 14)^2 ) <= 4.0 )
      capacity(n,m) = 0.075;
    elseif ( sqrt( (x(m) - 25.5)^2 + (y(n) - 6)^2 ) <= 4.0 )
      capacity(n,m) = 0.075;
    else 
      capacity(n,m) = 0.075;
    end;
      
    capacitySum = capacitySum + capacity(n,m);
    fprintf( fout, '%f\n', capacity(n,m) );
  end;
end;

fclose(fout);

fprintf( 'Start: (%fx%f), End: (%fx%f)\n', xStart, yStart, xStart+(M-1)*xStep, yStart+(N-1)*yStep );
fprintf( 'Total Capacity of Foyer: %f\n', capacitySum );

% plot the capacity in the grid and the story in which it is defined
hold;
surf(x,y,capacity, 'FaceColor','interp','EdgeColor','none');
story3d( 'story1.txt', 'story1.txta' );
hold;
