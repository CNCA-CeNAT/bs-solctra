function [ process] = wall_condition(R,r,point)
%WALL_CONDITION Created by Esteban Zamora, version june 2013
%   This function defined the boundary conditions related to vessel demensions 
%   and enable to set the end o contact of field line with the vessel wall. 
    % position related with cut palne 
    vector1=([point(1,1),point(1,2)]./norm([point(1,1),point(1,2)]))*R;
    
    %difinite respect to poloidal plane that contains the  point in
    %question(this analysis regard to xy plane by the poloidal origen are in z=0)
    vector1(1,3)=0;
    r1=norm(point-vector1);
    if r1<r
        process = true;
    else
        process = false;
    end    
end

