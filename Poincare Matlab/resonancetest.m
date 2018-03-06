function resonancetest(resonance)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    load('B0.mat');
    load('X0.mat');
    load('Y0.mat');
    load('Z0.mat');

    h=figure(2);
    
    %XY=sqrt(X.^2+Y.^2);
    c=contour(X,Z,B,resonance);
    close(h);

    line_number=1;
    k=1;
    Beta=0;%%%%%%%%%%%RECORDAR CAMBIAR
    h=figure(1);   
    hold all;
    while k<=size(c,2)
        contourpoints=c(2,k);
        row=0;
        temp=[];
        for i=1:contourpoints
            
            rxy=c(1,k+1)/abs(cos(Beta*pi/180))-((c(1,k+1)/abs(c(1,k+1)))*0.2381);
            rz=c(2,k+1);
            rnorm=norm([rxy rz]);
            
            if rnorm<=0.0944165
                
                row=row+1;
                
                temp(row,:)=[(c(1,k+1)/abs(cos(Beta*pi/180))) c(2,k+1)];
                
            end
            
            k=k+1;
            
        end
        contourxy{line_number}=temp;
        
        if row~=0
            line_number=line_number+1;
            row=0;
        end
        k=k+1;
        
        
    end
    for l=1:line_number-1
        xy=contourxy{:,l};
        plot(abs(xy(:,1)),xy(:,2),'r');
        %plot(abs(xy(:,1)),xy(:,2),'r');
        
    end
    
     %for angle=0:360
        %    x(angle+1)=(0.2381+0.0944165*cos(angle*pi/180))*cos(Beta*pi/180);
       %     y(angle+1)=(0.2381+0.0944165*cos(angle*pi/180))*sin(Beta*pi/180);
      %      z(angle+1)=0.0944165*sin(angle*pi/180);
     %end
        
       
        
      % plot(((x/abs(x)).*sqrt((x.^2)+(y.^2))),z,'k');
end

