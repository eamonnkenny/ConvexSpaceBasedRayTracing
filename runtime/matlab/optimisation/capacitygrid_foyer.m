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
fout = fopen('capacity-foyer.txt','w');

% print size of matrix to file
xStart = 0.75;
yStart = 0.75;
zStart = 2.5;
xStep  = 2.95;
yStep  = 1.95;
M      = 11;
N      = 11;
%xStep  = 5.9;
%yStep  = 3.9;
%M      = 6;
%N      = 6;

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
    if ( sqrt( (x(m) - 5.5)^2 + (y(n) - 6)^2 ) <= 4.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 15.5)^2 + (y(n) - 14)^2 ) <= 4.0 )
      capacity(n,m) = 0.128;
    elseif ( sqrt( (x(m) - 25.5)^2 + (y(n) - 6)^2 ) <= 4.0 )
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
cap=contourf(x,y,capacity);
%cap=contour(x,y,capacity) %, 'FaceColor','interp','EdgeColor','none');
axis tight;
axis equal;
colormap('autumn');
title('Open Foyer - User Capacity Requirements' );
story3d( '../coverage/thesis/story1-foyer.txt', '../coverage/thesis/story1-foyer.txta' );
%hold;
