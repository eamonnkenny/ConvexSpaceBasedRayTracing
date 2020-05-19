% LOADPATH loads in memory the output of the Raytracing software, containig the multipath
% between the central point of both the Transmitter and the Receiver; 
% the input file is raypaths_count1_count2.txt .

str=sprintf('raypaths_%d_%d.txt', count1, count2);

fid=fopen(str, 'r');
if (fid==-1)
    str=sprintf('Input file "raypaths_%d_%d.txt" missing.', count1, count2);
    error (str); 
else
    str=sprintf('\n\n -  Now link %d -> %d is processed:', count1, count2);
    fprintf(str);
end

paths=[];

% The number of multipaths is read here; it's stored in a variable called multipaths.
tmp=fscanf( fid, '%s ', 1 );
multipaths=fscanf( fid, '%d\n', 1 );

% Every path is described through one element of a structure, called 'paths'. 
% Description of the used structure: 
% paths(n) contains:    - paths(n).numT 	    : number of Trasmissions
%                   	- paths(n).numR 	    : number of Reflections
%                   	- paths(n).numD 	    : number of Diffraction
%                   	- paths(n).points       : number of significant points (redundant information)
%                   	- paths(n).coord 	    : a matrix (path(n).points x 3) containing the coordinates of every point
%  						- paths(n).vari *	    : a matrix (path(n).points x 6) with various information (see later for more details)
%                   	- paths(n).fieldloss 	: the ray pathloss (see later for more details)
%
% * this choice may be non optimal, but if the number of reflections and diffractions is big (as in
% real situations it is), that matrix is sufficiently full of information and thus does not waste a lot of memory 

% REMEMBER: the ray number 'i' in raypaths.txt is stored here in paths(i+1)!!

for n=1:multipaths

   % Here it is loaded the number of Trasmissions, Reflections and Diffraction:
   waste=fscanf( fid, '%s ', 4 );
   paths(n).numT=fscanf( fid, '%d', 1 );
   waste=fscanf( fid, '%s ', 1 );
   paths(n).numR=fscanf( fid, '%d', 1 );
   waste=fscanf( fid, '%s ', 1 );
   paths(n).numD=fscanf( fid, '%d', 1 );
   paths(n).points=paths(n).numT+paths(n).numR+paths(n).numD;
   
   % The following line initializes the matrix diff with 100 in all the positions 
   paths(n).vari=100*ones(paths(n).points,6);
   
   for m=1:((paths(n).points)+2)
  
      type=fscanf( fid, '%s\n', 1 );

		switch type
     
         % If the point is the transmitter, it stores: nothing.
      	case 'TRANSMITTER',  waste=fscanf( fid, '%lf\n', [3,1] );  
         
         % If the point is a transmitting point, it stores: nothing.
         case 'RECEIVER',  waste=fscanf( fid, '%lf\n', [3,1] );
         
         % If the point is the transmitter, it stores: - in paths(n).coord(m-1,:) the 3 coordinates of the trasmitting point; 
         % 											   - in paths(n).diff(m-1,1) a 10 and in the following 3 columns the 3
         % 											     components of a vector orthonormal to the plane of reflection.           
      	case 'TRANSMISSION',    paths(n).coord(m-1,:)=fscanf( fid, '%lf\n', [1,3] ); 
         						paths(n).vari(m-1,1)=10;
          						tmp=fscanf( fid, '%lf\n', [3,1] );
                                paths(n).vari(m-1,2)=tmp(1,1); 
                                paths(n).vari(m-1,3)=tmp(2,1); 
                                paths(n).vari(m-1,4)=tmp(3,1);
       
         % If the point is a reflecting point, it stores: - in paths(n).coord(m-1,:) the 3 coordinates of the reflecting point;
         % 												  - in paths(n).diff(m-1,1) a 20 and in the following 3 columns the 3
         % 												    components of a vector orthonormal to the plane of reflection.              
      	case 'REFLECTION', 	    paths(n).coord(m-1,:)=fscanf( fid, '%lf\n', [1,3] );
         						paths(n).vari(m-1,1)=20; 
         						tmp=fscanf( fid, '%lf\n', [3,1] );
                                paths(n).vari(m-1,2)=tmp(1,1); 
                                paths(n).vari(m-1,3)=tmp(2,1); 
                                paths(n).vari(m-1,4)=tmp(3,1);
            
         % If the point is a diffracting point, it stores: - in paths(n).coord(m-1,:) the 3 coordinates of the diffracting point;
         % 												   - in paths(n).diff(m-1,:) the 6 components of 2 vectors  
         %  												 orthonormal to the 2 planes of diffraction.              
      	case 'DIFFRACTION',	 	paths(n).coord(m-1,:)=fscanf( fid, '%lf\n', [1,3] );
         						paths(n).vari(m-1,4)=4; 
         						paths(n).vari(m-1,:)=fscanf( fid, '%lf\n', [1,6] );
                               	
      end
       
    end
    
   % Here we load the pathloss information.
   % It is composed of 9 complex numbers, that describe the channels between Ex, Ey, Ez of the trasmitting
   % antenna to Ex, Ey, Ez of the receiving antenna; Ex->Ex, Ey->Ey and Ez->Ez are direct links, while all 
   % the other are links due to the crosspolarization of the channel.
   for a=1:3 
       tmp=fscanf( fid, '%lf\n', [2,3] );
       % This is the complex number that describes the pathloss:
       paths(n).fieldloss(a,:)=tmp(1,:)+i.*tmp(2,:);
   end
       
end

fclose(fid);


