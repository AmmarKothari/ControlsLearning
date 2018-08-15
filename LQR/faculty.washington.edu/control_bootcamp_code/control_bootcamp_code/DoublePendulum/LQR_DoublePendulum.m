% http://users.auth.gr/lazarosm/double-inverted-pendlum-laguerre-moysis.pdf
PendulumSetup_1;
upright = true;


if upright
    x0 = [pi; 0; 0; 0];
else
    x0 = [0; 0; 0; 0];
end
A = A_linearized(0,0,DP_model.b1,DP_model.b2,DP_model.g,DP_model.l1,DP_model.l2,DP_model.m1,DP_model.m2,x0(1),x0(3),x0(2),x0(4));
B = B_linearized(DP_model.l1,DP_model.l2,DP_model.m1,DP_model.m2,x0(3));

Q = eye(4,4);
R = .01*eye(2,2);

[K,S,e] = lqr(A,B, Q, R);

x = [pi*0.5; 0; 0; 0];
u = [0;0];
t = 0:0.01:10;
t_last = 0;
X = zeros(length(t), length(x));
U = zeros(length(t), length(u));
for i = 1:length(t)
    dt = t(i) - t_last;
    X(i, :) = x';
    
    % determine control input
    u_calc = -K*(x - x0);
    u_noise = rand(2,1);
    u = u_calc + u_noise;
    U(i, :) = u';
    if any(isnan(x)) || any(isnan(u))
        disp('blah');
    end
    
    % step system forward
    x_d = DoublePendulum_Dynamics(u(1), u(2),DP_actual.b1,DP_actual.b2,DP_actual.g,DP_actual.l1,DP_actual.l2,DP_actual.m1,DP_actual.m2, x(1), x(3), x(2), x(4));
    if any(isnan(x_d))
        disp('blah');
    end
    x = x + [x(2); x_d(1); x(4); x_d(2)]*dt;
    disp([x', u']);
    t_last = t(i);
end

SimulateDoublePendulum(t, X, DP_model.l1, DP_model.l2)
    
    
    

