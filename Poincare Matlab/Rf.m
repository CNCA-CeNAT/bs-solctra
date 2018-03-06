%This fuction compute the Rf vector (Rf:is the vector from the end of segment to the compute point)
function Rnf = Rf(coil,torus)

    torus_size = size(torus);
    
    coil_size = size(coil);
    
    for i = 1 : 1 : torus_size(1) % loop for each cordinate of domain
        
        
        for t = 1 : 1: coil_size(1) % loop for each segment of coil
            
            if t < coil_size(1)
                
                Rnf(t,:,i) = torus(i,:)-coil(t+1,:);
            else
                
                Rnf(t,:,i) = torus(i,:)-coil(1,:);
            end
            
        end
        
        
        
    end
    



end

