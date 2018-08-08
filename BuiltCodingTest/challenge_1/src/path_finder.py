from waypoint import Waypoint
import pdb
import Queue
import numpy as np


########## FIX NAMING CONVENTION!!! #########

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

def get_neighbors(curNode, obstacleGrid):
    """
    Return a list of neighbors as indexes that are accesible from current state

    :param curNode: the current node as an index
    :param grid: Grid is a 2D numpy ndarray of boolean values. grid[x, y] == True if the cell contains an obstacle.
        The grid dimensions are exposed via grid.shape

    :rtype: A list of ints which represent the neighbors
    """

def get_neighbors(curWP, grid):
    # could do this through a set of transformation matrices, but that seems like overkill
    # should original node be in there?
    if curWP.orientation == 0:
        actions = np.array(
                [   [0,1,0],
                    [1,1,1],
                    [-1,1,-1],
                    [0,-1,0],
                    [-1,-1,1],
                    [1,-1,-1],
                    [0, 0, 0], ])
    elif curWP.orientation == 1:
        actions = np.array(
                [   [1,0,0],
                    [1,-1,1],
                    [1,1,-1],
                    [-1,0,0],
                    [-1,1,1],
                    [-1,-1,-1],
                    [0, 0, 0] ])
    elif curWP.orientation == 2:
        actions = np.array(
                [   [0,-1,0],
                    [-1,-1,1],
                    [1,-1,-1],
                    [0,1,0],
                    [1,1,1],
                    [-1,1,-1],
                    [0, 0, 0] ])
    elif curWP.orientation == 3:
        actions = np.array(
                [   [-1,0,0],
                    [-1,1,1],
                    [-1,-1,-1],
                    [1,0,0],
                    [1,-1,1],
                    [1,1,-1],
                    [0, 0, 0] ])
    else:
        throw("Orientation Error")

    possible_neighbors = curWP.tuple + actions
    possible_neighbors[:,2] = possible_neighbors[:,2]%4 # dealing with orientation wrap around
    neighbors = []
    width, height = grid.shape
    for n in possible_neighbors:
        if n[0] < 0 or n[1] < 0 or n[0] >= width or n[1] >= height:
            continue #outside of map
        if not grid[n[0], n[1]]:
            neighbors.append(Waypoint(n[0], n[1], n[2]))

    return neighbors

    
def hueristic(wp, end_wp):
    # heuristic estimate for distance to goal
    # largest distance along x and y direction
    # heuristic can be improved to be closer to actual distance that Anne can travel
    dx = abs(end_wp.tuple[0] - wp.tuple[0])
    dy = abs(end_wp.tuple[1] - wp.tuple[1])
    # dtheta = end_wp.tuple[2] - wp.tuple[2]
    return max(dx, dy)


def in_queue(node, q):
    with q.mutex:
        return (node in q.queue)

def extract_path(costFromStart, start_wp, end_wp, grid):
    pdb.set_trace()
    path = [start_wp]
    while True:
        curWP = path[-1]
        # get neighbors
        neighbors = get_neighbors(curWP, grid)
        # choose the neighbor with the lowest cost
        min_cost = 1e4
        best_neighbor = None
        for n in neighbors[:-1]:
            if min_cost > costFromStart[n.tuple]:
                min_cost = costFromStart[n.tuple]
                best_neighbor = n
                
        # update path
        if best_neighbor is None:
            pdb.set_trace()
        print("Best Neighbor: {}, Cost: {}".format(best_neighbor, min_cost) )
        path.append(best_neighbor)
        # continue until goal is reached
        if best_neighbor == end_wp:
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
        print "EDIT HERE"

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

            __, curWP = openSet.get()
            curCost = costFromStart[curWP.tuple]
            print(curWP)
            closedSet.add(curWP) # hueristic is monotonically increasing so don't need to revisit nodes
            # if curWP == start_wp:
            #     pdb.set_trace()
            #     path = extract_path(costFromStart, start_wp, end_wp, grid)
            #     break
            neighbors = get_neighbors(curWP, grid)
            
            for n in neighbors[:-1]:
                if n in closedSet:
                    # print("Already explored")
                    continue # don't need to do anything

                curCostFromStart = curCost + 1 # all neighbors have a cost 1 more than current state
                curCostHeuristic = hueristic(n, start_wp)
                curTotalCost = curCostFromStart + curCostHeuristic
                
                if not in_queue((costFromStart[n.tuple], n), openSet):
                    openSet.put( (curTotalCost, n) )
                elif (curCostFromStart >= costFromStart[n.tuple]):
                    continue # not better than current path
                previousBest[n] = curWP
                costHeuristic[n.tuple] = curCostHeuristic
                costFromStart[n.tuple] = curCostFromStart
                costTotal[n.tuple] = curCostHeuristic + curCostFromStart



        path = extract_path(costFromStart, start_wp, end_wp, grid)
        pdb.set_trace()
        return path
        # return [start_wp, Waypoint(5, 6, 0), Waypoint(5, 7, 0), end_wp]


if __name__ == '__main__':
    grid = np.zeros((3, 3)).astype(np.bool)
    queries = [
        [Waypoint(2, 0, 2), Waypoint(0, 2, 1)],
    ]
    P = PathFinder()
    P.get_path(grid, queries[0][0], queries[0][1])