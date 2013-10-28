function checkStaggering

%Build up MAC grid
leftBnd = -2;
rightBnd = 2;
lowerBnd = -2;
upperBnd = 2;

[ptsX, ptsY] = meshgrid2(leftBnd, rightBnd, lowerBnd, upperBnd);
[ptsUx, ptsUy] = meshgrid2(leftBnd - 0.5, rightBnd + 0.5, lowerBnd, upperBnd);
[ptsVx, ptsVy] = meshgrid2(leftBnd, rightBnd, lowerBnd - 0.5, upperBnd + 0.5);

figure;
hold on;
plot(ptsX, ptsY,'g+');
plot(ptsUx, ptsUy, 'b.');
plot(ptsVx, ptsVy, 'r.');

%Test departure point finding and interpolation point choice
%Arrival point
i = 0;
j = 0;

%Displacement (in gridcell units)
ax = -1.4;
ay = -0.3;

%PLAY AROUND FROM HERE
%Interpolation points for u
ip0_u = i + floor(ax + 0.5);
ip1_u = ip0_u + 1;
jp0_u = j + floor(ay);
jp1_u = jp0_u + 1;

%TAKE CARE OF THIS YOURSELF
%Account for staggering (only here for plotting)
ip0_u = ip0_u - 0.5;
ip1_u = ip1_u - 0.5;
% 
% jp0_v = jp0_v - 0.5;
% jp1_v = jp1_v - 0.5;

%Plot points
plot(i,j,'Marker','s','MarkerSize',10,'MarkerEdgeColor','k','LineWidth',2); %Arrival point
plot(i+ax, j+ay, 'Marker','s','MarkerSize',10,'MarkerEdgeColor','k','LineWidth',2); %Departure point
plot(ip0_u, jp0_u, 'Marker','h','MarkerSize',10,'MarkerEdgeColor','r','LineWidth',2);
plot(ip0_u, jp1_u, 'Marker','h','MarkerSize',10,'MarkerEdgeColor','r','LineWidth',2);
plot(ip1_u, jp0_u, 'Marker','h','MarkerSize',10,'MarkerEdgeColor','r','LineWidth',2);
plot(ip1_u, jp1_u, 'Marker','h','MarkerSize',10,'MarkerEdgeColor','r','LineWidth',2);
hold off;
end

function [x,y] = meshgrid2(x0, xEnd, y0, yEnd)
    xSeries = (x0:xEnd)';
    ySeries = y0:yEnd;
    x = repmat(xSeries,length(ySeries),1);
    y = repmat(ySeries,length(xSeries),1);
    y = y(:);
end