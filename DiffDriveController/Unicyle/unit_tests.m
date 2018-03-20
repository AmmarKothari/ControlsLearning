%% test dynamics with constant velocity and omega
statep_in = [0, 0, 0];
stated_in = [1, 0.9];
state_all = [];
dt = 0.1;
t = 10;
time = 1:dt:t;
for i = 1:length(time)
    statep_in = dynamics(statep_in, stated_in, dt);
    state_all = [state_all; statep_in];
end
subplot(2,1,1)
plot(state_all(:,1), state_all(:,2), 'ro')
axis('equal')
subplot(2,1,2)
plot(time, state_all(:,3), 'g*')

%% control over simple dynamics
wd = pi;
vd = 10;
des = [vd, wd];
stated_in = [0, 0];
stated_all = [];
while sum(abs(stated_in - des)) > 1
    stated_change = controller(stated_in, des);
    stated_all = [stated_all; stated_in];
    plot(stated_all(end,1), stated_all(end,2), 'bx', des(1), des(2),'go');
    pause(0.0001);
end
% plot(stated_all(:,1), stated_all(:,2), 'bx', des(1), des(2),'go');
