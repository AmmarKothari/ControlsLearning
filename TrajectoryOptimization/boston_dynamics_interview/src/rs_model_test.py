import math

import pytest
from src import rs_model


@pytest.fixture()
def model():
    return rs_model.RSModel()


@pytest.fixture()
def zero_pos():
    return [0.0, 0.0, 0.0]


def test_step_straight(model, zero_pos):
    next_step = model.step(zero_pos, 0.0, 1.0)
    assert next_step == (1.0, 0.0, 0.0)


def test_step_straight_with_turn(model, zero_pos):
    next_step = model.step(zero_pos, 1.0, 1.0)
    assert next_step == (1.0, 0.0, 1.0)


def test_step_turn_with_turn(model, zero_pos):
    next_step = model.step(zero_pos, math.pi/8, 1.0)
    assert next_step[0] == 1.0 and next_step[1] == 0.0

    current_yaw = next_step[2]
    next_step = model.step(next_step, math.pi/8, 1.0)
    assert next_step[0] == 1.0 + math.cos(current_yaw) and next_step[1] == math.sin(current_yaw)
