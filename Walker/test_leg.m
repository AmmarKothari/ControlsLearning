%% Test out leg configurations
a_upper = [0, 0, 1];
h_upper = [1, 0, 0];
a_lower = [0, 0, 1];
h_lower = [1, 0, 0];

upper = link(a_upper, h_upper, h_upper);
upper_draw = draw_link('b');
lower = link(a_lower, h_lower, h_lower);
lower_draw = draw_link('g');


upper = upper.setProximal([0,0,0]);
upper = upper.linkPosForward(0);
lower = lower.setProximal(upper.distal);
lower = lower.linkPosForward(pi/2);
f = figure(1);
a = axes(f);
upper_draw.drawLink(upper, a);
lower_draw.drawLink(lower, a);