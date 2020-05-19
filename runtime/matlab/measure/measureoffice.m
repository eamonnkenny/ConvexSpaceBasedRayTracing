% read in the story from the file
lambda = 300.0/1000.0;
C = (lambda / 8.0 / pi)/ sqrt(2.0);%/ sqrt( 120.0*pi );
fid = fopen( '../../measurements/measureoffice.dat' );
N = fscanf( fid, '%lf\n', 1 );
t = fscanf( fid, '%lf\n', [4, N] );
fclose( fid );

% check the values carefully on Monday
s1 = load( 'corridoropendoor1-r3t4.txt' );
s4 = load( 'corridoropendoor1-r4t5.txt' );
s5 = load( 'corridoropendoor2-r3t4.txt' );

figure(1);
hold;
plot( s1(:,1), 20.0*log10(s1(:,2)*C), 'r:o', ...
      s4(:,1), 20.0*log10(s4(:,2)*C), 'g:*', ...
      t(1,:), 18.33-t(4,:), 'b:s');
      %s5(:,1), 20.0*log10(s5(:,2)*C), 'g-', ...
xlabel('R_x - Receiver Locations/units');
ylabel('Signal Strength/dB');
legend('RT(3,4) with Open Doors \epsilon=2.32 \sigma=1.45e-3',...
       'RT(4,5) with Open Doors \epsilon=5.0 \sigma=1.45e+3',...
       'Measurement Results (Open Doors)' );
       %'RT(3,4) with Open Doors \epsilon=5.0 \sigma=1.45e+3',...
      set(1,'color','white');
      title('Measurement Set 1 - Open Doors' ); 
      axis tight;
hold;

s2 = load( 'corridorclosed1-r3t4.txt' );
s3 = load( 'corridornodoors-r3t4.txt' );

figure(2);
hold;
plot( s2(:,1), 20.0*log10(s2(:,2) * C), 'r:o', s3(:,1), 20.0*log10(s3(:,2)*C) , 'g:*', t(1,:), 18.33-t(4,:), 'b:s')
xlabel('R_x - Receiver Locations/units');
ylabel('Signal Strength/dB');
legend('RT with Closed Doors \epsilon=5.0 \sigma=1.45e-3', 'RT with No Doors', 'Measurement Results (Open Doors)' );
title('Measurement Set 1 - Closed Door/No Doors' ); 
axis tight;
hold;
set(2,'color','white');

meant   = mean( 18.33-t(4,:) );
means5  = mean( 20*log10(s5(:,2) * C) );

means2  = mean( 20*log10(s2(:,2) * C) );
means3  = mean( 20*log10(s3(:,2) * C) );

means   = mean( 20*log10(s1(:,2) * C) );

fprintf( 'mean diff5=%f diff1=%f\n', meant - means5, meant - means );
fprintf( 'mean diff2=%f diff3=%f\n', meant - means2, meant - means3 );


%legend('ray-trace', 'ray-trace closed door', 'measurements open', 'measurements closed');
%legend('RT with Open Doors', 'RT with No Doors', 'Measurement Results (Open Doors)' );
axis tight;
