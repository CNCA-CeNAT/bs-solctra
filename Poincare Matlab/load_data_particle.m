function [particles] = load_data_particle(num_particles)


    for k = 1:num_particles
            
        coil_name = sprintf('Results5/path%d.txt', k);
  
        particles{k}= importdata(coil_name);
  
    end

    
end