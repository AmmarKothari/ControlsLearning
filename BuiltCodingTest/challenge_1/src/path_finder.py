from waypoint import Waypoint
from path_visualizer import PathVisualizer
import pdb
import Queue
import numpy as np


########## FIX NAMING CONVENTION!!! #########

DEBUG = True

class Node(object):
    # object to represent each node on search graph
    def __init__(self, W, grid):
        state = Waypoint(0,0,0)
        index = 0
        cost_to_go = 1e10           # actual cost to get to a node
        cost_hueristic = 1e10       # heuristic cost to get to goal
        cost_estimate = 1e10        # estimated total cost of path through this node
        bestPrevious = None         # previous waypoint that has the lowest cost

    def __cmp__(self, other):
        return cmp(self.cost, other.cost)

def get_neighbors(curWP, grid):
    '''
    Return a list of neighbors to current state as Waypoints based on size of grid, movement model, and obstacles

    :param curWP: Waypoint of the current state
    :param grid: Grid is a 2D numpy ndarray of boolean values. grid[x, y] == True if the cell contains an obstacle.
        The grid dimensions are exposed via grid.shape

    :rtype: A list of Waypoints that represent neibhbors

    Notes:
    # could do this through a set of transformation matrices, but that seems like overkill
    '''
    if curWP.orientation == 0:
        actions = np.array(
                [   [0,1,0],
                    [0,-1,0],
                    [1,1,1],
                    [-1,1,-1],
                    [-1,-1,1],
                    [1,-1,-1] ])
    elif curWP.orientation == 1:
        actions = np.array(
                [   [1,0,0],
                    [-1,0,0],
                    [1,-1,1],
                    [1,1,-1],
                    [-1,1,1],
                    [-1,-1,-1] ])
    elif curWP.orientation == 2:
        actions = np.array(
                [   [0,-1,0],
                    [0,1,0],
                    [-1,-1,1],
                    [1,-1,-1],
                    [1,1,1],
                    [-1,1,-1] ])
    elif curWP.orientation == 3:
        actions = np.array(
                [   [-1,0,0],
                    [1,0,0],
                    [-1,1,1],
                    [-1,-1,-1],
                    [1,-1,1],
                    [1,1,-1] ])
    else:
        throw("Orientation Error")

    possible_neighbors = curWP.tuple + actions
    possible_neighbors[:,2] = possible_neighbors[:,2]%4 # dealing with orientation wrap around
    neighbors = []
    width, height = grid.shape
    for n in possible_neighbors:
        if n[0] < 0 or n[1] < 0 or n[0] >= width or n[1] >= height:
            continue #outside of map
        if not grid[n[0], n[1]]: #obstacles
            neighbors.append(Waypoint(n[0], n[1], n[2]))

    return neighbors

    
def hueristic(wp, end_wp):
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
    dtheta = abs(end_wp.tuple[2] - wp.tuple[2])
    # h = max(dx, dy) # paths have a lot of flare
    # h = abs(dx - dy) + min(dx,dy)
    h = max(dx,dy) + dtheta
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

# def extract_path(costFromStart, start_wp, end_wp, grid):
#     path = [start_wp]
#     while True:
#         curWP = path[-1]
#         # get neighbors
#         neighbors = get_neighbors(curWP, grid)
#         # choose the neighbor with the lowest cost
#         min_cost = 1e4
#         best_neighbor = None
#         for n in neighbors:
#             if min_cost > costFromStart[n.tuple]:
#                 min_cost = costFromStart[n.tuple]
#                 best_neighbor = n
                
#         # update path
#         if best_neighbor is None:
#             throw("No neighbors visitied")
#         if DEBUG: print("Best Neighbor: {}, Cost: {}".format(best_neighbor, min_cost) )
#         path.append(best_neighbor)
#         # continue until goal is reached
#         if best_neighbor == end_wp:
#             break
#     return path

def extract_path(previousBest, start_wp):
    path = [start_wp]
    while True:
        try:
            path.append(previousBest[path[-1]])
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
        """
        numStates = grid.size * 4
        mapShape = [grid.shape[0], grid.shape[1], 4]
        costFromStart = np.full(mapShape, 1e5)
        costHeuristic = np.full(mapShape, 1e5)
        costTotal = costFromStart + costHeuristic
        previousBest = dict()
        closedSet = set()
        openSet = Queue.PriorityQueue(numStates)
        costFromStart[end_wp.tuple] = 0
        costHeuristic[end_wp.tuple] = hueristic(end_wp, end_wp)
        costTotal[end_wp.tuple] = costHeuristic[end_wp.tuple]
        openSet.put( (costHeuristic[end_wp.tuple], end_wp) ) # search from endpoint and work towards start point
        

        while not openSet.empty():

            __, curWP = openSet.get(False)
            curCost = costFromStart[curWP.tuple]
            if DEBUG: print(curWP)
            closedSet.add(curWP) # hueristic is monotonically increasing so don't need to revisit nodes
            if curWP == start_wp:
                break
            neighbors = get_neighbors(curWP, grid)
            
            for n in neighbors:
                if n in closedSet:
                    # print("Already explored")
                    continue # don't need to do anything

                curCostFromStart = curCost + 1 # all neighbors have a cost 1 more than current state
                curCostHeuristic = hueristic(n, start_wp)
                curTotalCost = curCostFromStart + curCostHeuristic
                
                if not in_queue(n, openSet):
                    if openSet.full():
                        pdb.set_trace()
                    openSet.put( (curTotalCost, n), False )

                elif (curCostFromStart >= costFromStart[n.tuple]):
                    continue # not better than current path
                previousBest[n] = curWP
                costHeuristic[n.tuple] = curCostHeuristic
                costFromStart[n.tuple] = curCostFromStart
                costTotal[n.tuple] = curCostHeuristic + curCostFromStart

        # path = extract_path(costFromStart, start_wp, end_wp, grid)
        path = extract_path(previousBest, start_wp)
        return path

def smallGridTest():
    grid = np.zeros((3, 3)).astype(np.bool)
    queries = [
        [Waypoint(2, 0, 2), Waypoint(0, 2, 1)],
    ]
    return (grid, queries)

def test_no_obstacles_straight_line():
    grid = np.zeros((20, 20)).astype(np.bool)
    queries = [
        [Waypoint(5, 5, 0), Waypoint(5, 8, 0)],
        [Waypoint(16, 5, 1), Waypoint(8, 5, 1)],
        [Waypoint(5, 15, 3), Waypoint(16, 15, 3)],
    ]
    return (grid, queries)



def test_with_multiple_obstacles():
    grid = np.zeros((20, 20)).astype(np.bool)

    grid[3:4, 0:15] = True
    grid[13:14, 5:20] = True

    queries = [
        [Waypoint(0, 0, 0), Waypoint(19, 19, 3)]
    ]
    return (grid, queries)

if __name__ == '__main__':
    grid, queries = test_no_obstacles_straight_line()
    P = PathFinder()
    V = PathVisualizer()
    for q in queries:
        path = P.get_path(grid, q[0], q[1])

        print(path)


    

