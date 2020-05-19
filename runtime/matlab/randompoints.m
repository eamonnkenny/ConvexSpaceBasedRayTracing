hold;
story3d( 'story1.txt', 'story1.txta' );

s = load( 'randompoints.txt' );
for i = 1:size(s,1)
  if ( s(i,4) == 1 )
    plot( s(i,1), s(i,2), 'r+' );
  else
    plot( s(i,1), s(i,2), 'bo' );
  end;
end;

t = load( 'bts.txt' );
plot( t(:,1), t(:,2), 'g^' );

title( 'red meets SIR, blue fails' );

hold;
