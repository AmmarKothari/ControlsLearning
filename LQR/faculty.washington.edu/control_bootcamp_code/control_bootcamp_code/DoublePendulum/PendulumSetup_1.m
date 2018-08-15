
DP_model.l1 = 1; % meters
DP_model.l2 = 1; % meters
DP_model.g = 9.8; %meters/s^2
DP_model.m1 = 1; %kg
DP_model.m2 = 1; %kg
DP_model.b1 = 0.1; % kg/s
DP_model.b2 = 0.1; % kg/s

rng(1);
DP_actual.l1 = DP_model.l1 * (1 + rand(1)); % meters
DP_actual.l2 = DP_model.l2 * (1 + rand(1)); % meters
DP_actual.g = 9.8; %meters/s^2
DP_actual.m1 = DP_model.m1 * (1 + rand(1)); %kg
DP_actual.m2 = DP_model.m1 * (1 + rand(1)); %kg

DP_actual.b1 = DP_model.b1 * (1 + rand(1)); % kg/s
DP_actual.b2 = DP_model.b2 * (1 + rand(1)); % kg/s

