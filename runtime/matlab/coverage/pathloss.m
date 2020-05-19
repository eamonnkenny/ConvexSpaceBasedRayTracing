%dos( 'Release\multi.exe building bts900.dat' );
system( '../Release/multi.exe foyer bts900.dat ../directory.xml' );

hold
story3d( 'story1.txt', 'story1.txta' );

% use integer to give subplots of size mxn with position p
coverage( 'pathloss.txt', 1, 1, 1 );

hold
