
import math
import os
import random
import re
import sys
sys.setrecursionlimit(10**9)

#
# Complete the 'journeyScheduling' function below.
#
# The function is expected to return an INTEGER_ARRAY.
# The function accepts following parameters:
#  1. INTEGER n
#  2. 2D_INTEGER_ARRAY roads
#  3. 2D_INTEGER_ARRAY journeys
#

def journeyScheduling(n, roads, journeys):
    # Write your code here
    adj_list = [[] for i in range(n+1)]
    for u, v in roads:
        adj_list[u].append(v)
        adj_list[v].append(u)
    
    start = 1
    down_max_one = [(0, None)] * (n+1)
    down_max_two = [0] * (n+1)
    visited = [False] * (n+1)
    queue = [(start, None)]
    
    while len(queue):
        edge = queue.pop()
        u, p = edge
        if not visited[u]:
            queue.append(edge)
            visited[u] = True
            for v in adj_list[u]:
                if visited[v]:
                    continue
                # dfs_down(v)
                queue.append((v, u))
            
            continue
        
        d_one = 0
        nl_one = None
        d_two = 0
        nl_two = None
        for v in adj_list[u]:
            if v == p:
                continue
            d_v_one, nl_v_one = down_max_one[v]
            d_v_one += 1
            if d_v_one > d_one:
                d_two = d_one
                # nl_two = nl_one
                d_one = d_v_one
                nl_one = v

            elif d_v_one >= d_two:
                d_two = d_v_one
                # nl_two = v
                                    
        down_max_one[u] = (d_one, nl_one)
        down_max_two[u] = d_two
    
    # dfs_down(start)
        
    visited = [False] * (n+1)
    up_max = [0] * (n+1)
    dist_max = [0] * (n+1)
    queue = [(start, None)]
    
    while len(queue):
        edge = queue.pop()
        u, p = edge
        visited[u] = True

        if p is None:
            up_u = 0
            # up_nl_u = None#set()
        else:
            up_p = up_max[p]
            up_u_p = up_p + 1
            d_p_one, d_nl_p_one = down_max_one[p]
            d_p_two = down_max_two[p]
            
            if u != d_nl_p_one:
                d_p_v = d_p_one + 1
            else:
                d_p_v = d_p_two + 1
            
            up_u = max(up_u_p, d_p_v)
            
        up_max[u] = up_u
        d_u, d_nl_u = down_max_one[u]
        
        dist_max[u] = max(d_u, up_u)
        
        for v in adj_list[u]:
            if visited[v]:
                continue
            queue.append((v, u))


    # dfs_max_dist(start, None)
    diameter = max(dist_max)
    
    # print(diameter)
    # print(dist_max)
    m = len(journeys)
    res = [0] * m
    for i in range(m) :
        v, k = journeys[i]
        max_v = dist_max[v]
        res[i] = max_v + (k-1)*diameter
    return res
    

if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')

    first_multiple_input = input().rstrip().split()

    n = int(first_multiple_input[0])

    m = int(first_multiple_input[1])

    roads = [[] for i in range(n-1)]

    for i in range(n - 1):
        road_inputs = input().rstrip().split()
        roads[i] = (int(road_inputs[0]), int(road_inputs[1]))

    journeys = [[] for i in range(m)]

    for j in range(m):
        journey_inputs = input().rstrip().split()
        journeys[j] = (int(journey_inputs[0]), int(journey_inputs[1]))

    result = journeyScheduling(n, roads, journeys)
    # result = [0, 11]
    fptr.write('\n'.join(map(str, result)))
    fptr.write('\n')

    fptr.close()
