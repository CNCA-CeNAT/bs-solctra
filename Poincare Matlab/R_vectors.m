function [Rni,Rnf] = R_vectors(coil,point_x)
% R_VECTORS Created by Esteban Zamora Picado, version june 2013
% The function compute Rni: vector from the initial point of the segment to
% the observation point; and Rnf: vector from segment end point to the
% observation point. Conform to the ecuation (8) of "Compact expressions for the Biot-Savart 
%fields of a filamentary segment" paper.    
           
        
    for t = 1 : size(coil,1)-1 %loop for each coil segment. 
        Rni(t,:) = point_x-coil(t,:);
        Rnf(t,:) = point_x-coil(t+1,:);    
    end  
    
end
