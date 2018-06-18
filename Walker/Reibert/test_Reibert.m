R = Reibert();
f = figure(1);
f2 = figure(2);
ax2_1 = subplot(4,1,1);
ax2_2 = subplot(4,1,2);
ax2_3 = subplot(4,1,3);
ax2_4 = subplot(4,1,4);
R.theta = 0.0;
R.x = R.x + [0; 01]; % increase starting height
R.xd = [0.1; 0];
Hdes = 0.05 + R.l0;
Kp = 10*Hdes;
Ki = Kp/10;
u_F_max = 50;
e_total = 0;
D = ReibertDraw(f);
for i = 1:5000
    if R.leg_contact == 1
        E_needed = R.mass*R.g*(Hdes);
        KE = 1/2*R.mass*R.xd(2)^2;
        PE = 1/2*R.leg_k*(R.l0 - R.l)^2 + R.mass*R.g*(R.l*cos(R.theta));
        E_current = KE + PE;
        error = (E_needed - E_current);
        e_total = e_total + error;
%         u_F = (R.l0 - R.l)^-1 * (E_needed - E_current);
%          
        u_F = Kp * error + Ki * e_total;
        u_F = u_F/abs(u_F) * u_F_max; 
        if R.xd(2) >= 0
            u_dir = 1; % lengthen
        else
            u_dir = -1; % compress
        end
        u_F = u_dir * u_F;
    else
        u_F = 0;
    end
%     u = [u_F;0];
%     u = [0;sin(i*R.dt_sim)];
    u = [0;0];
%     R.theta = -1^i * pi/4;
    R = R.step(u);
    if rem(i,10) == 0
        D.draw(R, Hdes)
        
    end
%     if i > 350
%         blah = 1;
%     end
%     pause(0.1)

    % stop if crashed
    if R.x(2) < 0.01
        break
    end
    
    
end

plot(ax2_1, 1:10:length(R.x_all), R.x_all(1:10:end,2), 'rx', 1:10:length(R.x_all), Hdes, 'bo-')
plot(ax2_2, R.u_all(:,1))
plot(ax2_3, 1:10:length(R.x_all), R.x_all(1:10:end,1), 'bx')
plot(ax2_4, 1:10:length(R.theta_all), R.theta_all(1:10:end,1), 'bx')