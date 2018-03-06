
function B = magnetic_field(e_roof,Ri,Rf,leng_segment)
% MAGNETIC_FIELD Created by Esteban Zamora Picado, version june 2013
%This function implement the ecuation (8) proposed in "Compact expressions for the Biot-Savart 
%fields of a filamentary segment" by James D. Hanson (Physics Department, 
%Auburn University, Auburn, Alabama) and Steven P. Hirshman (Oak Ridge  
%National Laboratory, Oak Ridge, Tennessee)
    miu = 4*pi*(1e-7);    
    I= -4350;   
    B= zeros(1,3);        
    for i= 1: 1: size(leng_segment)      
        B=B+cross((e_roof(i,:).*((miu*I)./(4*pi))),((Ri(i,:)).*(((2*leng_segment(i)*(norm(Ri(i,:))+norm(Rf(i,:))))/(norm(Ri(i,:))*norm(Rf(i,:))))*((1/(((norm(Ri(i,:))+norm(Rf(i,:)))^2)-((leng_segment(i))^2)))))));
    end  
end