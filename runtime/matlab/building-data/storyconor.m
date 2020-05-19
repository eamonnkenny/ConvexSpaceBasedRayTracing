function y=storyconor(storyfile,storyfilea)
% read in the story from the file
fid = fopen( storyfile, 'r' );
N = fscanf( fid, '%lf\n', 1 );
story = fscanf( fid, '%lf\n', [3, N*8] );
fclose( fid );
%for k=1:N*8
  %story(3,k) = story(3,k) * 30 - 130;
%end;

faceindex = [1 4 3 2; 1 4 6 5; 4 3 7 6; 3 2 8 7; 2 1 5 8; 5 6 7 8];
N
for m=1:N
  index = (1:8) + (m-1)*8;
  x = story(:,index)';
  patch('Vertices',x,'Faces',faceindex,'FaceColor',[0.7 0.7 0.7] );
end;
alpha(0.5)

