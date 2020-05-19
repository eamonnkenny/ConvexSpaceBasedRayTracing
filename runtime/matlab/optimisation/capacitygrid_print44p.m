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
fout = fopen('capacity-print44p.txt','w');

% print size of matrix to file
xStart = 0.25;
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
    if ( sqrt( (x(m) - 1.0)^2 + (y(n) - 4)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 2.0)^2 + (y(n) - 4)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 1.0)^2 + (y(n) - 1)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 2.0)^2 + (y(n) - 1)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 5.0)^2 + (y(n) - 2)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 5.0)^2 + (y(n) - 6)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 9.0)^2 + (y(n) - 2)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 9.0)^2 + (y(n) - 6)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 13.5)^2 + (y(n) - 2)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 13.5)^2 + (y(n) - 6)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 17.5)^2 + (y(n) - 2)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 17.5)^2 + (y(n) - 6)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 1.0)^2 + (y(n) - 2)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 2.0)^2 + (y(n) - 2)^2 ) <= 1.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 1.0)^2 + (y(n) - 5)^2 ) <= 1.0 )
      capacity(n,m) = 0.0122;
    elseif ( sqrt( (x(m) - 2.0)^2 + (y(n) - 5)^2 ) <= 1.0 )
      capacity(n,m) = 0.0122;
    elseif ( sqrt( (x(m) - 5.5)^2 + (y(n) - 1)^2 ) <= 1.0 )
      capacity(n,m) = 0.0122;
    elseif ( sqrt( (x(m) - 5.5)^2 + (y(n) - 7)^2 ) <= 1.0 )
      capacity(n,m) = 0.0122;
    elseif ( sqrt( (x(m) - 10.0)^2 + (y(n) - 1)^2 ) <= 1.0 )
      capacity(n,m) = 0.0122;
    elseif ( sqrt( (x(m) - 10.0)^2 + (y(n) - 7)^2 ) <= 1.0 )
      capacity(n,m) = 0.0122;
    elseif ( sqrt( (x(m) - 14.0)^2 + (y(n) - 1)^2 ) <= 1.0 )
      capacity(n,m) = 0.0122;
    elseif ( sqrt( (x(m) - 14.0)^2 + (y(n) - 7)^2 ) <= 1.0 )
      capacity(n,m) = 0.0122;
    elseif ( sqrt( (x(m) - 18.0)^2 + (y(n) - 1)^2 ) <= 1.0 )
      capacity(n,m) = 0.0122;
    elseif ( sqrt( (x(m) - 18.0)^2 + (y(n) - 7)^2 ) <= 1.0 )
      capacity(n,m) = 0.0122;
    else 
      capacity(n,m) = 0.0;
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
fprintf( 'Total Capacity of Printing House: %f\n', capacitySum );

% plot the capacity in the grid and the story in which it is defined
hold;
%surf(x,y,capacity, 'FaceColor','interp');
%surf(x,y,capacity, 'FaceColor','interp','EdgeColor','none');
surf(x,y,capacity)
%, 'FaceColor','nointerp','EdgeColor','none');
title('Printing House - User Capacity Requirements' );
axis tight;
axis equal;
colormap('autumn');
story3d( '../building-data/story1-print44p.txt', '../building-data/story1-print44p.txta' );
hold;
