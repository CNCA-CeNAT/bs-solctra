
function [e_roofs, leng_segment]=e_roof(coils)
% E_ROOF Created by Esteban Zamora Picado, version june 2013
%This function enable to compute the unit vectors for every points of such 
% coil, in relation with ecuation 8 in the paper "Compact expressions for 
%the Biot-Savart %fields of afilamentary segment" by James D. Hanson et al.  
    numb_coils=size(coils,2);
    numb_elements= size(coils{1},1);
    for j = 1:numb_coils
        coil=coils{j};
        for i = 1 : numb_elements-1
           vector_segment(i,:,j) = (coil(i+1,:)- coil(i,:));            
           leng_segment(i,:,j) = norm(vector_segment(i,:,j));            
           e_roofs(i,:,j) = vector_segment(i,:,j)/norm(vector_segment(i,:,j));        
        end
    end
end