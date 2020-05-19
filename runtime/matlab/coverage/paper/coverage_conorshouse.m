% first run runtime/linux/paper-conorshouse.sh

hold
story3d( 'story1-conorshouse.txt', 'story1-conorshouse.txta' );

% use integer to give subplots of size mxn with position p
coverage( 'pathloss.txt', 1, 1, 1 );
colorbar

hold

