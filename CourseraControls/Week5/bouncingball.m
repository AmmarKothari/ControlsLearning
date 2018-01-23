

function bouncingball()
h = 1;
g = -9.8;
k = 0.95; %loss of energy due to hitting the table

dt = 0;
t_last = 0;
h_all = [];
v_all = [];
v = 0; %initial value
switches = 0;
for t = 0:0.01:1.1e1
    dt = t - t_last;
    if h <= 0
        h = 0;
        v = -k*v;
        switches = switches + 1;
    end
    v = v + g * dt;
    h = h + v * dt;
    t_last = t;
    h_all = [h_all; h];
    v_all = [v_all; v];
end


plot(h_all)
figure(2)
plot(v_all, '*')


end