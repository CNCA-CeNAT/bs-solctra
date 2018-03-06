function resonancei( beta )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

    global coils e leng_segment;
    coils = load_data();
    [e,leng_segment]=e_roof(coils);
    
    x = 0.2381-0.0944165:0.005:0.2381+0.0944165;

    z = -0.094416:0.005:0.094416;



    [X,Z] = meshgrid(x,z);
    Y = zeros(size(X));
    B = zeros(size(X));
    for i=1:size(X)
        for j=1:size(X)
            B0=[0 0 0];
            parfor k = 1 : 12         
                [Rim,Rfm] = R_vectors(coils{k},[X(i,j) Y(i,j) Z(i,j)]);       
                B0=B0+magnetic_field(e(:,:,k),Rim,Rfm,leng_segment(:,k));         
            end
            B(i,j) = norm(B0);

        end
        progress=(i/size(x,2))*100;
            %if 0==mod(progress,2)
                clc;
                formatSpec = 'El progreso para la trayectoria %d%%\n';
                fprintf(formatSpec,progress);
            %end
    end
     
    savefile1 = 'B.mat';
    savefile2 = 'X.mat';
    savefile3 = 'Y.mat';
    savefile4 = 'Z.mat';
    save(savefile1, 'B');
    save(savefile2, 'X');
    save(savefile3, 'Y');
    save(savefile4, 'Z');




end

