function [poincare] = poincare_section(angle,traj)
%POINCARE_SECTION:	Created by Esteban Zamora Picado, version june 2013
%This function enable to compute the poincaré section from
%several trajectories previously calculated with the FIELD_PROFILE function
%%If you need to remember about intersection of  plane and a line,  visit:http://www.tec-digital.itcr.ac.cr/revistamatematica/cursos-linea/Algebra-Lineal/algebra-vectorial-geova-walter/node6.html
    %%loading the trajectories from files.
    
    trajectory_number = sprintf('RBSSolctra/path%d.txt',traj);
    trajectory= importdata(trajectory_number);
    %%%trajectory{1}=trajectories{traj};
    turn=0;
    %%Loop for change of coordinates the trajectory points (Rectangular to toroidals)
    %for i=1:size(trajectory,2)
        path=trajectory;
        for j=1:size(path,1)-1
            tor(j,1)= cartesian_to_toroidal(0.2381,path(j,:),turn);%%%%%%%%[tor(j,1),tor(j,2),tor(j,3)]= cartesian_to_toroidal(0.2381,path(j,:),turn);
            if path(j,2)<0&path(j+1,2)>0
                p1(turn+1,:)=path(j,:);
                p2(turn+1,:)=path(j+1,:);
                turn=turn+1;
            end
        end
    %end
    %%a=1
    %%file= sprintf('Results4/Phi53/toroidal_%d.txt',a);
    %%save(file,'tor', '-ASCII');
    Beta=tor(:,1);
    poincare_point=1;
    poincare(poincare_point,:)=[0,0,0];
   %%%%%%O=[cos(angle*pi/180)*(0.2381-0.1004),sin(angle*pi/180)*(0.2381-0.1004),1];
    %%%%%P=[(0.2381)*cos(angle*pi/180),(0.2381)*sin(angle*pi/180),0];
    %%%%%Q=[cos(angle*pi/180),sin(angle*pi/180),0];
    %%%%%%%%%%%%%%%%Redefiniendo los puntos del plano%%%%%%%%%%%
    %%O=[(0.2381+tor(j,3)*cos(0))*cos(angle*pi/180),(0.2381+tor(j,3)*cos(0))*sin(angle*pi/180),tor(j,3)*sin(0)];
                
    %%%P=[(0.2381+tor(j,3)*cos(90*pi/180))*cos(angle*pi/180),(0.2381+tor(j,3)*cos(90*pi/180))*sin(angle*pi/180),tor(j,3)*sin(90*pi/180)];
    
    %%Q=[(0.2381+tor(j,3)*cos(225*pi/180))*cos(angle*pi/180),(0.2381+tor(j,3)*cos(225*pi/180))*sin(angle*pi/180),tor(j,3)*sin(225*pi/180)];
    
    O=[(0.2381+0.0944165*cos(0))*cos(angle*pi/180),(0.2381+0.0944165*cos(0))*sin(angle*pi/180),0.0944165*sin(0)];
    P=[(0.2381+0.0944165*cos(pi/2))*cos(angle*pi/180),(0.2381+0.0944165*cos(pi/2))*sin(angle*pi/180),0.0944165*sin(pi/2)];
    Q=[(0.2381+0.0944165*cos(pi*1.25))*cos(angle*pi/180),(0.2381+0.0944165*cos(pi*1.25))*sin(angle*pi/180),0.0944165*sin(pi*1.25)];
    
    %%surface2d(j,2)=(0.2381+toroidal_surface(j,3)*cos(toroidal_surface(j,2)))*sin(toroidal_surface(j,1));
            
                %%urface2d(j,3)=toroidal_surface(j,3)*sin(toroidal_surface(j,2));
    
    
    
    
    
    %%plane_vector=cross((P-Q),(P-O));
    plane_vector(1,1)=(P(1,2)-Q(1,2))*(P(1,3)-O(1,3))-((P(1,2)-O(1,2))*(P(1,3)-Q(1,3)));
    plane_vector(1,2)=-((P(1,1)-Q(1,1))*(P(1,3)-O(1,3))-(P(1,1)-O(1,1))*(P(1,3)-Q(1,3)));
    plane_vector(1,3)=(P(1,1)-Q(1,1))*(P(1,2)-O(1,2))-(P(1,1)-O(1,1))*(P(1,2)-Q(1,2));
    %%loop for search the intersection points between the plane selected and the
    %%line defined by two points, or one point in the case that the
    %%point is contained in the plane.
    for l=0:turn-1
        for k=2:size(Beta)
            if Beta(k)==((angle*pi/180)+(2*pi*l))
                poincare(poincare_point,:)=path(k,:);
                poincare_point=poincare_point+1;
            else
                if Beta(k)<((angle*pi/180)+(2*pi*l))&Beta(k+1)>((angle*pi/180)+(2*pi*l));
                    %line ecuation
                    line_vector=path(k+1,:)-path(k,:);
                    %Intersection of trajectory with the plane
                    %x=path(k,1)+((dot(plane_vector,P)-(plane_vector(1,1)*path(k,1)+plane_vector(1,2)*path(k,2)+plane_vector(1,3)*path(k,3)))/(plane_vector(1,1)*line_vector(1,1)+plane_vector(1,2)*line_vector(1,2)+plane_vector(1,3)*line_vector(1,3)))*line_vector(1,1);
                    %y=path(k,2)+((dot(plane_vector,P)-(plane_vector(1,1)*path(k,1)+plane_vector(1,2)*path(k,2)+plane_vector(1,3)*path(k,3)))/(plane_vector(1,1)*line_vector(1,1)+plane_vector(1,2)*line_vector(1,2)+plane_vector(1,3)*line_vector(1,3)))*line_vector(1,2);
                    %z=path(k,3)+((dot(plane_vector,P)-(plane_vector(1,1)*path(k,1)+plane_vector(1,2)*path(k,2)+plane_vector(1,3)*path(k,3)))/(plane_vector(1,1)*line_vector(1,1)+plane_vector(1,2)*line_vector(1,2)+plane_vector(1,3)*line_vector(1,3)))*line_vector(1,3);
                    
                    x=path(k,1)+((dot(plane_vector,P)-(plane_vector(1,1)*path(k,1)+plane_vector(1,2)*path(k,2)+plane_vector(1,3)*path(k,3)))/(plane_vector(1,1)*line_vector(1,1)+plane_vector(1,2)*line_vector(1,2)+plane_vector(1,3)*line_vector(1,3)))*line_vector(1,1);
                    y=path(k,2)+((dot(plane_vector,P)-(plane_vector(1,1)*path(k,1)+plane_vector(1,2)*path(k,2)+plane_vector(1,3)*path(k,3)))/(plane_vector(1,1)*line_vector(1,1)+plane_vector(1,2)*line_vector(1,2)+plane_vector(1,3)*line_vector(1,3)))*line_vector(1,2);
                    z=path(k,3)+((dot(plane_vector,P)-(plane_vector(1,1)*path(k,1)+plane_vector(1,2)*path(k,2)+plane_vector(1,3)*path(k,3)))/(plane_vector(1,1)*line_vector(1,1)+plane_vector(1,2)*line_vector(1,2)+plane_vector(1,3)*line_vector(1,3)))*line_vector(1,3);
                    
                    poincare(poincare_point,:)=[x,y,z];
                    poincare_point=poincare_point+1;
                end
                
            end
        end
    end
    
   
   %%This code is for plot a trajectorie in order to orintate the poincaré section   
   %trajectory_number = sprintf('Results/particle%d.txt',55);
   %trajectories{1}= importdata(trajectory_number);
   %path=trajectories{1};
   %plot3(path(:,1),path(:,2),path(:,3));

   %%code for coordinates change verification
   %for i=1:size(path,1)-1
       %beta=tor(i,1);
       %B(i,1)=(0.2381+tor(i,3)*cos(tor(i,2)))*cos(beta);
       %B(i,2)=((0.2381+tor(i,3)*cos(tor(i,2)))*sin(beta));
       %B(i,3)=tor(i,3)*sin(tor(i,2));
   %end

end

