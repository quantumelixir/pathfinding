#include <vector>
#include <queue>
#include <tuple>
#include <cstdlib>
#include <climits>

#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "pathfinders.h"

using namespace std;

int ExploredNodes;
vector<int> Landmarks;
vector<vector<int>> LD;

int BFSFindPath(const int nStartX, const int nStartY,
		const int nTargetX, const int nTargetY,
		const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
		int* pOutBuffer, const int nOutBufferSize) {

  auto idx = [nMapWidth](int x, int y) {
    return x + y*nMapWidth;
  };

  const int n = nMapWidth*nMapHeight;
  const int startPos = idx(nStartX, nStartY), targetPos = idx(nTargetX, nTargetY);

  ExploredNodes = 0;
  vector<int> p(n), d(n, INT_MAX);
  d[startPos] = 0;
  queue<int> q;
  q.push(startPos);
  while (!q.empty()) {
    int u = q.front(); q.pop(); ExploredNodes++;
    for (auto e : {+1, -1, +nMapWidth, -nMapWidth}) {
      int v = u + e;
      if ((e == 1 && (v % nMapWidth == 0)) || (e == -1 && (u % nMapWidth == 0)))
	continue;
      if (0 <= v && v < n && d[v] == INT_MAX && pMap[v]) {
	p[v] = u;
	d[v] = d[u] + 1;
	if (v == targetPos)
	  goto end;
	q.push(v);
      }
    }
  }
 end:

  if (d[targetPos] == INT_MAX) {
    return -1;
  } else if (d[targetPos] <= nOutBufferSize) {
    int curr = targetPos;
    for (int i = d[targetPos] - 1; i >= 0; i--) {
      pOutBuffer[i] = curr;
      curr = p[curr];
    }
    return d[targetPos];
  }

  return d[targetPos];
}

int BFSFindPathDiag(const int nStartX, const int nStartY,
		    const int nTargetX, const int nTargetY,
		    const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
		    int* pOutBuffer, const int nOutBufferSize) {

  auto idx = [nMapWidth](int x, int y) {
    return x + y*nMapWidth;
  };

  const int n = nMapWidth*nMapHeight;
  const int startPos = idx(nStartX, nStartY), targetPos = idx(nTargetX, nTargetY);

  ExploredNodes = 0;
  vector<int> p(n), d(n, INT_MAX);
  queue<int> q;
  d[startPos] = 0;
  q.push(startPos);
  while (!q.empty()) {
    int u = q.front(); q.pop(); ExploredNodes++;
    for (auto e : {-nMapWidth-1, -nMapWidth+1, +nMapWidth-1, +nMapWidth+1,
	  +1, -1, +nMapWidth, -nMapWidth}) {
      int v = u + e;
      if (((e == 1 || e == -nMapWidth+1 || e == nMapWidth+1) && (v % nMapWidth == 0))
	  || ((e == -1 || e == -nMapWidth-1 || e == nMapWidth-1) && (u % nMapWidth == 0)))
	continue;
      if (0 <= v && v < n && d[v] == INT_MAX && pMap[v]) {
	p[v] = u;
	d[v] = d[u] + 1;
	if (v == targetPos)
	  goto end;
	q.push(v);
      }
    }
  }
 end:

  if (d[targetPos] == INT_MAX) {
    return -1;
  } else if (d[targetPos] <= nOutBufferSize) {
    int curr = targetPos;
    for (int i = d[targetPos] - 1; i >= 0; i--) {
      pOutBuffer[i] = curr;
      curr = p[curr];
    }
    return d[targetPos];
  }

  return d[targetPos]; // buffer size too small
}

int AStarFindPath(const int nStartX, const int nStartY,
		  const int nTargetX, const int nTargetY,
		  const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
		  int* pOutBuffer, const int nOutBufferSize) {

  auto idx = [nMapWidth](int x, int y) {
    return x + y*nMapWidth;
  };

  auto h = [=](int u) -> int { // lower bound distance to target from u
    int x = u % nMapWidth, y = u / nMapWidth;
    return abs(x-nTargetX) + abs(y-nTargetY);
  };

  const int n = nMapWidth*nMapHeight;
  const int startPos = idx(nStartX, nStartY), targetPos = idx(nTargetX, nTargetY);

  int discovered = 0; ExploredNodes = 0;
  vector<int> p(n), d(n, INT_MAX);
  priority_queue<tuple<int, int, int>,
		 vector<tuple<int, int, int>>,
		 greater<tuple<int, int, int>>> pq; // A* with tie breaking
  d[startPos] = 0;
  pq.push(make_tuple(0 + h(startPos), 0, startPos));
  while (!pq.empty()) {
    int u = get<2>(pq.top()); pq.pop(); ExploredNodes++;
    for (auto e : {+1, -1, +nMapWidth, -nMapWidth}) {
      int v = u + e;
      if ((e == 1 && (v % nMapWidth == 0)) || (e == -1 && (u % nMapWidth == 0)))
	continue;
      if (0 <= v && v < n && d[v] > d[u] + 1 && pMap[v]) {
	p[v] = u;
	d[v] = d[u] + 1;
	if (v == targetPos)
	  goto end;
	pq.push(make_tuple(d[v] + h(v), ++discovered, v));
      }
    }
  }
 end:

  if (d[targetPos] == INT_MAX) {
    return -1;
  } else if (d[targetPos] <= nOutBufferSize) {
    int curr = targetPos;
    for (int i = d[targetPos] - 1; i >= 0; i--) {
      pOutBuffer[i] = curr;
      curr = p[curr];
    }
    return d[targetPos];
  }

  return d[targetPos]; // buffer size too small
}

int AStarFindPathDiag(const int nStartX, const int nStartY,
		      const int nTargetX, const int nTargetY,
		      const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
		      int* pOutBuffer, const int nOutBufferSize) {

  auto idx = [nMapWidth](int x, int y) {
    return x + y*nMapWidth;
  };

  auto h = [=](int u) -> int { // lower bound distance to target from u
    int x = u % nMapWidth, y = u / nMapWidth;
    return max(abs(x-nTargetX), abs(y-nTargetY));
  };

  const int n = nMapWidth*nMapHeight;
  const int startPos = idx(nStartX, nStartY), targetPos = idx(nTargetX, nTargetY);

  int discovered = 0; ExploredNodes = 0;
  vector<int> p(n), d(n, INT_MAX);
  priority_queue<tuple<int, int, int>,
		 vector<tuple<int, int, int>>,
		 greater<tuple<int, int, int>>> pq; // A* with tie breaking
  d[startPos] = 0;
  pq.push(make_tuple(0 + h(startPos), 0, startPos));
  while (!pq.empty()) {
    int u = get<2>(pq.top()); pq.pop(); ExploredNodes++;
    for (auto e : {-nMapWidth-1, -nMapWidth+1, +nMapWidth-1, +nMapWidth+1,
	  +1, -1, +nMapWidth, -nMapWidth}) {
      int v = u + e;
      if (((e == 1 || e == -nMapWidth+1 || e == nMapWidth+1) && (v % nMapWidth == 0))
	  || ((e == -1 || e == -nMapWidth-1 || e == nMapWidth-1) && (u % nMapWidth == 0)))
	continue;
      if (0 <= v && v < n && d[v] > d[u] + 1 && pMap[v]) {
	p[v] = u;
	d[v] = d[u] + 1;
	if (v == targetPos)
	  goto end;
	pq.push(make_tuple(d[v] + h(v), ++discovered, v));
      }
    }
  }
 end:

  if (d[targetPos] == INT_MAX) {
    return -1;
  } else if (d[targetPos] <= nOutBufferSize) {
    int curr = targetPos;
    for (int i = d[targetPos] - 1; i >= 0; i--) {
      pOutBuffer[i] = curr;
      curr = p[curr];
    }
    return d[targetPos];
  }

  return d[targetPos]; // buffer size too small
}

void InitializeLandmarks(int k, const unsigned char* pMap, const int nMapWidth, const int nMapHeight) {

  vector<int> traversable;
  for (int i = 0; i < nMapWidth; i++)
    for (int j = 0; j < nMapHeight; j++)
      if (pMap[nMapWidth*j + i])
	traversable.push_back(nMapWidth*j + i);

  while (Landmarks.size() < k) {

    if (Landmarks.empty()) {
      boost::random::random_device rng;
      boost::random::uniform_int_distribution<> uniform(0, traversable.size() - 1);
      Landmarks.push_back(traversable[uniform(rng)]);
      continue;
    }

    const int n = nMapWidth*nMapHeight;
    vector<int> p(n), d(n, INT_MAX);
    queue<int> q;
    for (auto s : Landmarks) {
      d[s] = 0;
      q.push(s);
    }
    int farthest = -1, maxdist = -1;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      if (d[u] > maxdist)
	maxdist = d[u], farthest = u;
      for (auto e : {+1, -1, +nMapWidth, -nMapWidth}) {
	int v = u + e;
	if ((e == 1 && (v % nMapWidth == 0)) || (e == -1 && (u % nMapWidth == 0)))
	  continue;
	if (0 <= v && v < n && d[v] == INT_MAX && pMap[v]) {
	  p[v] = u;
	  d[v] = d[u] + 1;
	  q.push(v);
	}
      }
    }

    Landmarks.push_back(farthest); // works well when the graph is not too disconnected
  }

  LD.resize(Landmarks.size());
  for (int i = 0; i < Landmarks.size(); i++) {
    const int n = nMapWidth*nMapHeight;
    vector<int> p(n); LD[i].resize(n, INT_MAX);
    queue<int> q;
    int s = Landmarks[i];
    LD[i][s] = 0;
    q.push(s);
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (auto e : {+1, -1, +nMapWidth, -nMapWidth}) {
	int v = u + e;
	if ((e == 1 && (v % nMapWidth == 0)) || (e == -1 && (u % nMapWidth == 0)))
	  continue;
	if (0 <= v && v < n && LD[i][v] == INT_MAX && pMap[v]) {
	  p[v] = u;
	  LD[i][v] = LD[i][u] + 1;
	  q.push(v);
	}
      }
    }
  }
}

int AStarFindPathLandmarks(const int nStartX, const int nStartY,
			   const int nTargetX, const int nTargetY,
			   const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
			   int* pOutBuffer, const int nOutBufferSize) {

  auto idx = [nMapWidth](int x, int y) {
    return x + y*nMapWidth;
  };

  const int n = nMapWidth*nMapHeight;
  const int startPos = idx(nStartX, nStartY), targetPos = idx(nTargetX, nTargetY);

  auto h = [=](int u) { // lower bound distance to target from u
    int m = 0;
    for (int i = 0; i < Landmarks.size(); i++) // global vector<int> Landmarks
      m = max(m, LD[i][targetPos] - LD[i][u]); // global vector<vector<int>> LD
    return m;
  };

  int discovered = 0; ExploredNodes = 0;
  vector<int> p(n), d(n, INT_MAX);
  priority_queue<tuple<int, int, int>,
		 vector<tuple<int, int, int>>,
		 greater<tuple<int, int, int>>> pq; // A* with tie breaking
  d[startPos] = 0;
  pq.push(make_tuple(0 + h(startPos), 0, startPos));
  while (!pq.empty()) {
    int u = get<2>(pq.top()); pq.pop(); ExploredNodes++;
    for (auto e : {+1, -1, +nMapWidth, -nMapWidth}) {
      int v = u + e;
      if ((e == 1 && (v % nMapWidth == 0)) || (e == -1 && (u % nMapWidth == 0)))
	continue;
      if (0 <= v && v < n && d[v] > d[u] + 1 && pMap[v]) {
	p[v] = u;
	d[v] = d[u] + 1;
	if (v == targetPos)
	  goto end;
	pq.push(make_tuple(d[v] + h(v), ++discovered, v));
      }
    }
  }
 end:

  if (d[targetPos] == INT_MAX) {
    return -1;
  } else if (d[targetPos] <= nOutBufferSize) {
    int curr = targetPos;
    for (int i = d[targetPos] - 1; i >= 0; i--) {
      pOutBuffer[i] = curr;
      curr = p[curr];
    }
    return d[targetPos];
  }

  return d[targetPos]; // buffer size too small
}

void InitializeLandmarksDiag(int k, const unsigned char* pMap, const int nMapWidth, const int nMapHeight) {

  vector<int> traversable;
  for (int i = 0; i < nMapWidth; i++)
    for (int j = 0; j < nMapHeight; j++)
      if (pMap[nMapWidth*j + i])
	traversable.push_back(nMapWidth*j + i);

  while (Landmarks.size() < k) {

    if (Landmarks.empty()) {
      boost::random::random_device rng;
      boost::random::uniform_int_distribution<> uniform(0, traversable.size() - 1);
      Landmarks.push_back(traversable[uniform(rng)]);
      continue;
    }

    const int n = nMapWidth*nMapHeight;
    vector<int> p(n), d(n, INT_MAX);
    queue<int> q;
    for (auto s : Landmarks) {
      d[s] = 0;
      q.push(s);
    }
    int farthest = -1, maxdist = -1;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      if (d[u] > maxdist)
	maxdist = d[u], farthest = u;
      for (auto e : {-nMapWidth-1, -nMapWidth+1, +nMapWidth-1, +nMapWidth+1,
	    +1, -1, +nMapWidth, -nMapWidth}) {
	int v = u + e;
	if (((e == 1 || e == -nMapWidth+1 || e == nMapWidth+1) && (v % nMapWidth == 0))
	    || ((e == -1 || e == -nMapWidth-1 || e == nMapWidth-1) && (u % nMapWidth == 0)))
	  continue;
	if (0 <= v && v < n && d[v] == INT_MAX && pMap[v]) {
	  p[v] = u;
	  d[v] = d[u] + 1;
	  q.push(v);
	}
      }
    }

    Landmarks.push_back(farthest); // works well when the graph is not too disconnected
  }

  LD.resize(Landmarks.size());
  for (int i = 0; i < Landmarks.size(); i++) {
    const int n = nMapWidth*nMapHeight;
    vector<int> p(n); LD[i].resize(n, INT_MAX);
    queue<int> q;
    int s = Landmarks[i];
    LD[i][s] = 0;
    q.push(s);
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (auto e : {-nMapWidth-1, -nMapWidth+1, +nMapWidth-1, +nMapWidth+1,
	    +1, -1, +nMapWidth, -nMapWidth}) {
	int v = u + e;
	if (((e == 1 || e == -nMapWidth+1 || e == nMapWidth+1) && (v % nMapWidth == 0))
	    || ((e == -1 || e == -nMapWidth-1 || e == nMapWidth-1) && (u % nMapWidth == 0)))
	  continue;
	if (0 <= v && v < n && LD[i][v] == INT_MAX && pMap[v]) {
	  p[v] = u;
	  LD[i][v] = LD[i][u] + 1;
	  q.push(v);
	}
      }
    }
  }
}

int AStarFindPathLandmarksDiag(const int nStartX, const int nStartY,
			       const int nTargetX, const int nTargetY,
			       const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
			       int* pOutBuffer, const int nOutBufferSize) {

  auto idx = [nMapWidth](int x, int y) {
    return x + y*nMapWidth;
  };

  const int n = nMapWidth*nMapHeight;
  const int startPos = idx(nStartX, nStartY), targetPos = idx(nTargetX, nTargetY);

  auto h = [=](int u) { // lower bound distance to target from u
    int m = 0;
    for (int i = 0; i < Landmarks.size(); i++) // global vector<int> Landmarks
      m = max(m, LD[i][targetPos] - LD[i][u]); // global vector<vector<int>> LD
    return m;
  };

  int discovered = 0; ExploredNodes = 0;
  vector<int> p(n), d(n, INT_MAX);
  priority_queue<tuple<int, int, int>,
		 vector<tuple<int, int, int>>,
		 greater<tuple<int, int, int>>> pq; // A* with tie breaking
  d[startPos] = 0;
  pq.push(make_tuple(0 + h(startPos), 0, startPos));
  while (!pq.empty()) {
    int u = get<2>(pq.top()); pq.pop(); ExploredNodes++;
    for (auto e : {-nMapWidth-1, -nMapWidth+1, +nMapWidth-1, +nMapWidth+1,
	  +1, -1, +nMapWidth, -nMapWidth}) {
      int v = u + e;
      if (((e == 1 || e == -nMapWidth+1 || e == nMapWidth+1) && (v % nMapWidth == 0))
	  || ((e == -1 || e == -nMapWidth-1 || e == nMapWidth-1) && (u % nMapWidth == 0)))
	continue;
      if (0 <= v && v < n && d[v] > d[u] + 1 && pMap[v]) {
	p[v] = u;
	d[v] = d[u] + 1;
	if (v == targetPos)
	  goto end;
	pq.push(make_tuple(d[v] + h(v), ++discovered, v));
      }
    }
  }
 end:

  if (d[targetPos] == INT_MAX) {
    return -1;
  } else if (d[targetPos] <= nOutBufferSize) {
    int curr = targetPos;
    for (int i = d[targetPos] - 1; i >= 0; i--) {
      pOutBuffer[i] = curr;
      curr = p[curr];
    }
    return d[targetPos];
  }

  return d[targetPos]; // buffer size too small
}

int AStarFindPathNoTie(const int nStartX, const int nStartY,
		       const int nTargetX, const int nTargetY,
		       const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
		       int* pOutBuffer, const int nOutBufferSize) {

  auto idx = [nMapWidth](int x, int y) {
    return x + y*nMapWidth;
  };

  auto h = [=](int u) { // lower bound distance to target from u
    int x = u % nMapWidth, y = u / nMapWidth;
    return abs(x-nTargetX) + abs(y-nTargetY);
  };

  const int n = nMapWidth*nMapHeight;
  const int startPos = idx(nStartX, nStartY), targetPos = idx(nTargetX, nTargetY);

  ExploredNodes = 0;
  vector<int> p(n), d(n, INT_MAX);
  priority_queue<pair<int, int>,
		 vector<pair<int, int>>,
		 greater<pair<int, int>>> pq;
  d[startPos] = 0;
  pq.push(make_pair(0 + h(startPos), startPos));
  while (!pq.empty()) {
    int u = pq.top().second; pq.pop(); ExploredNodes++;
    for (auto e : {+1, -1, +nMapWidth, -nMapWidth}) {
      int v = u + e;
      if ((e == 1 && (v % nMapWidth == 0)) || (e == -1 && (u % nMapWidth == 0)))
	continue;
      if (0 <= v && v < n && d[v] > d[u] + 1 && pMap[v]) {
	p[v] = u;
	d[v] = d[u] + 1;
	if (v == targetPos)
	  goto end;
	pq.push(make_pair(d[v] + h(v), v));
      }
    }
  }
 end:

  if (d[targetPos] == INT_MAX) {
    return -1;
  } else if (d[targetPos] <= nOutBufferSize) {
    int curr = targetPos;
    for (int i = d[targetPos] - 1; i >= 0; i--) {
      pOutBuffer[i] = curr;
      curr = p[curr];
    }
    return d[targetPos];
  }

  return d[targetPos]; // buffer size too small
}

int AStarFindPathNoTieDiag(const int nStartX, const int nStartY,
			   const int nTargetX, const int nTargetY,
			   const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
			   int* pOutBuffer, const int nOutBufferSize) {

  auto idx = [nMapWidth](int x, int y) {
    return x + y*nMapWidth;
  };

  auto h = [=](int u) { // lower bound distance to target from u
    int x = u % nMapWidth, y = u / nMapWidth;
    return max(abs(x-nTargetX), abs(y-nTargetY));
  };

  const int n = nMapWidth*nMapHeight;
  const int startPos = idx(nStartX, nStartY), targetPos = idx(nTargetX, nTargetY);

  ExploredNodes = 0;
  vector<int> p(n), d(n, INT_MAX);
  priority_queue<pair<int, int>,
		 vector<pair<int, int>>,
		 greater<pair<int, int>>> pq;
  d[startPos] = 0;
  pq.push(make_pair(0 + h(startPos), startPos));
  while (!pq.empty()) {
    int u = pq.top().second; pq.pop(); ExploredNodes++;
    for (auto e : {-nMapWidth-1, -nMapWidth+1, +nMapWidth-1, +nMapWidth+1,
	  +1, -1, +nMapWidth, -nMapWidth}) {
      int v = u + e;
      if (((e == 1 || e == -nMapWidth+1 || e == nMapWidth+1) && (v % nMapWidth == 0))
	  || ((e == -1 || e == -nMapWidth-1 || e == nMapWidth-1) && (u % nMapWidth == 0)))
	continue;
      if (0 <= v && v < n && d[v] > d[u] + 1 && pMap[v]) {
	p[v] = u;
	d[v] = d[u] + 1;
	if (v == targetPos)
	  goto end;
	pq.push(make_pair(d[v] + h(v), v));
      }
    }
  }
 end:

  if (d[targetPos] == INT_MAX) {
    return -1;
  } else if (d[targetPos] <= nOutBufferSize) {
    int curr = targetPos;
    for (int i = d[targetPos] - 1; i >= 0; i--) {
      pOutBuffer[i] = curr;
      curr = p[curr];
    }
    return d[targetPos];
  }

  return d[targetPos]; // buffer size too small
}
