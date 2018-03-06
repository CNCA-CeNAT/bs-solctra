function [B]=field_profile(point)%,coils, e, leng_segment)%[line,B]=field_profile()
%FIELD_PROFILE Created by Esteban Zamora Picado, version june 2013
% This function compute the magnetic field value (B) at point position by
% means of ecuation (8) proposed in "Compact expressions for the Biot-Savart 
%fields of a filamentary segment" by James D. Hanson (Physics Department, 
%Auburn University, Auburn, Alabama) and Steven P. Hirshman (Oak Ridge  
%National Laboratory, Oak Ridge, Tennessee)

    global  e leng_segment coils;  
    
    B=[0 0 0];
    parfor k = 1 : 12         
        [Rim,Rfm] = R_vectors(coils{k},point);       
        B=B+magnetic_field(e(:,:,k),Rim,Rfm,leng_segment(:,k));         
    end
    
end