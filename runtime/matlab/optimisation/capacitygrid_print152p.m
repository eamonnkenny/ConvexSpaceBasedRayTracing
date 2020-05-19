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
fout = fopen('capacity-print152p.txt','w');

% print size of matrix to file
xStart = 0.60;
yStart = 1.00;
zStart = 2.4;
xStep  = 1.0;
yStep  = 1.0;
M      = 19;
N      = 8;

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

for n = 1:N
  for m = 1:M
    x(m) = xStart + (m-1) * xStep;
    if ( sqrt( (x(m) - 5.5)^2 + (y(n) - 2)^2 ) <= 1.5 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 5.5)^2 + (y(n) - 6)^2 ) <= 1.5 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 10.0)^2 + (y(n) - 2)^2 ) <= 1.5 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 10.0)^2 + (y(n) - 6)^2 ) <= 1.5 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 14.0)^2 + (y(n) - 2)^2 ) <= 1.5 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 14.0)^2 + (y(n) - 6)^2 ) <= 1.5 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 18.0)^2 + (y(n) - 2)^2 ) <= 1.5 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 18.0)^2 + (y(n) - 6)^2 ) <= 1.5 )
      capacity(n,m) = 0.128;
    else 
      capacity(n,m) = 0.0122;
    end;
    if ( x(m) == 2.75 & y(n) == 6.25 )
      sqrt( (x(m) - 5.5)^2 + (y(n) - 6)^2 )
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
%surf(x,y,capacity, 'FaceColor','interp');
surf(x,y,capacity)
%, 'FaceColor','interp','EdgeColor','none');
title('Printing House - User Capacity Requirements' );
story3d( '../building-data/story1-print152p.txt', '../building-data/story1-print152p.txta' );
axis tight;
axis equal;
colormap('autumn');
hold;
