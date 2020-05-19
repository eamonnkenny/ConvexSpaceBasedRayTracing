function field = gopathloss_p74( fileName, m1, n1, p1 )
%story3d( 'story1.txt', 'story1.txta' );
%floor3d;
fid   = fopen( fileName, 'r' );
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
    a1 = fscanf( fid, '%lf\n', 1 );
    b1 = fscanf( fid, '%lf\n', 1 );
    c1 = fscanf( fid, '%lf\n', 1 );

    a1 = fscanf( fid, '%lf\n', 1 );
    b1 = fscanf( fid, '%lf\n', 1 );
    a1 = fscanf( fid, '%lf\n', 1 );
    b1 = fscanf( fid, '%lf\n', 1 );
    a1 = fscanf( fid, '%lf\n', 1 );
    b1 = fscanf( fid, '%lf\n', 1 );

    a1 = fscanf( fid, '%lf\n', 1 );
    b1 = fscanf( fid, '%lf\n', 1 );
    a1 = fscanf( fid, '%lf\n', 1 );
    b1 = fscanf( fid, '%lf\n', 1 );
    a1 = fscanf( fid, '%lf\n', 1 );
    b1 = fscanf( fid, '%lf\n', 1 );

    a1 = fscanf( fid, '%lf\n', 1 );
    b1 = fscanf( fid, '%lf\n', 1 );
    u(n,m) = a1 + j*b1;
    a1 = fscanf( fid, '%lf\n', 1 );
    b1 = fscanf( fid, '%lf\n', 1 );
    v(n,m) = a1 + j*b1;
    a1 = fscanf( fid, '%lf\n', 1 );
    b1 = fscanf( fid, '%lf\n', 1 );
    w(n,m) = a1 + j*b1;
  end;
end;
fclose( fid );

X = (1:M)*xstep + xstart - xstep;
Y = (1:N)*ystep + ystart - xstep;
Z = 5.0;
fieldgo = 20.0*log10(sqrt( abs(u).^2 + abs(v).^2 + abs(w).^2 ));
%[cx,cy,cz] = meshgrid( X, Y, Z );

subplot( m1, n1, p1 )
surf( X,Y,fieldgo, 'Facecolor', 'interp', 'Edgecolor', 'none' );

title('Geometrical Optics solution. Tx=(0,0,5) over Flat Plate')
xlabel('Location/m')
ylabel('Location/m')
view([-10 80] );
%axis([0 31 0 21 -150 50] );
axis tight;
colormap('hot');
