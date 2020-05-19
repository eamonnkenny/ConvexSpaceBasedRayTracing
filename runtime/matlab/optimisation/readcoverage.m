function [x y grid] = readcoverage(fileName)

fid    = fopen( fileName );
xStart = fscanf( fid, '%lf\n', 1 );
yStart = fscanf( fid, '%lf\n', 1 );
xStep  = fscanf( fid, '%lf\n', 1 );
yStep  = fscanf( fid, '%lf\n', 1 );
M      = fscanf( fid, '%lf\n', 1 );
N      = fscanf( fid, '%lf\n', 1 );

grid = zeros( N, M );
x = zeros( M, 1 );
y = zeros( N, 1 );

for m = 1:M
  x(m) = xStart + (m-1) * xStep;
end;

for n = 1:N
  y(n) = yStart + (n-1) * yStep;
  for m = 1:M
    grid(n,m) = fscanf( fid, '%lf\n', 1 );
  end;
end;
fclose( fid );

