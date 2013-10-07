function stabilityAnalysisPlot(filename)

A = importdata(filename,' ',10);

timesteps   = A.data(:,1);
ref         = A.data(:,6);
methodNames = A.colheaders(2:5);
methodData  = A.data(:,(2:5));

tmp = size(methodNames);
nMethods = tmp(2);

ymax = 1.5*max(ref);

color = 'rgbc';
h=figure;
for i=1:nMethods
    plot(timesteps,methodData(:,i),'+-','color',color(i));
    hold on;
end
hold off;
ylim([0 ymax]);
ylabel('Maximal amplitude');
xlabel('Timestep');
legend('Euler','Symplectic Euler','Midpoint','Backward Euler');
[pathstr,name,ext] = fileparts(filename);
saveas(h,name,'pdf');
end