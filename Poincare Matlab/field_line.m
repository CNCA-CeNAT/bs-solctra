function field_line(start_point,steps,size_step,num_particle)%,coils,e,leng_segment)
%FIELD_LINE Created by Esteban Zamora Picado, version june 2013
%   This function computed the trajectory with the method Runge-Kutta oder 4 discribed 
%   in the paper 'Numerical Methods for Particle Tracing in Vector Fields'
%   by Kenneth I. Joy. This trajectories of a magnetic field line are generated 
%   by the arrangement of coils difined in the 'bobinas#.txt' files, 
%   and once complete the imposed steps "save" the trajectory in the "Results" Folder.
    global particless;
    
    field_line(1,:)=start_point;
    
    for i=2:steps

        %{
        p0=field_line(i-1,:);        
        K1=field_profile(p0);%,coils,e,leng_segment);
        K1=(K1.*size_step)./norm(K1);
        
        p1=field_line(i-1,:)+(K1./2);        
        K2=field_profile(p1);%,coils,e,leng_segment);        
        K2=(K2.*size_step)./norm(K2);
        
        p2=field_line(i-1,:)+(K2./2);        
        K3=field_profile(p2);%,coils,e,leng_segment);
        K3=(K3.*size_step)./norm(K3);
        
        p3= field_line(i-1,:)+(K3);      
        K4=field_profile(p3);%,coils,e,leng_segment);
        K4=(K4.*size_step)./norm(K4);
        
        field_line(i,:)=field_line(i-1,:)+((K1+(2*K2)+(2*K3)+K4)./3);
        %}
        
        p0=field_line(i-1,:);        
        K1=field_profile(p0);%,coils,e,leng_segment);
        k1(i,:)=K1;
        B= 'Results/B.txt';
        save(B,'k1', '-ASCII');
        K1=(K1./norm(K1)).*size_step;
        
        p1=field_line(i-1,:)+(K1./2);        
        K2=field_profile(p1);%,coils,e,leng_segment);        
        K2=(K2./norm(K2)).*size_step;
        
        p2=field_line(i-1,:)+(K2./2);        
        K3=field_profile(p2);%,coils,e,leng_segment);
        K3=(K3./norm(K3)).*size_step;
        
        p3= field_line(i-1,:)+(K3);      
        K4=field_profile(p3);%,coils,e,leng_segment);
        K4=(K4./norm(K4)).*size_step;
            
        field_line(i,:)=field_line(i-1,:)+((K1+(2*K2)+(2*K3)+K4)./6);
        
        %condition = wall_condition(0.2381,0.1004,field_line(i,:));
        %if condition==false
            %particle= sprintf('Results/particle%d.txt',num_particle);
            %save(particle,'field_line', '-ASCII');
            %break;
        %else
            progress=(i/steps)*100;
            if 0==mod(progress,2)
                clc;
                formatSpec = 'El progreso para la trayectoria #%d de %d, es:  %d%%\n';
                fprintf(formatSpec,num_particle,particless,progress);
            end
        %end    
        particle= sprintf('Results/particle%d.txt',num_particle);
        save(particle,'field_line', '-ASCII');
        
    end
    %particle= sprintf('Results/particle%d.txt',num_particle);
    %save(particle,'field_line', '-ASCII');
end

