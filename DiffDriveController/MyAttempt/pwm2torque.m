function [aL, aR] = pwm2torque(pwm)
    p2t_const = 0.1;
    aL = pwm(1) * p2t_const;
    aR = pwm(2) * p2t_const;
end