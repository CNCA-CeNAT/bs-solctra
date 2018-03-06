function Rni = Ri(coil,line)

    line_size = size(line);
    
    coil_size = size(coil);
    
    for i = 1 : line_size(1) % loop for each cordinate of domain
        
        
        for t = 1 : coil_size(1) % loop for each segment of coil
            
            Rni(t,:,i) = line(i,:)-coil(t,:);            
            
        end    
        
        
    end
    



end
