function [coils] = load_data()


    for k = 0:11
    
        coil_name = sprintf('Bobina%dm.txt', k);
  
        coils{k+1} = importdata(coil_name);
  
    end

    
end