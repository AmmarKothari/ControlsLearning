 function [pwm, error, errorTotal] = controller(state_goal, state_current, errorTotal, errorLast)
    kP = 20;
    kI = 0;
    kD = 0;
    wkP = 0;
%     wkI = 0;
%     wkD = 0;
    if nargin < 3
        errorTotal = 0;
    end
    if nargin < 4
        errorLast = 0;
    end
    st_goal = state_track(state_goal);
    st_current = state_track(state_current);
    error = 0;
    error = error + sum(st_goal(1:2) - st_current(1:2)); %error in distance
    error = error + st_goal(3) - st_current(3);
    
    derror = error - errorLast;
    errorTotal = errorTotal + error;
    
    omega_error = st_goal(4) - st_current(4);
%     pwm_dif = state_goal(1) - state_current(1);
%     pwm_for = state_goal(2) - state_current(2);
    forwardPWM = kP * error + kI * errorTotal + kD * derror;
%     forwardPWM = 50;
    turnPWM = wkP * omega_error;
    forwardPWM = clamp(forwardPWM);
    turnPWM = clamp(turnPWM);
    if omega_error < 0
        pwmL = forwardPWM + turnPWM;
        pwmR = forwardPWM - turnPWM;
    elseif omega_error > 0
        pwmR = forwardPWM + turnPWM;
        pwmL = forwardPWM - turnPWM;
    else
        pwmR = forwardPWM;
        pwmL = forwardPWM;
    end
    pwm = [pwmR, pwmL];
end

function pwm_clamp = clamp(pwm_in)
    pwm_MAX = 90; pwm_MIN = 0;
    if pwm_in >= 0
        pwm_clamp = max(min(pwm_in, pwm_MAX),pwm_MIN);
    elseif pwm_in < 0
        pwm_clamp = -max(min(-pwm_in, pwm_MAX),pwm_MIN);
    end
end