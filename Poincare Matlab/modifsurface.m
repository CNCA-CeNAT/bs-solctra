function D=modifsurface()
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

   
    A=load_data_surface(48);
    for i=1:48
        B=A{1,i};
        h=1;
        for j=1:3:size(B,1)
            C(h,:)=B(j,:);
            h=h+1;
        end
        D{:,i}=C(:,:);
        
    end
    


end

