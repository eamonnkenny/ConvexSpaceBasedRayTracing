% read in the story from the file

fid = fopen( 'story.txt' );
N = fscanf( fid, '%lf\n', 1 );
storyRead = fscanf( fid, '%lf\n', [3, N*8] );
fclose( fid );

for m=1:N
  index = (1:8) + (m-1)*8;
  plot( storyRead(1,index), storyRead(2,index), 'k-' );
end;
