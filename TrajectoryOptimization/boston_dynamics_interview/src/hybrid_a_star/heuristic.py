import math


def simple_heuristic(current_position, goal_position):
    """Use the continuous positions here."""
    return math.hypot(current_position[0] - goal_position[0], current_position[1] - goal_position[1])
