% read in the rays from the file
%fprintf(1,'About to run C++ code\n');
%dos('.\Release\reflectCoeff.exe');
%fprintf(1,'finished C++ code\n');
close all;
clear all;

fid   = fopen( 'singleReflect.txt' );

x     = zeros( 90, 1 );
Rpar  = zeros( 90, 1 );
Rperp = zeros( 90, 1 );
Tpar  = zeros( 90, 1 );
Tperp = zeros( 90, 1 );
Rpar2 = zeros( 90, 1 );
Rperp2= zeros( 90, 1 );

for m = 1:90
  x(m) = fscanf( fid, '%lf\n', 1 );
  Rpar(m) = fscanf( fid, '%lf\n', 1 );
  Rperp(m) = fscanf( fid, '%lf\n', 1 );
end;

subplot(2,4,1);
plot( x, Rpar, 'r-'  );
title(' \mid R_{||} \mid - single reflection');
xlabel('\theta_i/degrees');
ylabel('reflection coefficient');
axis tight;
subplot(2,4,2);
plot( x, Rperp, 'g-'  );
xlabel('\theta_i/degrees');
ylabel('reflection coefficient');
title(' \mid R_{\perp} \mid - single reflection');
axis tight;

fid   = fopen( 'singleTransmission.txt' );

for m = 1:90
  x(m) = fscanf( fid, '%lf\n', 1 );
  Tpar(m) = fscanf( fid, '%lf\n', 1 );
  Tperp(m) = fscanf( fid, '%lf\n', 1 );
end;

fclose( fid );

subplot(2,4,3);
plot( x, Tpar, 'b-' );
xlabel('\theta_i/degrees');
ylabel('transmission coefficient');
title('\mid T_{||} \mid - single transmission');
axis tight;

subplot(2,4,4);
plot( x, Tperp, 'm-' );
xlabel('\theta_i/degrees');
ylabel('transmission coefficient');
title('\mid T_{\perp} \mid - single transmission');
axis tight;

fid   = fopen( 'infiniteReflect.txt' );

for m = 1:90
  x(m) = fscanf( fid, '%lf\n', 1 );
  Rpar(m) = fscanf( fid, '%lf\n', 1 );
  Rperp(m) = fscanf( fid, '%lf\n', 1 );
end;

fclose( fid );

fid   = fopen( 'correctedReflect.txt' );

for m = 1:90
  x(m) = fscanf( fid, '%lf\n', 1 );
  Rpar2(m) = fscanf( fid, '%lf\n', 1 );
  Rperp2(m) = fscanf( fid, '%lf\n', 1 );
end;

fprintf(1,'Generating plots\n');

subplot(2,4,5);
plot( x, Rpar+.001, 'b-', x, Rpar2, 'g-' );
xlabel('\theta_i/degrees');
ylabel('reflection coefficient');
title(' \mid R_{||} \mid - infinite reflection');
axis tight;

subplot(2,4,6);
plot( x, Rperp+.001, 'b-', x, Rperp2, 'g-' );
xlabel('\theta_i/degrees');
ylabel('reflection coefficient');
title(' \mid R_{\perp} \mid - infinite reflection');
axis tight;

fclose( fid );

fid   = fopen( 'infiniteTrans.txt' );

for m = 1:90
  x(m) = fscanf( fid, '%lf\n', 1 );
  Tpar(m) = fscanf( fid, '%lf\n', 1 );
  Tperp(m) = fscanf( fid, '%lf\n', 1 );
end;

fclose( fid );

fid   = fopen( 'correctedTrans.txt' );

for m = 1:90
  x(m) = fscanf( fid, '%lf\n', 1 );
  Tpar2(m) = fscanf( fid, '%lf\n', 1 );
  Tperp2(m) = fscanf( fid, '%lf\n', 1 );
end;

fprintf(1,'Generating plots\n');

subplot(2,4,7);
plot( x, Tpar+.001, 'b-', x, Tpar2, 'g-' );
xlabel('\theta_i/degrees');
ylabel('transmission coefficient');
title(' \mid T_{||} \mid - infinite transmission');
axis tight;

subplot(2,4,8);
plot( x, Tperp+.001, 'b-', x, Tperp2, 'g-' );
xlabel('\theta_i/degrees');
ylabel('transmission coefficient');
title(' \mid T_{\perp} \mid - infinite transmission');
axis tight;

fclose( fid );
