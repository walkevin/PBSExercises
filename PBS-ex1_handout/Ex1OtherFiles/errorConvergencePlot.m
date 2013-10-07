function errorConvergencePlot(filename,damp)

A = importdata(filename,' ',22);

ref         = A.data(:,6);
timesteps   = A.data(:,1);
methodNames = A.colheaders(2:5);
methodData  = A.data(:,(2:5));

tmp = size(methodNames);
nMethods = tmp(2);

% methodErrors = 1 - (abs(methodData - repmat(ref,1,nMethods)) ./ repmat(ref,1,nMethods));
methodErrors = abs(methodData - repmat(ref,1,nMethods));
methodErrorConvergence = methodErrors((2:end),:) ./ methodErrors((1:end-1),:);
avg = mean(methodErrorConvergence((3:end),:),1);

loglog(repmat(timesteps,1,nMethods),methodErrors,'+-');
ylabel('Error');
xlabel('Timestep');
title(['Error convergence for damping factor d=',num2str(damp)]);
legend(['Euler: ', num2str(avg(1))], ['Symplectic Euler: ', num2str(avg(2))], ['Midpoint: ', num2str(avg(3))],['Backward Euler: ', num2str(avg(4))]);
set(gca,'xdir','rev')
end