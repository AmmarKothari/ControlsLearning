import pytest
from src.di_2d_system import di_2d_model


@pytest.fixture(params=list(di_2d_model.ObjectiveTypes))
def objective(request):
    return request.param

def test_simple(objective):
    model = di_2d_model.DI2D(100)
    model.setup_control_constraints([1, 1], [-1, -1])
    model.setup_start_constraints([0, 0, 0, 0], [0, 0])
    model.setup_end_constraints([10, 10, 0, 0])
    model.constrain_time(10)
    model.setup_solve()
    model.set_objective(objective)
    model.solve()
    # model.plot_solution()


