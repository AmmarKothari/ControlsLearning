def path_integral(controls):
    return sum([sum([x**2 for x in control]) for control in controls])