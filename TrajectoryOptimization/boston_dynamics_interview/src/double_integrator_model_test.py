import pytest
from src import double_integrator_model
import numpy as np

@pytest.fixture
def ds():
    return double_integrator_model.DoubleIntegrator()

def test_step(ds):
    next = ds.step([0.0, 0.0, 0.0, 0.0], [1.0, 1.0], 1.0)
    assert np.array_equal([0.5, 0.5, 1.0, 1.0], next)

def test_multiple_steps(ds):
    next = [0.0, 0.0, 0.0, 0.0]
    steps = 10
    dt = 1.0 /steps
    for acceleration in np.full(steps, 1.0):
        next = ds.step(next, [acceleration, 0.0], dt)
    assert np.isclose(next[0], 0.5) and np.isclose(next[1], 0.0)
