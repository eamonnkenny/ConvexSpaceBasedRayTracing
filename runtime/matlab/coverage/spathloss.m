%dos( 'Release\multi.exe building bts900.dat' );

hold
story3d( 'story1.txt', 'story1.txta' );
%aspect3d( 'story1.txt', 'story1.txta', 'story1.txtb' );
%floor3d;

% use integer to give subplots of size mxn with position p
coverage( 'pathloss.txt', 1, 1, 1 );

hold
