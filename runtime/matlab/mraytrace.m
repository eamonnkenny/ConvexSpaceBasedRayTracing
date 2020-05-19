dos( 'release\multi.exe twofloor' );
hold
%story3d( 'story1.txt', 'story1.txta' );
elongated3d;
%floor3d;
fid   = fopen( 'pathloss.txt0' );
xstart = fscanf( fid, '%lf\n', 1 );
ystart = fscanf( fid, '%lf\n', 1 );
xstep = fscanf( fid, '%lf\n', 1 );
ystep = fscanf( fid, '%lf\n', 1 );
M = fscanf( fid, '%lf\n', 1 );
N = fscanf( fid, '%lf\n', 1 );

x     = zeros( N, M );
y     = zeros( N, M );
z     = zeros( N, M );
u     = zeros( N, M );
v     = zeros( N, M );
w     = zeros( N, M );

k = 0;
for n = 1:N
  for m = 1:M
    k = k + 1;
    x(n,m) = fscanf( fid, '%lf\n', 1 );
    y(n,m) = fscanf( fid, '%lf\n', 1 );
    z(n,m) = fscanf( fid, '%lf\n', 1 );
    u(n,m) = fscanf( fid, '%lf\n', 1 );
    v(n,m) = fscanf( fid, '%lf\n', 1 );
    w(n,m) = fscanf( fid, '%lf\n', 1 );
  end;
end;
fclose( fid );

X = (1:M)*xstep + xstart - xstep;
Y = (1:N)*ystep + ystart - xstep;
Z = 5.0;
field = 20.0*log10(sqrt( u.^2 + v.^2 + w.^2 ));
%[cx,cy,cz] = meshgrid( X, Y, Z );

surf( X,Y,field, 'Facecolor', 'interp', 'Edgecolor', 'none' );

title('Field Strength on x-y grid at 0.5 metre resolution (z=5m). Tx=(5,5,5)')
xlabel('x-axis/m')
ylabel('y-axis/m')
view( -10, 80 );
axis tight;
colormap('hot');

hold
