function field = coverage( fileName, m1, n1, p1 )

fid   = fopen( fileName );
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
    x(n,m) = fscanf( fid, '%lf\n', 1 );
    y(n,m) = fscanf( fid, '%lf\n', 1 );
    z(n,m) = fscanf( fid, '%lf\n', 1 );
    x(n,m) = fscanf( fid, '%lf\n', 1 );
    y(n,m) = fscanf( fid, '%lf\n', 1 );
    z(n,m) = fscanf( fid, '%lf\n', 1 );
    x(n,m) = fscanf( fid, '%lf\n', 1 );
    y(n,m) = fscanf( fid, '%lf\n', 1 );
    z(n,m) = fscanf( fid, '%lf\n', 1 );
    x(n,m) = fscanf( fid, '%lf\n', 1 );
    y(n,m) = fscanf( fid, '%lf\n', 1 );
    z(n,m) = fscanf( fid, '%lf\n', 1 );
    a1     = fscanf( fid, '%lf\n', 1 );
    b1     = fscanf( fid, '%lf\n', 1 );
    a2     = fscanf( fid, '%lf\n', 1 );
    b2     = fscanf( fid, '%lf\n', 1 );
    a3     = fscanf( fid, '%lf\n', 1 );
    b3     = fscanf( fid, '%lf\n', 1 );
    u(n,m) = a1 + j * b1;
    v(n,m) = a2 + j * b2;
    w(n,m) = a3 + j * b3;
  end;
end;
fclose( fid );

X = (1:M)*xstep + xstart - xstep;
Y = (1:N)*ystep + ystart - xstep;
Z = 5.0;
field = 20.0*log10(sqrt( abs(u).^2 + abs(v).^2 + abs(w).^2 ));
%field = 20.0*log10(sqrt( abs(w).^2 ));
%[cx,cy,cz] = meshgrid( X, Y, Z );

subplot( m1, n1, p1 );
%surf( X,Y,field );
surf( X,Y,field, 'Facecolor', 'interp', 'Edgecolor', 'none' );
%surf( X,Y,field, 'Facecolor', 'interp' );

title('Ray-trace Signal Strength/dBm')
xlabel('Location/m')
ylabel('Location/m')
view(-10, 80 );
axis tight;
colormap('hot');
%alpha(0.6);
