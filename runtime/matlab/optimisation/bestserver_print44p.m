% plot path losses
%function field = coverage( fileName, m1, n1, p1 )
hold

fid    = fopen( '../coverage/bestserver-print44p-r2.txt' );
xstart = fscanf( fid, '%lf\n', 1 );
ystart = fscanf( fid, '%lf\n', 1 );
xstep  = fscanf( fid, '%lf\n', 1 );
ystep  = fscanf( fid, '%lf\n', 1 );
M      = fscanf( fid, '%d\n', 1 );
N      = fscanf( fid, '%d\n', 1 );

server = zeros( N, M );

for n = 1:N
  for m = 1:M
    server(n,m) = fscanf( fid, '%lf\n', 1 );
  end;
end;
fclose( fid );

fout = fopen('capacity-print44p.txt','r');

% print size of matrix to file
dummy = fscanf( fout, '%s', 1 );
xStart = fscanf( fout, '%f', 1 );
yStart = fscanf( fout, '%f', 1 );
zStart = fscanf( fout, '%f', 1 );
dummy = fscanf( fout, '%s', 1 );
xStep = fscanf( fout, '%f', 1 );
yStep = fscanf( fout, '%f', 1 );
dummy = fscanf( fout, '%s', 1 );
M = fscanf( fout, '%f', 1 );
N = fscanf( fout, '%f', 1 );

fprintf( 1, 'StartPoint: %f %f %f \n', xStart, yStart, zStart );
fprintf( 1, 'StepSize: %f %f\n', xStep, yStep );
fprintf( 1, 'Dimensions: %d %d\n', M, N );

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
    a = fscanf( fout, '%f', 1 );
    capacity(n,m) = a;
    capacitySum = capacitySum + capacity(n,m);
  end;
end;

fclose(fout);

fprintf( 'Start: (%fx%f), End: (%fx%f)\n', xStart, yStart, xStart+(M-1)*xStep, yStart+(N-1)*yStep );
fprintf( 'Total Capacity of Printing House: %f\n', capacitySum );

% plot the capacity in the grid and the story in which it is defined
%surf(x,y,capacity, 'FaceColor','interp','EdgeColor','none');
% plot best server positions
colormap('hot')
surf(x,y,server)
% 'Facecolor', 'nointerp', 'Edgecolor', 'none' );
%story3d( '../building-data/story1.txt', '../building-data/story1.txta' );
cellcentres('../cells-print44p-start.txt');
endlocations('../../linux/locations-print44p.txt');
%view( -10, 85 );
axis tight;
title( 'centres of capacity, boxes, and best servers' )
hold
