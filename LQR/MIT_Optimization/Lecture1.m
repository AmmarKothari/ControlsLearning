F = @(x1, x2) 100*(x1.^2 - x2).^2 + (1-x1).^2;

x = linspace(-3,3,100);
y = linspace(-3,3,100);

[X,Y] = meshgrid(x,y);

fs = F(X,Y);