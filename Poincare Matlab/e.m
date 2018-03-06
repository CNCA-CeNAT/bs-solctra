% Calcula todos los vectores unitarios de los segmentos del filamento de
% bobina

function [vector_segment,mid_pos]=e(bobina0)


    for i = 1 : size(bobina0,1)-1
        
           vector_segment(i,:) = (bobina0(i+1,:)- bobina0(i,:));
            
           leng_segment(i,1) = norm(vector_segment(i,:));         
            
           mid_pos(i,:)= (bobina0(i+1,:)+(bobina0(i,:)/2));
           
           
           %e_roofs(i,:) = vector_segment(i,:)/norm(vector_segment(i,:));
        
        
        %if i < s(1,1)
            
         %   vector_segment(i,:) = (bobina0(i+1,:)- bobina0(i,:));
            
          % leng_segment(i,1) = norm(vector_segment(i,:));         
            
           % e_roofs(i,:) = vector_segment(i,:)/norm(vector_segment(i,:))
            
        
        %else
            
         %   vector_segment(i,:) = (bobina0(1,:)- bobina0(i,:));
            
          %  leng_segment(i,1) = norm(vector_segment(i,:));
            
           % e_roofs(i,:) = vector_segment(i,:)/norm(vector_segment(i,:))    
            
       % end
        
        
    end;

        %r= line-mid_pos;

end