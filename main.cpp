#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstring>
#include <random>
#include "pathfinders.h"


int main() {

  int width, height;

  std::string temp;
  getline(std::cin, temp);
  std::cin >> temp; std::cin >> height;
  std::cin >> temp; std::cin >> width;
  getline(std::cin, temp);

  std::vector<unsigned char> passable{'.', 'G', 'S'};
  unsigned char* pMap = new unsigned char[height*width];

  std::vector<int> traversable;
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++) {
      char c; std::cin >> c;
      pMap[width*j + i] = (count(passable.begin(), passable.end(), c) > 0);
      if (count(passable.begin(), passable.end(), c) > 0)
        traversable.push_back(width*j + i);
    }

  // const int bufsize = height*width;
  // int* pOutBuffer = new int[height*width];

  std::random_device rng;
  std::uniform_int_distribution<> uniform(0, traversable.size() - 1);
  const int TIMES = 100;
  std::vector<std::pair<int, int>> tests;
  for (int t = 0; t < TIMES; t++) {
    int u = traversable[uniform(rng)];
    int v = traversable[uniform(rng)];
    tests.push_back({u % width, u / width});
    tests.push_back({v % width, v / width});
  }

  //                   //
  // BENCHMARKING CODE //
  //                   //

  for (auto PathFinder : {std::make_pair(BFSFindPath, "BFS"),
	std::make_pair(AStarFindPath, "AStar"),
	std::make_pair(AStarFindPathLandmarks, "AStarLandmarks"),
	std::make_pair(AStarFindPathNoTie, "AStarNoTie"),
	std::make_pair(BFSFindPathDiag, "BFSDiag"),
	std::make_pair(AStarFindPathDiag, "AStarDiag"),
	std::make_pair(AStarFindPathLandmarksDiag, "AStarLandmarksDiag"),
	std::make_pair(AStarFindPathNoTieDiag, "AStarNoTieDiag")}) {

    if (!strncmp(PathFinder.second, "AStarLandmarks", 14)) {
      Landmarks.clear();
      LD.clear();
      InitializeLandmarks(8, pMap, width, height);
    }

    if (!strncmp(PathFinder.second, "AStarLandmarksDiag", 18)) {
      Landmarks.clear();
      LD.clear();
      InitializeLandmarksDiag(8, pMap, width, height);
    }

    // printf("%12s -> %12s :: %s (%s)\n", "from", "to", "distance", "efficiency");
    long long tot = 0;
    const clock_t begin_time = clock();
    for (int t = 0; t < TIMES; t++) {
      std::pair<int, int> u = tests[2*t], v = tests[2*t + 1];
      PathFinder.first(u.first, u.second, v.first, v.second,
		       pMap, width, height, NULL, 0);
      // printf("(%4d, %4d) -> (%4d, %4d) :: %8d (%.2f)\n", u.first, u.second, v.first, v.second,
      // 	     dist, (dist == -1) ? 0.0 : 100.0 * dist/ExploredNodes);
      tot += ExploredNodes;
    }
    double ticks = clock() - begin_time;
    printf("%20s -- avg time: %-3.3fms\tavg nodes: %8lld\n", PathFinder.second, double(ticks/CLOCKS_PER_SEC)*1000/TIMES, tot/TIMES);
  }

  //                       //
  // END BENCHMARKING CODE //
  //                       //


  delete []pMap;
  // delete []pOutBuffer;

  return 0;
}

