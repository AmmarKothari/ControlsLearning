
import numpy as np
import scipy.optimize

from src.models import projectile


def test_dynamics():
    params = projectile.Params()
    dz = projectile.dynamics([np.array([0, 0, 0, 0, 0, 0])], params)
    assert dz[-1] == -9.8


def test_run_simulation():
    params = projectile.Params()
    assert projectile.run_simulation(10.0, [0.0, 0.0, 10.0], params) is not None


def test_get_end_state_defect():
    dec_vars = [10, 0.0, 0.0, 10.0]
    params = projectile.Params()
    defect = projectile.get_end_state_defect(dec_vars, params, 0)
    assert defect == params.target[0]


def test_single_shooting_for_ballistic():
    def obj_func(x):
        _, initial_vel = projectile.unpack_dec_vars(x)
        cost = sum(np.square(np.array(initial_vel)))
        return cost
    params = projectile.Params()
    guess = projectile.Guess()
    initial_guess = projectile.pack_dec_vars(guess.total_time, np.array(guess.initial_vel).T)
    constraints = []
    for i_state in range(3):
        constraints.append({'type': 'eq', 'fun': lambda x: projectile.get_end_state_defect(x, params, i_state)})
    solution = scipy.optimize.minimize(obj_func, initial_guess, constraints=constraints)
    import pdb; pdb.set_trace()
