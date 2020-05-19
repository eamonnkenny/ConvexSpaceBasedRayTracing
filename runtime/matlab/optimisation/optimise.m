fprintf( 1, '\n\nPress a key to see the capacity grid' );
pause
figure(1);
capacitygrid

fprintf( 1, '\n\nPress a key to perform optimisation' );
pause
system('../../Release/lp.exe');

fprintf( 1, '\n\nPress a key to show bestserver calculations and other stats' );
pause
figure(2);
bestserver

fprintf( 1, '\n\n Press a key to show bestserver calculation' );
pause
figure(3);
bestserver2

fprintf( 1, 'End of Demonstration' );
