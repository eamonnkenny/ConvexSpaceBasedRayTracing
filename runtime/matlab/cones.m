% size allocation
M     = 120;
N     = 80;
L     = 3;
x     = zeros( N, M, L );
y     = zeros( N, M, L );
z     = zeros( N, M, L );
u     = zeros( N, M, L );
v     = zeros( N, M, L );
w     = zeros( N, M, L );

fileName = 'pathloss.txt0';
l        = 1;

fid   = fopen( fileName );
xstart = fscanf( fid, '%lf\n', 1 );
ystart = fscanf( fid, '%lf\n', 1 );
xstep = fscanf( fid, '%lf\n', 1 );
ystep = fscanf( fid, '%lf\n', 1 );
M = fscanf( fid, '%lf\n', 1 );
N = fscanf( fid, '%lf\n', 1 );

k = 0;
for n = 1:N
  for m = 1:M
    k = k + 1;
    x(n,m,l) = fscanf( fid, '%lf\n', 1 );
    y(n,m,l) = fscanf( fid, '%lf\n', 1 );
    z(n,m,l) = fscanf( fid, '%lf\n', 1 );
    u(n,m,l) = fscanf( fid, '%lf\n', 1 );
    v(n,m,l) = fscanf( fid, '%lf\n', 1 );
    w(n,m,l) = fscanf( fid, '%lf\n', 1 );
  end;
end;
fclose( fid );

fileName = 'pathloss.txt1';
l        = 2;

fid   = fopen( fileName );
xstart = fscanf( fid, '%lf\n', 1 );
ystart = fscanf( fid, '%lf\n', 1 );
xstep = fscanf( fid, '%lf\n', 1 );
ystep = fscanf( fid, '%lf\n', 1 );
M = fscanf( fid, '%lf\n', 1 );
N = fscanf( fid, '%lf\n', 1 );

k = 0;
for n = 1:N
  for m = 1:M
    k = k + 1;
    x(n,m,l) = fscanf( fid, '%lf\n', 1 );
    y(n,m,l) = fscanf( fid, '%lf\n', 1 );
    z(n,m,l) = fscanf( fid, '%lf\n', 1 );
    u(n,m,l) = fscanf( fid, '%lf\n', 1 );
    v(n,m,l) = fscanf( fid, '%lf\n', 1 );
    w(n,m,l) = fscanf( fid, '%lf\n', 1 );
  end;
end;
fclose( fid );

fileName = 'pathloss.txt2';
l        = 3;

fid   = fopen( fileName );
xstart = fscanf( fid, '%lf\n', 1 );
ystart = fscanf( fid, '%lf\n', 1 );
xstep = fscanf( fid, '%lf\n', 1 );
ystep = fscanf( fid, '%lf\n', 1 );
M = fscanf( fid, '%lf\n', 1 );
N = fscanf( fid, '%lf\n', 1 );

k = 0;
for n = 1:N
  for m = 1:M
    k = k + 1;
    x(n,m,l) = fscanf( fid, '%lf\n', 1 );
    y(n,m,l) = fscanf( fid, '%lf\n', 1 );
    z(n,m,l) = fscanf( fid, '%lf\n', 1 );
    u(n,m,l) = fscanf( fid, '%lf\n', 1 );
    v(n,m,l) = fscanf( fid, '%lf\n', 1 );
    w(n,m,l) = fscanf( fid, '%lf\n', 1 );
  end;
end;
fclose( fid );

fileName = 'pathloss.txt3';
l        = 4;

fid   = fopen( fileName );
xstart = fscanf( fid, '%lf\n', 1 );
ystart = fscanf( fid, '%lf\n', 1 );
xstep = fscanf( fid, '%lf\n', 1 );
ystep = fscanf( fid, '%lf\n', 1 );
M = fscanf( fid, '%lf\n', 1 );
N = fscanf( fid, '%lf\n', 1 );

k = 0;
for n = 1:N
  for m = 1:M
    k = k + 1;
    x(n,m,l) = fscanf( fid, '%lf\n', 1 );
    y(n,m,l) = fscanf( fid, '%lf\n', 1 );
    z(n,m,l) = fscanf( fid, '%lf\n', 1 );
    u(n,m,l) = fscanf( fid, '%lf\n', 1 );
    v(n,m,l) = fscanf( fid, '%lf\n', 1 );
    w(n,m,l) = fscanf( fid, '%lf\n', 1 );
  end;
end;
fclose( fid );

fileName = 'pathloss.txt4';
l        = 5;

fid   = fopen( fileName );
xstart = fscanf( fid, '%lf\n', 1 );
ystart = fscanf( fid, '%lf\n', 1 );
xstep = fscanf( fid, '%lf\n', 1 );
ystep = fscanf( fid, '%lf\n', 1 );
M = fscanf( fid, '%lf\n', 1 );
N = fscanf( fid, '%lf\n', 1 );

k = 0;
for n = 1:N
  for m = 1:M
    k = k + 1;
    x(n,m,l) = fscanf( fid, '%lf\n', 1 );
    y(n,m,l) = fscanf( fid, '%lf\n', 1 );
    z(n,m,l) = fscanf( fid, '%lf\n', 1 );
    u(n,m,l) = fscanf( fid, '%lf\n', 1 );
    v(n,m,l) = fscanf( fid, '%lf\n', 1 );
    w(n,m,l) = fscanf( fid, '%lf\n', 1 );
  end;
end;
fclose( fid );

fileName = 'pathloss.txt5';
l        = 6;

fid   = fopen( fileName );
xstart = fscanf( fid, '%lf\n', 1 );
ystart = fscanf( fid, '%lf\n', 1 );
xstep = fscanf( fid, '%lf\n', 1 );
ystep = fscanf( fid, '%lf\n', 1 );
M = fscanf( fid, '%lf\n', 1 );
N = fscanf( fid, '%lf\n', 1 );

k = 0;
for n = 1:N
  for m = 1:M
    k = k + 1;
    x(n,m,l) = fscanf( fid, '%lf\n', 1 );
    y(n,m,l) = fscanf( fid, '%lf\n', 1 );
    z(n,m,l) = fscanf( fid, '%lf\n', 1 );
    u(n,m,l) = fscanf( fid, '%lf\n', 1 );
    v(n,m,l) = fscanf( fid, '%lf\n', 1 );
    w(n,m,l) = fscanf( fid, '%lf\n', 1 );
  end;
end;
fclose( fid );

X = (1:M)*xstep + xstart - xstep;
Y = (1:N)*ystep + ystart - xstep;
Z = [4.5 4.7 4.9 5.1 5.3 5.5];

field = 20.0*log10(sqrt( u.^2 + v.^2 + w.^2 ) + 1.0e-12);

%[cx,cy,cz] = meshgrid( X, Y, Z );

%surf( X,Y,field, 'Facecolor', 'interp', 'Edgecolor', 'none' );

%title('Field Strength on x-y grid at 1metre resolution (z=5m). Tx=(5,5,5)')
%xlabel('x-axis/m')
%ylabel('y-axis/m')
