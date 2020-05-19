
cd %dos( 'Release\multi.exe building bts900.dat' );
%system( '../Release/multi.exe foyer bts900.dat ../directory.xml' );

hold
story3d( 'test4-story2.txt', 'test4-story2.txta' );

% use integer to give subplots of size mxn with position p
coverage( 'thesis/test4-pathloss-p76.txt', 1, 1, 1 );
colorbar

hold
