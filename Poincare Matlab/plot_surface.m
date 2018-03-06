function plot_surface( nums_surface,Beta)
%PLOT_SURFACE Summary of this function goes here
%   Note: in 2D(x,z graphic) graphic the viewpoint is from -y to +y
    surfaces=load_data_surface(nums_surface)    ;
    %surfaces=modifsurface();
    for i=1:nums_surface
        surface1=surfaces{i};  
        surface=surface1;%flipdim(surface1,)%rot90(surface1,2);
        switch i
            case 1
                    %h=figure(1)
                    color='r'%0;%noislas externas eliminadas
                    hold all;
            case 2
                    color=0;%no
            case 3
                    color=0;%n0
            case 4
                    color=0;%no
            case 5
                    color='b';
            case 6
                    color='g'; 
            case 7
                    color='m'; 
            case 8
                    color='c';
            case 9
                    color='r';
            case 10
                    color='g';
            case 11
                    color='r';
            case 12
                    color='m';
            case 13
                    color='c';
            case 14
                    color='c';
            case 15
                    color='g'; 
            case 16
                    color='r';
            case 17
                    color='m';
            case 18
                    color='c';
            case 19
                    color='b';
            case 20
                    color='g';
            case 21
                    color='r';
            case 22%%%eje
                    color='m';
            case 23
                    color='b';
            case 24
                    color='b'; 
            case 25
                    color='c';
            case 26
                    color='r';
            case 27
                    color=0;
            case 28
                    color=0;
            case 29
                    color=0;
            case 30
                    color=0;
            case 31
                    color=0;
            case 32
                    color=0;
            case 33
                    color=0;
            case 34
                    color=0;
            case 35
                   color=0;
            case 36
                    color=0;
            case 37
                    color=0;
            case 38
                    color=0;
            case 39 
                    color='b';
            case 40
                    color=0;
            case 41
                    color=0;
            case 42
                    color=0;%%%%revisar pues muestra comportamiento anomalo
            case 43
                    color=0;
            case 44
                    color='m'; 
            case 45
                    color='b';
            case 46
                    color='c';%%revisar
            case 47
                    color=0;
            case 48
                    color=0;
            case 49
                    color=0;%islas externas
            case 50
                    color='r';%superficie mas externa
            case 51
                    color=0;
            case 52
                    color=0;%no
            case 53
                    color=0;                     
                    
            otherwise
                %%%exception
        end
        if color~=0
            %scatter(surface(:,1),surface(:,3),24,'.',color);
            %scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)).*(surface(:,1)./sqrt(surface(:,1).^2)),surface(:,3),24,'.',color);  
            scatter(sqrt((surface(:,1).^2)+(surface(:,2).^2)),surface(:,3),24,'.',color);
            %%%scatter(sqrt((surface(1,1).^2)+(surface(1,2).^2)),surface(1,3),24,'.','g');%%%
            %%%scatter(sqrt((surface(2,1).^2)+(surface(2,2).^2)),surface(2,3),24,'.','b');%%%modificado 5/3/20188
            
            %%scatter(sqrt((surface(3,1).^2)+(surface(3,2).^2)),surface(3,3),24,'.','k');%%%
            %%scatter(sqrt((surface(4,1).^2)+(surface(4,2).^2)),surface(4,3),24,'.','m');
            %scatter3(surface(:,1),surface(:,2),surface(:,3),24,'.',color);
        end            
            
       
        %%%for j=1:size(surface,1)
            %%%[toroidal_surface(j,1),toroidal_surface(j,2),toroidal_surface(j,3)]=cartesian_to_toroidal(0.2481,surface(j,:),0);
            
                %%%surface2d(j,1)=(0.2481+toroidal_surface(j,3)*cos(toroidal_surface(j,2)))*cos(toroidal_surface(j,1));
                %%%surface2d(j,2)=(0.2481+toroidal_surface(j,3)*cos(toroidal_surface(j,2)))*sin(toroidal_surface(j,1));
            
               %%% surface2d(j,3)=toroidal_surface(j,3)*sin(toroidal_surface(j,2));
            
        %%%end
        
    
        %%scatter3(surface(:,1),surface(:,2),surface(:,3),'.');%,view(0,0);f
        %color=input('Ingrese el valor de la variable: ','s');
        %scatter(surface(:,1),surface(:,3),24,'.',color);%,view(0,0);f
        %hold all;
        

        
        %scatter(surface2d(:,1),surface2d(:,3),'.','k');%,view(0,0);
        %scatter3(surface2d(:,1),surface2d(:,3),surface2d(:,3),'.','b');%,view(0,0);
    end   
    
       for angle=0:360
            x(angle+1)=(0.2381+0.0944165*cos(angle*pi/180))*cos(Beta*pi/180);
            y(angle+1)=(0.2381+0.0944165*cos(angle*pi/180))*sin(Beta*pi/180);
            z(angle+1)=0.0944165*sin(angle*pi/180);
       end
        
        %plot3(x,y,z,'k');
        %plot(sqrt((x.^2)+(y.^2)),z,'k');
        %plot(sqrt((x.^2)+(y.^2)),z,'k');
        %plot(((x/abs(x)).*sqrt((x.^2)+(y.^2))),z,'k');
        plot(sqrt((x.^2)+(y.^2)),z,'k');
        %%%%p1(1,:)=[0,0,0];
        %%%%p1(2,:)=[(cos(229.8*pi/180))*0.2481,(sin(229.8*(pi/180)))*0.2481,0];
        %%%%plot3(p1(:,1),p1(:,2),p1(:,3),'b');        
    

end

