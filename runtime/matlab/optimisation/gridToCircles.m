function [ x1 y1 colors radius ] = gridToCircles( x, y, gridValues )

M = size(x,1);
N = size(y,1);

T = M*N;

x1      = size(T,1);
y1      = size(T,1);
colors  = size(T,1);
radius  = size(T,1);

k = 1;
for n = 1:N
  for m = 1:M
        % Zero capacity points are marked with -1 in gridValues matrix
        % This has the affect of no circle being drawn at these points
        if ( gridValues(n,m) >= 0 )
          x1(k) = x(m); 
          y1(k) = y(n); 
          colors(k) = gridValues(n,m)+1;
          if (gridValues(n,m) >= 1) && (gridValues(n,m) <= 10)
            % large circles represent meeting user capacity requirements
            radius(k) = 50.0;
          else
            % small circles represent exceeded user capacity requirements
            radius(k) = 5.0;
          end;
          k = k + 1;
        end;
  end;
end;
