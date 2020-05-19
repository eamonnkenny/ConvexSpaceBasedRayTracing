clear all
hold

axis([0 31 0 21] );
story1;

M0=fullrays(2,0);
k1=size(M0,2)
title('1st order reflection paths');

M=fullrays2(3,k1);

for k = 1:k1
  M(k) = M0(k);
end;

title('2nd order reflection paths');

k1=size(M,2)
M0=fullrays3(4,k1);

for k = 1:k1
  M0(k) = M(k);
end;

title('1st order diffraction paths');
k1=size(M0,2)
M=diffraction(3,k1);

for k = 1:k1
  M(k) = M0(k);
end;

k1=size(M,2)
k1 = k1 + 1;
story1;
reflect3( 'reflection.txt' );
reflect2( 'reflection.txt' );
reflect1( 'reflection.txt' );
reflect0( 'reflection.txt' );
title('0^{th}-3^{th} order reflections, colour code- magneta, red, green, blue' );
M(k1) = getframe;

story1
diffraction2
k1 = k1+1;
M(k1) = getframe;

hold
