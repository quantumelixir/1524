1524
----

* What it currently does

Solves not too difficult instances of the 15 puzzle [6] using IDA*
with the Manhattan distance heuristic. By "not too difficult" I mean
the instance must be solvable using at most 65 moves. The hardest
instances of the 15 puzzle (see the veryhard input file) require as
many as 80 moves to solve and the Manhattan distance heuristic in the
current implementation isn't strong enough to prune the search space
sufficiently.

There is a (slow) A* implementation and a memory efficient (and
faster) IDA* implementation inside.

```bash
$ g++ -std=c++11 -O3 15puzzle.cpp
$ time ./a.out < input

LLLDRDRDR
This puzzle is not solvable.
ULULDRDRURDDLLLUUURDRRDLLDRULUURRDLDDR
LUUURDDDLUULLDDRURUULDLDRRURULLDRULDLURRRDDD
This puzzle is not solvable.
URDLUURDRDLURULDDLULDRUULDRURDLLDRRDRUULLDRDR

real	0m8.112s
user	0m8.105s
sys	0m0.004s
```

* What needs to be done

A heuristic generated by disjoint pattern databases [1] would be
necessary to quickly solve the 15 puzzle. Some multithreading maybe?
Bidirectional search?

* In the future

Some attempts at the 24 puzzle perhaps.

* References

[1]: R. E. Korf and A. Felner, Disjoint pattern database heuristics
[2]: R. Zhou and E. Hansen, Space-efficient memory-based heuristics
[3]: A. Felner and A. Adler, Solving the 24 Puzzle with Instance Dependent Pattern Databases
[4]: R. C. Holte, J. Newton, A. Felner, R. Meshulam, D. Furcy, Multiple Pattern Databases
[5]: R. E. Korf, Depth-First Iterative-Deepening- An Optimal Admissible Tree Search
[6]: Wm. W. Johnson, Notes on the 15-Puzzle
[7]: R. C. Holte, A. Felner, J. Newton, R. Meshulam, D. Furcy, Maximizing over Multiple Pattern Databases Speeds up Heuristic Search
[8]: A. Felner, R. E. Korf, S. Hanan, Additive Pattern Database Heuristics

