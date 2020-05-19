figure(1)

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
%    server(n,m) = server(n,m)/3.0;
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
hold;
surf( x, y, server, 'Facecolor', 'flat', 'Edgecolor', 'none' );
%story3d( 'story1.txt', 'story1.txta' );
cellcentres( 'newcells.txt' );
title('Servers meeting target SIR')
xlabel('x-axis/m')
ylabel('y-axis/m')
%view( 0, 90 );
colorbar;
axis tight;
colormap('hot');
hold;

