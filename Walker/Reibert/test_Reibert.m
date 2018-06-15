R = Reibert();
f = figure(1);
f2 = figure(2);
ax2 = gca(f2);
R.theta = 0;
R.x = R.x + [0; 0.1]; % increase starting height
Hdes = 0.3 + R.l0;
D = ReibertDraw(f);
for i = 1:5000
    if R.leg_contact == 1 && R.xd(2) > 0
        E_needed = R.mass*R.g*(Hdes);
        KE = 1/2*R.mass*R.xd(2)^2;
        PE = 1/2*R.leg_k*(R.l - R.l0)^2;
        E_current = KE + PE;
        u_F = 1/2*(R.l0 - R.l)^-1 * (E_needed - E_current);
%         u_F = (E_needed - E_current);
    else
        u_F = 0;
    end
    u = [u_F;0];
%     R.theta = -1^i * pi/4;
    R = R.step(u);
    if rem(i,10) == 0
        D.draw(R, Hdes)
        
    end
%     pause(0.1)
    
    
end

plot(ax2, 1:10:length(R.x_all), R.x_all(1:10:end,2), 'rx', 1:10:length(R.x_all), Hdes, 'bo-')