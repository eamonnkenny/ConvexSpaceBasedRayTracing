figure(1)

% plot path losses
%coverage( 'pathloss.txt0', 2, 4, 1 );
%coverage( 'pathloss.txt1', 2, 4, 2 );
%coverage( 'pathloss.txt2', 2, 4, 3 );
%coverage( 'pathloss.txt3', 3, 3, 4 );
%coverage( 'pathloss.txt4', 3, 3, 5 );
%coverage( 'pathloss.txt5', 3, 3, 6 );

% plot building
subplot( 2, 4, 4 );
story3d( 'story1.txt','story1.txta' );
title( 'Foyer Geometry' );
axis tight;

%function field = coverage( fileName, m1, n1, p1 )

fid    = fopen( 'bestserver.txt' );
xstart = fscanf( fid, '%lf\n', 1 );
ystart = fscanf( fid, '%lf\n', 1 );
xstep  = fscanf( fid, '%lf\n', 1 );
ystep  = fscanf( fid, '%lf\n', 1 );
M      = fscanf( fid, '%lf\n', 1 );
N      = fscanf( fid, '%lf\n', 1 );

server = zeros( N, M );

for n = 1:N
  for m = 1:M
    server(n,m) = fscanf( fid, '%lf\n', 1 );
  end;
end;
fclose( fid );

% plot best server positions
subplot( 2, 4, 5 );
surf( server, 'Facecolor', 'interp', 'Edgecolor', 'none' );

title('Best Servers')
xlabel('x-axis/m')
ylabel('y-axis/m')
view( 0, 90 );
axis tight;
colormap('hot');

subplot( 2, 4, 6 );
hold
cellsize( 'capacity.txt' );
hold

fid    = fopen( 'bestserver2.txt' );
xstart = fscanf( fid, '%lf\n', 1 );
ystart = fscanf( fid, '%lf\n', 1 );
xstep  = fscanf( fid, '%lf\n', 1 );
ystep  = fscanf( fid, '%lf\n', 1 );
M      = fscanf( fid, '%lf\n', 1 );
N      = fscanf( fid, '%lf\n', 1 );

server = zeros( N, M );

for n = 1:N
  for m = 1:M
    server(n,m) = fscanf( fid, '%lf\n', 1 );
    server(n,m) = server(n,m)/3.0;
  end;
end;
fclose( fid );

y = zeros( N,1 );
for n = 1:N
  y(n) = ystart + (n-1) * ystep;
end;

x = zeros( M,1 );
for m = 1:M
  x(m) = xstart + (m-1) * xstep;
end;

% plot best server positions
subplot( 2, 4, 8 );
hold;
surf( x, y, server, 'Facecolor', 'interp', 'Edgecolor', 'none' );
cellcentres( 'newcells.txt' );
title('Servers meeting target SIR')
xlabel('x-axis/m')
ylabel('y-axis/m')
view( 0, 90 );
axis tight;
colormap('hot');
hold;

