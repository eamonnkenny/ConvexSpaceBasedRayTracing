% read in the rays from the file
function y=diffraction2(kk,fileName)

fid   = fopen( fileName );
Nrays = fscanf( fid, '%lf\n', 1 );

Norder = 0;
for n = 1:Nrays
  Norder = fscanf( fid, '%d\n', 1 );
  z      = zeros( 3, Norder );

  for m = 1:Norder
    z(1,m) = fscanf( fid, '%lf\n', 1 );
    z(2,m) = fscanf( fid, '%lf\n', 1 );
    z(3,m) = fscanf( fid, '%lf\n', 1 );
  end;

  if Norder == 3 
      plot3( z(1,1:Norder), z(2,1:Norder), z(3,1:Norder),'k-','EraseMode','none' );
  elseif Norder == 4 
      plot3( z(1,1:Norder), z(2,1:Norder), z(3,1:Norder),'m-','EraseMode','none' );
  elseif Norder == 5
      plot3( z(1,1:Norder), z(2,1:Norder), z(3,1:Norder),'r-','EraseMode','none' );
  elseif Norder == 6
      plot3( z(1,1:Norder), z(2,1:Norder), z(3,1:Norder),'b-','EraseMode','none' );
  elseif Norder == 7
      plot3( z(1,1:Norder), z(2,1:Norder), z(3,1:Norder),'g-','EraseMode','none' );
  elseif Norder == 8
      plot3( z(1,1:Norder), z(2,1:Norder), z(3,1:Norder),'y-','EraseMode','none' );
  else
      plot3( z(1,1:Norder), z(2,1:Norder), z(3,1:Norder),'c-','EraseMode','none' );
  end;
end;

fclose( fid );

