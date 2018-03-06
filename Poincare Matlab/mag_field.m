    function [B]=mag_field(point)
    
    %coils = load_data();    
    %line = cut_line([0.0948230 0.0948230 0],[0.2419012  0.2419012 0],0.01);
    %line = cut_line([-0.09736 -0.09736 0],[-0.23935 -0.23935 0],0.005);
    %line = cut_line([0.1377 0 0],[0.3385 0 0],0.005);
    %line = cut_line([0.2381 0 -0.1004],[0.2381 0 0.1004],0.005);
    %line = cut_line([0 0.2381 -0.1004],[0 0.2381 0.1004],0.005);
    %results_cell= cell(1,1,12); 
    %results_cell{1,1,13}=torus;
    
    for k = 1 : 12        
        
        %coil_name = sprintf('Bobina%d.txt', k);
        
        [e,leng_segment]=e_roof(coils{k});    
        Rim = Ri(coils{k},point);    
        Rfm = Rf(coils{k},point);    
        B_cell(:,:,k) = magnetic_field(e(:,:,k),Rim,Rfm,leng_segment(:,:,k));
        
    end
    
    B = B_cell(:,:,1);    
    
    for i = 2 : 12        
        B=B+B_cell(:,:,i);         
    end    

end