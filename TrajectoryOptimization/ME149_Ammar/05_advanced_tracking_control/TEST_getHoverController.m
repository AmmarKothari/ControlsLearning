function  TEST_getHoverController()

run('../../ME149_Spring2018/codeLibrary/addLibraryToPath.m')
%%
%       .m = mass of the quadrotor
%       .w = distance between the rotors (width)
%       .g = gravity acceleration
param.m = 0.4;
param.w = 0.4;
param.g = 10;

fprintf('Test 1 \n');
z0 = [0; 0; 0; 0; 0; 0];
hover_des = [0,0];
run_test(z0, hover_des, param, false);

fprintf('Test 2 \n');
z0 = [0; 0; 0; 0; 0; 0];
hover_des = [5,2];
run_test(z0, hover_des, param, false);

fprintf('Test 3 \n');
z0 = -1*ones(1,6);
hover_des = [5,2];
run_test(z0, hover_des, param, false);

fprintf('Test 4 \n');
param.m = 0.1;
param.w = 1.0;
z0 = -1*ones(1,6);
hover_des = [5,2];
run_test(z0, hover_des, param, true);

end

function run_test(z0, hover_des, param, showPlot)

    hoverController = getHoverController(hover_des(1), hover_des(2), param);

    t_step = 0.01;
    t_final = 5;

    sim_func = @(t,z) planarQuadrotorRealDyn(t, z, hoverController(z), param);

    [T,Y] = ode45(sim_func, 0:t_step:t_final, z0);
    u = hoverController(Y');

    q_end = mean(Y(end-50:end,:));
    SteadyStateError = norm(q_end(1:2) - hover_des);

    fprintf('Parameters: w - %0.2f, m - %0.2f, g - %0.2f \n', param.w, param.m, param.g)
    fprintf('Start State: [%0.2f,%0.2f, %0.2f, %0.2f, %0.2f, %0.2f] \n', z0);
    fprintf('End State:   [%0.2f,%0.2f, %0.2f, %0.2f, %0.2f, %0.2f] \n', hover_des, zeros(1,4));
    fprintf('Steady State Error: %0.3f\n\n', SteadyStateError)
    if showPlot
        figure()
        planarQuadrotorPlot(T, Y', u, param, false)
    end


end
% planarQuadrotorPlot(T, Y', u, param, true)

% planarQuadrotorRealDyn
% planarQuadrotorAnimate
% planarQuadrotorPlot