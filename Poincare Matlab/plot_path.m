function  plot_path(num_particles)
%This function plot the magnetic filed lines trajectories in the array of
%coils of simulation case.
%   First the function loads and plot the coils shape (filamentary aproximation). 
%Second the function loads files that contains data obtained of SCRcode.
    %h=figure;
    hold on; 
    coils=load_data();
    for j=2:12
        coil=coils{j};
        plot3(coil(:,1),coil(:,2),coil(:,3),'r');
    end
    
    paths=load_data_particle(num_particles);%num_particles);
     
    for i=1:79;
        path=paths{i};
       plot3(path(:,1),path(:,2),path(:,3),'b');
    end
    %figure_name=sprintf('Results/field_paths3.fig');
    %saveas(h,figure_name);
    %hold off;

end

