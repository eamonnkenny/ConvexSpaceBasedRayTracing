clear all
hold

fid = fopen( 'story1.txt' );
N = fscanf( fid, '%lf\n', 1 );
story = fscanf( fid, '%lf\n', [3, N*8] );
fclose( fid );

for m=1:N
  index = (1:8) + (m-1)*8;
  plot( story(1,index), story(2,index), 'k-', 'EraseMode','none' );
end;

M0=fullref(5,0)

title('1st order reflection paths including circles denoting transmission points');

hold
