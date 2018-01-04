function drawPhasePortrait(f)


    y1 = linspace(-2,8,20);
    y2 = linspace(-2,2,20);
    % creates two matrices one for all the x-values on the grid, and one for
    % all the y-values on the grid. Note that x and y are matrices of the same
    % size and shape, in this case 20 rows and 20 columns
    [x,y] = meshgrid(y1,y2);

    u = zeros(size(x));
    v = zeros(size(x));

    % we can use a single loop over each element to compute the derivatives at
    % each point (y1, y2)
    t=0; % we want the derivatives at each point at t=0, i.e. the starting time
    for i = 1:numel(x)
        Yprime = f(t,[x(i); y(i)]);
        u(i) = Yprime(1);
        v(i) = Yprime(2);
    end
    % normalize arrow size
    % for i = 1:numel(x)
    % Vmod = sqrt(u(i)^2 + v(i)^2);
    % u(i) = u(i)/Vmod;
    % v(i) = v(i)/Vmod;
    % end

    % plot vector field
    figure()
    quiver(x,y,u,v,'r'); 
    xlabel('y_1')
    ylabel('y_2')
    axis tight equal;

    % plotting solutions on the vector field
    % pendulum always start at zero position but has different angular
    % momentums for each initial condition
    hold on
    for y20 = [0 0.25 0.35 0.45 0.5 1 1.5 2 2.5]
        [ts,ys] = ode45(f,[0,50],[0;y20]);
        plot(ys(:,1),ys(:,2))
        plot(ys(1,1),ys(1,2),'bo') % starting point
        plot(ys(end,1),ys(end,2),'ks') % ending point
    end
    xlim([-2, 8])
    ylim([-4, 4])
    hold off


end