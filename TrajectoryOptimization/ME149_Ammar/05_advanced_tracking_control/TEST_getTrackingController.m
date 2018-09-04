function  TEST_getTrackingController()

run('../../ME149_Spring2018/codeLibrary/addLibraryToPath.m')
%%
%       .m = mass of the quadrotor
%       .w = distance between the rotors (width)
%       .g = gravity acceleration

[ref, param] = importReferenceTrajectory('quadrotorOptimalFlipData.csv', 'quadrotorOptimalFlipParam.csv');

fprintf('Test 1 \n');
z0 = ppval(ref.state, 0); % state at t=0
run_test(z0, ref, param, false);

fprintf('Test 2 \n');
planarQuadrotorRealDyn()
z0 = ppval(ref.state, 0); % state at t=0
run_test(z0, ref, param, false);

fprintf('Test 3 \n');
planarQuadrotorRealDyn(100)
z0 = ppval(ref.state, 0); % state at t=0
run_test(z0, ref, param, false);

fprintf('Test 4 \n');
planarQuadrotorRealDyn(1000)
z0 = ppval(ref.state, 0); % state at t=0
run_test(z0, ref, param, false);

fprintf('Test 5 \n');
errModel.xDist = 0.04;
errModel.yDist = 0.04;
errModel.qDist = 0.04;
errModel.nTerm = 5;
errModel.freqBnd = [0.1, 10];  % Hz
planarQuadrotorRealDyn(1000,errModel)
z0 = ppval(ref.state, 0); % state at t=0
run_test(z0, ref, param, false);


end

function run_test(z0, ref, param, showPlot)

    trackingController = getTrackingController(ref, param);
    
    t_start = ref.time(1);
    t_step = 0.01;
    t_final = ref.time(end);

    sim_func = @(t,z) planarQuadroto
    fprintf('Test 3 \n');
planarQuadrotorRealDyn(1000)
z0 = ppval(ref.state, 0); % state at t=0
run_test(z0, ref, param, false);rRealDyn(t, z, trackingController(t,z), param);

    sol = ode45(sim_func, [t_start t_final], z0);
    Z = deval(sol, ref.time);
    u = zeros(ref.control.dim, length(ref.time));
    for i = 1:length(ref.time)
        u(:,i) = trackingController(ref.time(i), Z(:,i));
    end

    Z_des = ppval(ref.state, ref.time);
    AvgStateError = mean(Z_des - Z, 2);
    NormAvgStateError = norm(AvgStateError);
% 
    fprintf('Parameters: w - %0.2f, m - %0.2f, g - %0.2f \n', param.w, param.m, param.g)
%     fprintf('Start State: [%0.2f,%0.2f, %0.2f, %0.2f, %0.2f, %0.2f] \n', z0);
%     fprintf('End State:   [%0.2f,%0.2f, %0.2f, %0.2f, %0.2f, %0.2f] \n', hover_des, zeros(1,4));
    fprintf('Avg State Error: %0.3f\n\n', NormAvgStateError)
    if showPlot
        figure()
        planarQuadrotorPlot(ref.time, Z, u, param, true)
    end


end
% planarQuadrotorPlot(T, Y', u, param, true)

% planarQuadrotorRealDyn
% planarQuadrotorAnimate
% planarQuadrotorPlot