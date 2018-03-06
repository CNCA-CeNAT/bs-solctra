function resonance( beta, steps )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

    global coils e leng_segment;
    coils = load_data();
    [e,leng_segment]=e_roof(coils);
    i=1;
   
    
    %for r=0:0.03:0.094416
       %for alfa=0:10:360
       
         %%[toroidal_surface(j,1),toroidal_surface(j,2),toroidal_surface(j,3)]=cartesian_to_toroidal(0.2481,surface(j,:),0);
         %xt(i,1) = ((0.2381+r*cos(alfa*pi/180))*cos(beta*pi/180)); 
         %yt(i,1) = ((0.2381+r*cos(alfa*pi/180))*sin(beta*pi/180));
         %zt(i,1)= (r*sin(alfa*pi/180));
         %i=i+1;
       %end
            
   %end
    r=0.094416;
    
    
    xf=((0.2381+r*cos(180*pi/180))*cos(beta*pi/180));
    xi=((0.2381+r*cos(0*pi/180))*cos(beta*pi/180));
    xlength=xf-xi;    
    xstep=abs(xlength/steps);
     if xf>xi&xlength>0
        xstep=-xstep;
    end
    x=xf:xstep:xi;
    
    
    yf=((0.2381+r*cos(180*pi/180))*sin(beta*pi/180));
    yi=((0.2381+r*cos(0*pi/180))*sin(beta*pi/180));
    ylength=yf-yi;
    ystep=abs(ylength/steps);  
    if yf>yi&ylength>0
        y=yf:-ystep:yi;
    elseif ylength==0
        y=zeros(1,steps+1);
    else
        y=yf:ystep:yi;
    end
    
    zstep=2*r/steps;
    z= (r*sin(270*pi/180)):zstep:(r*sin(90*pi/180));
    
    
    %xs=sortrows(xt);
    %ys=sortrows(yt);
    %zs=sortrows(zt);
    %x=transpose(xs);
    %y=transpose(ys);
    %z=transpose(zs);0.043331419726714

    [X,Z] = meshgrid(x,z); 
    
    Y = meshgrid(y);
    B = zeros(size(X));
    
    for i=1:size(X)
        for j=1:size(X)
            B0=[0 0 0];
            parfor k = 1 : 12         
                [Rim,Rfm] = R_vectors(coils{k},[X(i,j) Y(i,j) Z(i,j)]);       
                B0=B0+magnetic_field(e(:,:,k),Rim,Rfm,leng_segment(:,k));         
            end
            B(i,j) = norm(B0);
            
            
        end
        progress=(i/size(x,2))*100;
            %if 0==mod(progress,2)
                clc;
                formatSpec = 'El progreso para la trayectoria %d%%\n';
                fprintf(formatSpec,progress);
            %end
    end
    
   % c=contour(X,Z,B,resonance);
   % contourpoints=c(2,1);
    %line_number=1;
    
    %k=1;
    %while k<=size(c,2)
        %row=1;
        %for i=k+1:contourpoints
            
            %xcontour(row,line_number)=c(1,i);
            %ycontour(row,line_number)=c(2,i);
            
            %k=k+1;
            %row=row+1;
        %end
        
        %line_number=line_number+1;
        %contourpoints=c(2,k+2);
    
    %end
    savefile1 = 'B0.mat';
    savefile2 = 'X0.mat';
    savefile3 = 'Y0.mat';
    savefile4 = 'Z0.mat';
    
    save(savefile1, 'B');
    save(savefile2, 'X');
    save(savefile3, 'Y');
    save(savefile4, 'Z');



end

