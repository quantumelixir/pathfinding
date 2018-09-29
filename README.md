# pathfinding
Efficient C++ pathfinders on 2D grids using variations of A* search.

## What is a pathfinder?

   An algorithm to find the shortest path from a given source to a
   given sink. It often gets little notice in theoretical computer
   science because there are nearly linear time algorithms to solve
   the more general problem of finding single source shortest paths
   (to all destinations) on any graph with non-negative edge
   weights. But in practice, when all you really care about is the
   shortest path to a particular target and there is some domain
   information that is available one can handily beat the performance
   of a simple breadth first search or Dijkstra. This is something not
   often taught in CS classes but the idea is to fudge the weights of
   the underlying graph (using the available domain information) in
   such a way that running Dijkstra on the graph with modified weights
   biases the search direction towards the target. In an
   implementation however, one doesn't actually modify the weights but
   uses a priority queue with priorities /different from the distance
   labels/ to achieve the same effect.

## What is the concrete problem it solves?

   Suppose you're given a 2D grid with 0/1s corresponding to
   impassable/passable locations and a given pair of source and target
   locations in the grid. Each location not on the boundary has 4
   neighbors in the N, S, W and E directions unless some of them are
   not passable (there's also the option of considering 8 neighbors
   per non-boundary location) and is at unit distance away from
   it. Find the shortest path from the source to target or decided
   there is none.

## API

   The API is very simple and explained [on this
   page](https://paradox.kattis.com/problems/paradoxpath) that
   inspired me to learn more about pathfinding.

```C++
   int FindPath(const int nStartX, const int nStartY,
                const int nTargetX, const int nTargetY,
                const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
                int* pOutBuffer, const int nOutBufferSize);
```

   Depending on the particular algorithm you choose from
   `pathfinders.h` the name of the function would change. For example
   in grids with 8 neighbors per location you would call one with the
   `Diag` suffix.

## What algorithms does it currently implement?

   Breadth first search, A* using the Manhattan distance heuristic
   with/without tiebreaking, and A* with lower bound computation using
   landmarks.

# Example

  Currently you can run benchmarks to compare the different algorithms like this.

```bash
make # might fail because you don't have boost installed
./path_test < maps/starcraft/GhostTown.map
```

```
                 BFS -- avg time: 3.648ms	avg nodes:   154413
               AStar -- avg time: 3.238ms	avg nodes:    33288
      AStarLandmarks -- avg time: 4.502ms	avg nodes:    32399
          AStarNoTie -- avg time: 3.377ms	avg nodes:    34534
             BFSDiag -- avg time: 8.263ms	avg nodes:   155472
           AStarDiag -- avg time: 5.012ms	avg nodes:    30826
  AStarLandmarksDiag -- avg time: 8.387ms	avg nodes:    45012
      AStarNoTieDiag -- avg time: 5.018ms	avg nodes:    36760
```

# Links

  Some cool related stuff.

  * [l1 pathfinder](http://mikolalysenko.github.io/l1-path-finder/www/)
  * [Map source for benchmarks](http://www.movingai.com/benchmarks/)

