fp = fopen( 'rayinfo.txt' );
str = fscanf( fp, '%s', 1 );
x = zeros( 2, 1 );
y = zeros( 2, 1 );
z = zeros( 2, 1 );

p = fscanf( fp, '%f', 1 );
x(1) = p;
p = fscanf( fp, '%f', 1 );
y(1) = p;
p = fscanf( fp, '%f', 1 );
z(1) = p;
x(2) = 0.0;
y(2) = 0.0;
z(2) = 0.0;

for m = 1:11
 str = fscanf( fp, '%s', 1 );
end;

points= fscanf( fp, '%f', [3, 8] );

fclose( fp );

faceindex = [1 4 3 2; 1 4 6 5; 4 3 7 6; 3 2 8 7; 2 1 5 8; 5 6 7 8];
w = points(:,1:8)';

axis( [-10 10 -10 10 0 18] )
patch('Vertices',w,'Faces',faceindex,'FaceColor',[0.7 0.7 0.7] );
hold;

plot3( x, y, z );
title( 'geometry of problem' );
xlabel( 'x/metres' );
ylabel( 'y/metres' );
hold;
