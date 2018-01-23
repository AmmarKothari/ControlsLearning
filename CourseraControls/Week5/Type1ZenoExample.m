f1 = @(x) 6 - x;
f2 = @(x) 5*x;
g  = @(x) 1;

epsilon = 0.01;
x_switch = 10;
x = 11;
x_all = [];
dt = 0;
t_last = 0;
x_dot = 0;
for t = 1:0.0001:1e1
    dt = t - t_last;
    if abs(x - x_switch) < epsilon
        x_dot = g(x);
    elseif x < x_switch
        x_dot = f2(x);
    elseif x > x_switch
        x_dot = f1(x);
    end
    x = x + dt * x_dot;
    t_last = t;
    x_all = [x_all; x];
end



plot(x_all)

