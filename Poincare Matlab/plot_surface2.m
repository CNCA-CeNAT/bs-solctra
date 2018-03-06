function plot_surface( nums_surface,Beta)
%PLOT_SURFACE Summary of this function goes here
%   Note: in 2D(x,z graphic) graphic the viewpoint is from -y to +y
    surfaces=load_data_surface(nums_surface);
    
    for i=1:nums_surface
        surface1=surfaces{i};  
        surface=surface1;%flipdim(surface1,)%rot90(surface1,2);
        switch i
            case 1
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','r');
                    hold all;
            case 2
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','m');
            case 3
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','c');
            case 4
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','b');
            case 5%%%%más externa
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','g');
            case 6  %1
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','r'); 
            case 7
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','m');
            case 8
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','c');
            case 9
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','r');
            case 10
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','g');
            case 11
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','r');
            case 12
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','m');
            case 13
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','c');
            case 14
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','b');
            case 15
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','g'); 
            case 16
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','r');
            case 17
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','m');
            case 18
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','c');
            case 19
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','b');
            case 20
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','g');
            case 21
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','r');
            case 22
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','m');
            case 24
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','b');
            case 24
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','b'); 
            case 25
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','c');
            case 26
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','r');
            case 27
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 28
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 29
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 30
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','b');
            case 31
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','c');
            case 32
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 33
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 34
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',); 
            case 35
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 36
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 37
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 38
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 39
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','b');
            case 40
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','c');
            case 41
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 42
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','c');
            case 43
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 44
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','m'); 
            case 45
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','b');
            case 46
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','c');
            case 47
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 48
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 49
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','g');
            case 50
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','m');
            case 51
                    %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',);
            case 52
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','r');
            case 53
                    scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.','g');                     
                    
            otherwise
        end
        
       
        %%%for j=1:size(surface,1)
            %%%[toroidal_surface(j,1),toroidal_surface(j,2),toroidal_surface(j,3)]=cartesian_to_toroidal(0.2481,surface(j,:),0);
            
                %%%surface2d(j,1)=(0.2481+toroidal_surface(j,3)*cos(toroidal_surface(j,2)))*cos(toroidal_surface(j,1));
                %%%surface2d(j,2)=(0.2481+toroidal_surface(j,3)*cos(toroidal_surface(j,2)))*sin(toroidal_surface(j,1));
            
               %%% surface2d(j,3)=toroidal_surface(j,3)*sin(toroidal_surface(j,2));
            
        %%%end
        
    
        %%scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),'.');%,view(0,0);f
        %color=input('Ingrese el valor de la variable: ','s');
        %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',color);%,view(0,0);f
        %hold all;
        

        
        %scatter(surface2d(:,1),surface2d(:,3),'.','k');%,view(0,0);
        %scatter3(surface2d(:,1),surface2d(:,3),surface2d(:,3),'.','b');%,view(0,0);
    end   
       for angle=0:360
            x(angle+1)=(0.2381+0.0944165*cos(angle*pi/180))*cos(Beta*pi/180);
            y(angle+1)=(0.2381+0.0944165*cos(angle*pi/180))*sin(Beta*pi/180);
            z(angle+1)=0.0944165*sin(angle*pi/180);
        end
        plot(((x/abs(x)).*sqrt((x.^2)+(y.^2))),z,'k');
        %plot3(x,y,z,'k');
        %plot(x,z,'b');
        %%%%p1(1,:)=[0,0,0];
        %%%%p1(2,:)=[(cos(229.8*pi/180))*0.2481,(sin(229.8*(pi/180)))*0.2481,0];
        %%%%plot3(p1(:,1),p1(:,2),p1(:,3),'b');
        
    

end

