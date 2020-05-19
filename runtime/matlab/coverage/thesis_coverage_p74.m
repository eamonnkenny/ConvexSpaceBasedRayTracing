%dos( 'Release\multi.exe building bts900.dat' );
%system( '../Release/multi.exe foyer bts900.dat ../directory.xml' );

%story3d( 'thesis/test4-story2.txt', 'thesis/test4-story2.txta' );

% use integer to give subplots of size mxn with position p
figure
gopathloss_p74('thesis/test2-gopathloss-p74.txt',2,1,1)
coverage( 'thesis/test2-pathloss-p74.txt', 2, 1, 2 );
