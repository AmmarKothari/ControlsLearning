%%
F = @(x) x(1)^2 + x(2)^2;
A_eq = [1 1];
b_eq = [-2];
eq_const = @(x1,x2) x1+x2+2;

x = linspace(-2,2,100);
y = linspace(-2,2,100);

[X, Y] = meshgrid(x,y);

x_sol = fmincon(F, [0,0], [], [], A_eq, b_eq);

%%
F = @(x) x(1)^2 + x(1)*x(2) + x(2)^2;
A_ineq = [0 -1; 1 1];
b_ineq = [-1, 3];


x = linspace(-2,2,100);
y = linspace(-2,2,100);

L = @(x, lambda) F(x) + lambda(1)*(1-x(2)) + lambda(2)*(x(1)+x(2)-3);
dL_dx = @(x,lambda) 2*x(1)+x(2)+lambda(2);


% both constraints not active
lambda = [0,0];



% case 4
% b = [x1 x2 lambda1 lambda2]
syms x1 x2 lambda_1 lambda_2
S = solve(2*x1+x2+lambda_2 == 0, ...
            x1 + 2*x2 - lambda_1 + lambda_2 == 0, ...
            lambda_1 * (1-x2) == 0, ...
            lambda_2 * (x1 +x2 - 3) == 0)
        
        
for i = 1:length(S.x1)
    fprintf("Constriaints: %s, %s, States: %s, %s, Cost: %s\n",...
        S.lambda_1(i), S.lambda_2(i), S.x1(i), S.x2(i), F([S.x1(i), S.x2(i)]));
end
        

