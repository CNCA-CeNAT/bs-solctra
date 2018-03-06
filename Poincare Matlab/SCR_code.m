function SCR_code(point1, point2,particles,steps,step_size)
%SCR_CODE Created by Esteban Zamora Picado, version june 2013
%This code is a effort of Esteban Zamora for create a application capable
%of to compute trajectories or magnetic field lines for the device called
%SCR-1 (Stellarator of Costa Rica 1). The magnetics field is generate by a
%arrangement of 12 helical coils, that we modeling like filaments,The
%field lines have a complex denfined shape (if you regard the field like stationary), 
%and this field lines are estimated through of the Runge_Kutta order 4
%method, of course the code enable to calculate the field lines for any
%coils configuration.SCR_code([1.9392400e-01 0 0],[3.1440400e-1 0 0],2,1000000,0.0001)

    %Global definitions
    global coils e leng_segment particless;
    %Global definitions Asignement
    coils = load_data();
    [e,leng_segment]=e_roof(coils);
    particless=particles;
    
    %Dicretization of start positions a long of cut line generated from 
    %POINTS1 and POINTS2
    %start_positions= cut_line(point1,point2,particles);
    start_positions=[0.5,0.05,0];
    
    %Loop for all particles
    i=1;
    for i=1:particles 
                
        field_line(start_positions(i,:),steps,step_size,i);%,coils,e,leng_segment);
    end

end

