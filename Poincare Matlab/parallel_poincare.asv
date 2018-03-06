function parallel_poincare(t)
%PARALLEL_POINCARE Summary of this function goes here
%   Detailed explanation goes here
            Beta=0;
            poincare_cell{t}=poincare_section(Beta,t); 
            surface=poincare_cell{t};%particle};
            file= sprintf('Resultsmt/Surface_%d.txt',t);
            save(file,'surface', '-ASCII');
end


