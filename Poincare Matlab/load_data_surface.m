function surfaces =load_data_surface( num_surfaces )
%LOAD_DATA_SURFACE Summary of this function goes here
%   Detailed explanation goes here
    for k = 1:num_surfaces
            
        file_name= sprintf('Resultsmt/Surface0_%d.txt',k);%%%cambiado 5/3/2018
  
        surfaces{k}= importdata(file_name);
  
    end

end

