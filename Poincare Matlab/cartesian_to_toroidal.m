function [beta,alfa,r] = cartesian_to_toroidal(R,point,turn)
%Compute a point in toroidal coordinates from rectangular coordinates
%   Remember that poloidal coordinates are: 
%   x=(R+r*cos(alfa))*cos(beta);y=(R+r*cos(alfa))*sin(beta);z=r*sin(alfa);
%   alfa:poloidal angle, beta: toroidal angle
 
    %This line calculated r, for a point centering in R
    %%%%%%%%vectorO=([point(1,1),point(1,2)]./norm([point(1,1),point(1,2)]))*R;
    %%%%%%%%vectorO(1,3)=0;
    %%%%%%%%rvector=point-vectorO;
    %%%%%%%%r=norm(rvector);%norm([rvector(1,1),rvector(1,3)]);
    
    %alfa determination REVISAR-------------------------------------
    %%%%%%%%vectorxy=[vectorO(1,1)+rvector(1,1),vectorO(1,2)+rvector(1,2)]
    %%%%%%%%alfa=abs(asin((rvector(1,3)/r))); 
    %%%%%%%%if point(1,3)<0&norm([point(1,1),point(1,2)])<R%if point(1,3)<0&point(1,3)~=cuadrante III
        %%%%%%%%alfa= alfa+pi;
    %%%%%%%%else
        %%%%%%%%if point(1,3)<0&norm([point(1,1),point(1,2)])>R%%Cuadrante IV
             %%%%%%%%alfa= 2*pi-alfa;
        %%%%%%%%else
            %%%%%%%%if point(1,3)>0&norm([point(1,1),point(1,2)])<R%%Cuadrante II
              %%%%%%%%alfa= pi-alfa;
            %%%%%%%%end
        %%%%%%%%end
    %%%%%%%%end
    
    %if point(1,3)<0&point(1,3)~=0
        %alfa=alfa+pi;%+(pi/2);
    %else
        %if norm(vectorxy)>R&point(1,3)>0
            %alfa=alfa;%+pi/2;
        %else
            %if norm(vectorO+rvector)>R&point(1,3)<0&point(1,3)~=0 
                %alfa=alfa+pi;
            %end
        %end
        %if norm(vectorO+rvector)<R&point(1,3)>0
            %alfa=alfa+pi;--------------------------------------------
    %end
    
    %beta determination on xy plane and we add 2*pi turn
            %normxy=norm([point(1,1),point(1,2)]);
    %rxyz=(rvector+vectorO);   
    if point(1,2)<0%%% III o IV cuadrante
            
            beta=acos(point(1,1)/norm([point(1,1),point(1,2)]));%[rxyz(1,1),rxyz(1,2)]
            beta=(2*pi-beta)+2*pi*turn;        
    else
        beta=acos(point(1,1)/norm([point(1,1),point(1,2)])); %[rxyz(1,1),rxyz(1,2)]
        beta=beta+2*pi*turn;
    end
%    if point(1,2)<0%&point(1,2)<0
        %beta=pi+(pi-beta)+(2*pi*turn);
    %end
        %beta=beta;        
    %else
        %if point(1,1)<0&point(1,2)<0
            %beta=beta+pi+2*pi*turn;
        %else
            %if point(1,1)>0&point(1,2)<0
                %beta=beta+pi+(pi/2)+2*pi*turn;
            %else
                %beta=beta+2*pi*turn;
            %end
        %end
    %end    
end

