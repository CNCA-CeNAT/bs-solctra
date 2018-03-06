function [ spherical_point ] = rectangular_to_spherical(pointXYZ)
%RECTANGULAR_TO_SPHERICAL Summary of this function goes here
%   Detailed explanation goes here
%   Coordinates conversion
    r=sqrt(pointXYZ(1)^2+pointXYZ(2)^2+pointXYZ(3)^2);
    theta=acos(pointXYZ(3)/r);
    phi=atan(pointXYZ(2)/pointXYZ(1));
    
    spherical_point(1)=r; 
    spherical_point(2)=theta; 
    spherical_point(3)=phi; 

end

