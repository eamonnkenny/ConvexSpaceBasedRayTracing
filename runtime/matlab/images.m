% read in the images from the file

fid = fopen( 'images.txt' );
Tx = fscanf( fid, '%lf\n', 3 );
Nimages = fscanf( fid, '%lf\n', 1 );
image = fscanf( fid, '%lf\n', [3, Nimages] ); % array size Nimages x 3
fclose( fid );

plot( image(1,:), image(2,:), 'go' );
