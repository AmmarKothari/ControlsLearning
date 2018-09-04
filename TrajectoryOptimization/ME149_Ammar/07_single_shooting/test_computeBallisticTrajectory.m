function test_computeBallisticTrajectory()

run('../../ME149_Spring2018/codeLibrary/addLibraryToPath.m')
%%
param = getBallisticParameters();
method = 'rk4';
nlpOpt = optimset('fmincon');
for nGrid = [10, 50, 100, 500, 1000]
    tic;
    nlpOpt.Display = 'off';
    soln = computeBallisticTrajectory(param, nGrid, method, nlpOpt);
    t = toc;
    dispStats(soln, nGrid, param, t);
end

for drag = [0.01, 0.05, 0.1, 1, 10]
    param.drag = drag;
    nGrid = 50;
    tic;
    nlpOpt.Display = 'off';
    soln = computeBallisticTrajectory(param, nGrid, method, nlpOpt);
    t = toc;
    dispStats(soln, nGrid, param, t);
    figure();
    plotSolution(param, soln)
    
end


end


function plotSolution(param, soln)
    scatter3(soln.zGrid(1,:), soln.zGrid(2,:), soln.zGrid(3,:), 'bo');
    hold on;
    scatter3(param.start(1), param.start(2), param.start(3), 'r*')
    scatter3(param.target(1), param.target(2), param.target(3), 'kx')
    hold off;

end


function dispStats(soln, nGrid, param, t)
    fprintf('====================\n');
    fprintf('Final Accuracy: %0.2f \n', norm(soln.zGrid(1:3, end) - param.target));
    fprintf('Solution: %0.2f %0.2f %0.2f \n', soln.zGrid(1:3, end));
    fprintf('Grid Points: %0.0f \n', nGrid);
    fprintf('Drag: %.2f \n', param.drag);
    fprintf('Run Time: %0.2f \n', t);
    fprintf('Iterations: %0.0f \n', soln.output.iterations);
    fprintf('Function Calls: %0.0f \n', soln.output.funcCount)
    fprintf('====================\n');
end