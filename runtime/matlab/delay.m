% DELAY evaluates for each path the delays between tx and rx and the phase shift between 
% all the transmitting points and all the receiving points.

%-------------------------------------------------------------------------------------------
% The first thing that we can do is add to 'paths' a field called 'maindel',
% containing the "main delay" between the central point of Tx and Rx.
%-------------------------------------------------------------------------------------------

for n=1:multipaths  
    
    % Note that tmp variable contains the squared distance over the 3 components
    dist=0;
    
    if ( paths(n).points == 0 )
        % This is the direct ray
        tmp=(tx-rx).^2;  
        dist=sqrt(sum(tmp));
    else
        tmp=(tx-paths(n).coord(1,:)).^2;  
        dist=sqrt(sum(tmp));
        for m=1:(paths(n).points-1)
            tmp=(paths(n).coord(m+1,:)-paths(n).coord(m,:)).^2;
            dist=dist+sqrt(sum(tmp));
        end    
        tmp=(paths(n).coord(paths(n).points,:)-rx).^2;
        dist=dist+sqrt(sum(tmp));
    end
    
    paths(n).maindel=dist/C;
    
end


%-------------------------------------------------------------------------------------------
% Thus, now we can compute the min and max of all the main delays, in order to know the time 
% spread of this channel; they are stored in mindel and maxdel.
%-------------------------------------------------------------------------------------------

mindel=10;
maxdel=0;                          
for n=1:multipaths  
    if ( paths(n).maindel < mindel ) 
        mindel=paths(n).maindel;
    end
    if ( paths(n).maindel > maxdel ) 
        maxdel=paths(n).maindel;
    end
end   
tspread=maxdel-mindel;


%-------------------------------------------------------------------------------------------
% The second step is find out (for both the transmitter and the receiver) the relative  
% position of the impinging plane wave respect to the elements of the array and compute
% their phase shift due to their different position. 
% We store this new information in 2 new fields: - paths(n).ArrTxPhase , a (1 x M) vector
%                                                - paths(n).ArrRxPhase , a (1 x N) vector
%-------------------------------------------------------------------------------------------

for n=1:multipaths  
    
    % - Trasmitter - 
    
    % First: we have to compute the vector 'vec1' orthogonal to the plane wave leaving from
    % the Tx array.

    if ( paths(n).points == 0 )
        % Here we consider the special case of the direct ray:
        tmp=rx-tx;
        vec1=tmp/norm(tmp);
    else
        tmp=paths(n).coord(1,:)-tx; 
        vec1=tmp/norm(tmp);
    end
        
    % Second: we produce a matrix ArrTxPhase (1 x M), that contains the phase shift
    % between each point of the Tx array and the main delay of the central array.

    for a=1:M 
        tmp=ArrTx(a,:)-tx; 
        % Here we consider the case in which a trasmitting point is the central point of the array:
        if (tmp == 0)
            paths(n).ArrTxPhase(1,a)=0;
        else
            % 'vec2' is an vector (with norm=1) that links the central point of the array 
            %  with the considered transmitting point:
            vec2=tmp/norm(tmp);
            % The following operation returns the cosine of the angles between the 2 vectors. 
            co=vec1.*vec2;   
            % for each component, if extrapath > 0 , then the point is closer to the plane wave; 
            %                     if extrapath < 0 , then the point is farther to the plane wave.
            extrapath=-co.*abs(ArrTxDiff(1,:));   
            % Now we combine the information over the 3 coordinates into the amplitude of the "delta-path":
            tmp=extrapath(1,1)^2*sign(extrapath(1,1))+extrapath(1,2)^2*sign(extrapath(1,2))+extrapath(1,3)^2*sign(extrapath(1,3));
            % and then into the corresponding phase shift:
            paths(n).ArrTxPhase(1,a)=sign(tmp)*2*pi*( mod ( sqrt(sign(tmp)*tmp) ,lambda) ); 
        end
    end
    
    
    % - Receiver - 
    
    % First: we have to compute the vector 'vec' orthogonal to the plane wave impinging into
    % the Rx array.
    
    if ( paths(n).points == 0 )
        % Here we consider the special case of the direct ray:
        tmp=rx-tx;
        vec=tmp/norm(tmp);
    else
        tmp=rx-paths(n).coord(paths(n).points,:);
        vec1=tmp/norm(tmp);
    end
           
    % Second: we produce a matrix ArrRxPhase (1 x N), that contains the phase shift
    % between each point of the Rx array and the main delay of the central array.
    
    for a=1:N 
        % 'vec2' is an vector (with norm=1) that links the central point of the array  
        % with the considered receiving point:
        tmp=ArrRx(a,:)-rx; 
        % Here we consider the case in which a trasmitting point is the central point of the array:
        if tmp == 0
            paths(n).ArrRxPhase(1,a)=0;
        else
            vec2=tmp/norm(tmp);
            % The following operation returns the cosine of the angles between the 2 vectors 
            co=vec1.*vec2;  
            % for each component, if extrapath > 0 , then the point is closer to the plane wave; 
            %                     if extrapath < 0 , then the point is farther to the plane wave.
            extrapath=co.*abs(ArrRxDiff(1,:));   
            % Now we combine the information over the 3 coordinates into the modulus of the "delta-path":
            tmp=extrapath(1,1)^2*sign(extrapath(1,1))+extrapath(1,2)^2*sign(extrapath(1,2))+extrapath(1,3)^2*sign(extrapath(1,3));
            % and then we convert it into the corresponding phase shift:
            paths(n).ArrRxPhase(1,a)=sign(tmp)*2*pi*( mod ( sqrt(sign(tmp)*tmp) ,lambda) );  
        end
    end
    
    
end

