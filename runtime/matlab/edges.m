% read in the images from the file

fid = fopen( 'edges.txt' );
Nedges = fscanf( fid, '%lf\n', 1 );
edges = fscanf( fid, '%lf\n', [3, Nedges*2] ); % array size Nimages x 2 x 3
fclose( fid );

for m = 1:Nedges
  index = (m*2-1:m*2);
  plot( edges(1,index), edges(2,index), 'co' );
  plot( edges(1,index), edges(2,index), 'r-' );
end;
