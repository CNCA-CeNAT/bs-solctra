function [ cutline ] = cut_line( p1,p2,step)
%CUT_LINE: Created by Esteban Zamora Picado, version june 2013
%   This function creates a discretized line from two point P1 and P2 
    global particless;
    vect_dir=(p2-p1)/norm(p2-p1);
    num_steps=norm(p2-p1)/particless;
    cutline(1,:)=p1;
    j=2;
    cutline(1,:)=p1;    
    for i=1:particless       
        cutline(i,:)=p1+(vect_dir*i*num_steps);
    end
end

