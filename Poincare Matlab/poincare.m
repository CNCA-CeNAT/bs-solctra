function poincare()
%POINCARE Created by Esteban Zamora Picado, version june 2013
%This fuction excecute and plot results obtained with 
%the function POINCARE_SECTION.
%   This function is useful because enable to implemet PARFOR loop
    %for Beta=0:45:90
       % Beta=53;
   
       parfor j=1:64%%parfor
            %poincare_cell{t}=poincare_section(Beta,t); 
            %surface=poincare_cell{t};%particle};
            %file= sprintf('Results4/Phi53/Surface53_%d.txt',t);
            %save(file,'surface', '-ASCII');
            parallel_poincare(j);
       end
        
        %%This code lines enable to plot the intersection points for each trajectory.
        %h=figure;
        %hold on;
        %particle=1;
        %for particle=1:52
            %surface=poincare_cell{particle};%particle};
            %file= sprintf('Results2/Surface45,%d.txt',particle);
            %save(file,'surface', '-ASCII');
            %scatter3(surface(:,1),surface(:,2),surface(:,3),'.','b'),view(0,0);%[0.2497,-0.2497,0]); 
            %name= sprintf('Results/surface%d.txt',particle);
            %save(surface,'name','-ASCII');
        %end
        %figure_name=sprintf('Results/Poincare9mil%ddegrees.fig',Beta);
        %saveas(h,figure_name);
        %hold off;
    %end    
    

end

