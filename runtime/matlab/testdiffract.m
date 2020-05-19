vib = load('diffract.txt');
vrb = load('diffract2.txt');

figure(1);
hold;
x = [210,210];
y = [0.05,0.5];
plot( vib(:,1), vib(:,2), 'b-' );
plot( x, y, 'r-' );
xlabel('Angles \phi/ degrees');
ylabel('Magnitude |v_B^i|');
legend('UTD \phi''=30^0, \rho=100\lambda' );
axis([0 360 0 0.5]);
hold;

figure(2);
hold;
plot( vrb(:,1), vrb(:,2), 'b-' );
x = [150,150];
plot( x, y, 'r-' );
xlabel('Angles \phi / degrees');
ylabel('Magnitude |v_B^r|');
legend('UTD \phi''=30^0, \rho=100\lambda' );
axis([0 360 0 0.5]);
hold;
