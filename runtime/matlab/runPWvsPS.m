s = load( 'meanInfinitePS.txt' );
figure(1);
polar( s(:,1), s(:,2), 'r-' );
hold;
polar( s(:,1), s(:,3), 'g.-' );
polar( s(:,1), s(:,6), 'm:' );
polar( s(:,1), s(:,8), 'k.' );
hold
legend( 'mean Infinite Reflection', 'mean Single Reflection', 'mean Burnside Infinite','mean clemmow exact');
title('r = 10, source = (-r cos\theta,0,-r sin\theta), receiver(r,0,r)');

figure(2);
polar( s(:,1), s(:,4), 'b-.' );
hold
polar( s(:,1), s(:,5), 'k:' );
%polar( s(:,1), s(:,7), 'm.' );
polar( s(:,1), s(:,9), 'r-' );
%polar( s(:,1), s(:,10), 'g.-' );
hold;
%legend( 'Incident Field', 'single Reflection', 'infinite Reflection', 'clemmow' , 'burnside');
legend( 'Incident Field', 'single Reflection', 'clemmow');
title('r = 10, source = (-r cos\theta,0,-r sin\theta), receiver(r,0,r)');
