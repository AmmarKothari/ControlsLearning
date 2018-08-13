from waypoint import Waypoint
import Queue
import numpy as np

DEBUG = False

def get_neighbors(wp, grid):
    '''
    Return a list of neighbors to current state as Waypoints based on size of grid, movement model, and obstacles

    :param curWP: Waypoint of the current state
    :param grid: Grid is a 2D numpy ndarray of boolean values. grid[x, y] == True if the cell contains an obstacle.
        The grid dimensions are exposed via grid.shape

    :rtype: A list of Waypoints that represent neibhbors

    Notes:
    # could do this through a set of transformation matrices
    '''
    if wp.orientation == 0:
        actions = np.array(
                [   [0,1,0],
                    [0,-1,0],
                    [1,1,1],
                    [-1,1,-1],
                    [-1,-1,1],
                    [1,-1,-1] ])
    elif wp.orientation == 1:
        actions = np.array(
                [   [1,0,0],
                    [-1,0,0],
                    [1,-1,1],
                    [1,1,-1],
                    [-1,1,1],
                    [-1,-1,-1] ])
    elif wp.orientation == 2:
        actions = np.array(
                [   [0,-1,0],
                    [0,1,0],
                    [-1,-1,1],
                    [1,-1,-1],
                    [1,1,1],
                    [-1,1,-1] ])
    elif wp.orientation == 3:
        actions = np.array(
                [   [-1,0,0],
                    [1,0,0],
                    [-1,1,1],
                    [-1,-1,-1],
                    [1,-1,1],
                    [1,1,-1] ])
    else:
        raise ValueError("Orientation Error")

    possible_neighbors = wp.tuple + actions
    possible_neighbors[:,2] = possible_neighbors[:,2]%4 # dealing with orientation wrap around
    neighbors = []
    for n in possible_neighbors:
        if valid_state(n, grid):
            neighbors.append(Waypoint(n[0], n[1], n[2]))

    return neighbors

def valid_state(state, grid):
    '''
    Returns if a state is on the map and not in collision

    :param state: tuple of state that needs to be checked.  Can be accessed with Waypoint.tuple
    :grid  grid: map with obstacles

    rtype: boolean that is true if valid state, otherwise false
    '''
    width, height = grid.shape
    #  ---------------------------- on map -----------------------------------    *****in collision*****
    if state[0] < 0 or state[1] < 0 or state[0] >= width or state[1] >= height or grid[state[0], state[1]]:
        return False
    else:
        return True


    
def get_hueristic(wp, end_wp):
    '''
    Returns an heuristic estimate of the distance from the current state to the goal state

    :param wp: Waypoint to estimate distance to goal
    :param end_wp: Waypoint for ending state of estiamte

    :rtype: estimate of distance between Waypoints

    Notes:
    Heuristic calculates distance in two parts: minimum travel distance since can move diagonal,
     and a cost for different orientations to prioritize expanding moves that don't change 
     orientation over other options
    '''
    dx = abs(end_wp.tuple[0] - wp.tuple[0])
    dy = abs(end_wp.tuple[1] - wp.tuple[1])
    h = max(dx,dy)
    return h


def in_queue(wp, q):
    '''
    Returns whether Waypoint is in queue

    :param wp: Waypoint to check
    :param q: queue that contains nodes as (cost, Waypoint)

    :rtype: boolean that is True if node is in queue

    Notes:
    a custom node class may make this function uncessary and can use built it PriorityQueue functionality    
    '''

    with q.mutex:
        for cost, q_wp in q.queue:
            if q_wp == wp:
                return True
        return False

def extract_path(previous_best, start_wp):
    path = [start_wp]
    while True:
        try:
            path.append(previous_best[path[-1]])
        except:
            break
    return path


class PathFinder(object):

    def get_path(self, grid, start_wp, end_wp):
        """Returns a list of Waypoints from the start Waypoint to the end Waypoint.
:
        :param grid: Grid is a 2D numpy ndarray of boolean values. grid[x, y] == True if the cell contains an obstacle.
        The grid dimensions are exposed via grid.shape
        :param start_wp: The Waypoint that the path should start from.
        :param end_wp: The Waypoint that the path should end on.
        :return: The path from the start waypoint to the end waypoint that follows the movement model without going
        off the grid or intersecting an obstacle.
        :rtype: A list of Waypoints.


        More documentation at
        https://docs.google.com/document/d/1b30L2LeKyMjO5rBeCui38j_HSUYgEGWXrwSRjB7AnYs/edit?usp=sharing

        Note:
        A* starting from the goal and working back towards the start is implemented
        

        ***************************** BIG O NOTATION ESTIMATE *********************
        Time Complexity of the algorithm is dominated by the number of nodes exapanded.
        For this part, the complexity is O(6^n) worst case where n is max(width, height) of map
        Based on some testing, the branching factor is closer to 1.2 in cases without an obstacle which makes the complexity O(1.2^n) where n is same as above
        
        for each node: In the worst case, the time complexity to process a node is O(m) primarily from checking if the node is in the priority queue or closed set where m is the number of nodes in the set.
        The number of states could be almost as large as the number of nodes in the map.  m = width*height of map

        Overall, O(6^n * m) for worst case.  In general, the run time complexity will be much less.

        Raises exception if no path found
        Returns stat_wp if start and end states are the same
        """

        if not valid_state(start_wp.tuple, grid):
            raise ValueError("Start Point not a valid state: either off of map or in collision")
        if not valid_state(end_wp.tuple, grid):
            raise ValueError("End Point not a valid state: either off of map or in collision")

        num_states = grid.size * 4
        map_shape = [grid.shape[0], grid.shape[1], 4]
        cost_from_start = np.full(map_shape, 1e5)
        cost_heuristic = np.full(map_shape, 1e5)
        cost_total = cost_from_start + cost_heuristic
        previous_best = dict()
        closed_set = set()
        open_set = Queue.PriorityQueue(num_states)
        cost_from_start[end_wp.tuple] = 0
        cost_heuristic[end_wp.tuple] = get_hueristic(end_wp, end_wp)
        cost_total[end_wp.tuple] = cost_heuristic[end_wp.tuple]
        open_set.put( (cost_heuristic[end_wp.tuple], end_wp) ) # search from endpoint and work towards start point

        while not open_set.empty():
            __, curWP = open_set.get(False)
            cur_cost = cost_from_start[curWP.tuple]
            if DEBUG: print(curWP)
            closed_set.add(curWP) # hueristic is monotonically increasing so don't need to revisit nodes
            if curWP == start_wp:
                break
            neighbors = get_neighbors(curWP, grid)
            
            for n in neighbors:
                if n in closed_set:
                    continue # don't need to do anything

                cur_cost_from_start = cur_cost + 1 # all neighbors have a cost 1 more than current state
                cur_cost_heuristic = get_hueristic(n, start_wp)
                cur_total_cost = cur_cost_from_start + cur_cost_heuristic
                
                if not in_queue(n, open_set):
                    if open_set.full():
                        raise Exception("Open set is full") # this should never happen
                    open_set.put( (cur_total_cost, n), False )

                elif (cur_cost_from_start >= cost_from_start[n.tuple]):
                    continue # not better than current path
                previous_best[n] = curWP
                cost_heuristic[n.tuple] = cur_cost_heuristic
                cost_from_start[n.tuple] = cur_cost_from_start
                cost_total[n.tuple] = cur_cost_heuristic + cur_cost_from_start

        path = extract_path(previous_best, start_wp)
        if path[-1] != end_wp:
            raise Exception("Path not found!")
        return path    

