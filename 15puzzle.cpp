#include <iostream>
#include <climits>
#include <iterator>
#include <set>
#include <stack>
#include <algorithm>
#include <vector>
#include <queue>
#include <map>

using namespace std;

struct State {
  int board[4][4];
  int x, y; // location of blank
};

State goal, nongoal;
set<State> nongoals;
map<pair<int, int>, char> moves;

bool operator==(const State& lhs, const State& rhs) {
  if (!(lhs.x == rhs.x && lhs.y == rhs.y))
    return false;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (lhs.board[i][j] != rhs.board[i][j])
	return false;
  return true;
}

bool operator<(const State& lhs, const State& rhs) {
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      if (lhs.board[i][j] < rhs.board[i][j])
	return true;
      else if (lhs.board[i][j] > rhs.board[i][j])
	return false;
    }
  if (lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y))
    return true;
  return false;
}

int h(State &s) {
  int ret = 0;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      if (s.x == i && s.y == j) continue;
      int v = s.board[i][j];
      int x = (v - 1)/4, y = (v - 1)%4;
      ret += abs(x-i) + abs(y-j);
    }
  return ret;
}

void solve_astar(State &start) {
  map<State, int> d;
  map<State, pair<State, int> > p;
  priority_queue<pair<int, State>,
		 vector<pair<int, State> > > q;
  q.push(make_pair(-h(start), start));
  d[start] = 0; State curr;
  while (!q.empty()) {
    curr = q.top().second; q.pop();
    if (curr == goal || nongoals.count(curr) > 0) {
      break;
    }

    for (map<pair<int, int>, char>::iterator it = moves.begin(); it != moves.end(); it++) {
      int dx = it->first.first, dy = it->first.second;
      if (0 <= curr.x + dx && curr.x + dx < 4 &&
	  0 <= curr.y + dy && curr.y + dy < 4) {
	State tovisit = curr;
	swap(tovisit.board[tovisit.x][tovisit.y],
	     tovisit.board[tovisit.x + dx][tovisit.y + dy]);
	tovisit.x += dx; tovisit.y += dy;
	if (d.count(tovisit) == 0) {
	  d[tovisit] = d[curr] + 1;
	  p[tovisit] = make_pair(curr, it->second);
	  q.push(make_pair(-(d[tovisit] + h(tovisit)), tovisit));
	}
      }
    }
  }

  string soln;
  if (curr == goal) {
    while (!(curr == start)) {
      soln += p[curr].second;
      curr = p[curr].first;
    }
    reverse(soln.begin(), soln.end());
    cout << soln << endl;
  }
  else {
    cout << "This puzzle is not solvable." << endl;
  }
}

bool solvable;
vector<char> prefix;
int exceed, costbound;
set<State> visited;
State node;

bool dfs(int depth) {
  int currcost = depth + h(node);
  if (currcost > costbound) {
    exceed = min(exceed, currcost);
    return false;
  }
  if (node == goal || nongoals.count(node) > 0) {
    solvable = (node == goal);
    return true;
  }

  visited.insert(node);

  for (map<pair<int, int>, char>::iterator it = moves.begin(); it != moves.end(); it++) {
    int dx = it->first.first, dy = it->first.second;
    if (0 <= node.x + dx && node.x + dx < 4 &&
	0 <= node.y + dy && node.y + dy < 4) {
      swap(node.board[node.x][node.y],
	   node.board[node.x + dx][node.y + dy]);
      node.x += dx; node.y += dy;
      if (visited.count(node) == 0) {
	prefix.push_back(it->second);
	if (dfs(depth + 1))
	  return true;
	else
	  prefix.pop_back();
      }
      node.x -= dx; node.y -= dy;
      swap(node.board[node.x][node.y],
	   node.board[node.x + dx][node.y + dy]);
    }
  }

  visited.erase(node);

  return false;
}

void solve_idastar(State &start) {
  costbound = h(start);
  bool success;
  do {
    exceed = INT_MAX;
    prefix.clear();
    visited.clear();
    node = start;
    success = dfs(0);
    costbound = exceed;
    // cerr << "deepening to " << costbound << endl;
  } while (!success);
  if (solvable) {
    copy(prefix.begin(), prefix.end(), ostream_iterator<char>(cout, ""));
    cout << endl;
  }
  else
    cout << "This puzzle is not solvable." << endl;
}

int main() {
  moves[make_pair(0, -1)] = 'L'; moves[make_pair(0, +1)] = 'R';
  moves[make_pair(-1, 0)] = 'U'; moves[make_pair(+1, 0)] = 'D';

  int cases;
  cin >> cases;
  while (cases--) {
    State start;
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++) {
	goal.board[i][j] = nongoal.board[i][j] = 4*i + j + 1;
	cin >> start.board[i][j];
	if (start.board[i][j] == 0) {
	  start.x = i; start.y = j;
	}
      }
    goal.board[3][3] = nongoal.board[3][3] = 0;
    goal.x = nongoal.x = 3; goal.y = nongoal.y = 3;

    swap(nongoal.board[3][2], nongoal.board[3][1]);
    nongoals.insert(nongoal);
    swap(nongoal.board[3][2], nongoal.board[3][1]);

    swap(nongoal.board[0][0], nongoal.board[0][1]);
    nongoals.insert(nongoal);
    swap(nongoal.board[0][0], nongoal.board[0][1]);

    //    solve_astar(start);
    solve_idastar(start);
  }

  return 0;
}
