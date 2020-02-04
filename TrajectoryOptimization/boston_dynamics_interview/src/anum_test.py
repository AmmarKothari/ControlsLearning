import pytest
from src import anum


@pytest.fixture
def positive():
    return 10.0


@pytest.fixture
def negative():
    return -10.0


@pytest.fixture
def zero():
    return 0.0

def test_one_when_positive(positive):
    assert anum.sign(positive) == 1

def test_neg_one_when_negative(negative):
    assert anum.sign(negative) == -1

def test_zero_when_zero(zero):
    assert anum.sign(zero) == 0


