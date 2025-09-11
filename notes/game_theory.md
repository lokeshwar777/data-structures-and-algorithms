- combinatorial game theory
  - perfect info (no random)
  - impartial - same moves for players
  - partisan - diff moves for players
- states - win, lose or tie
- misere game - opposite / reversed winning condition
- composite game -> combination of multiple independent imparital games (disjunctive sum)
- x1 + x2 + .. + xn -> normal sum
- x1 ^ x2 ^ .. ^ xn -> nim-sum
- mex (minimum excludant)

- nimbers/grundy numbers
  - reduce any impartial game to nim pile
  - deterministic recurrence -> $G(n)=mex\{G(n−m)∣m∈allowed moves, n−m≥0\}$
  - periodicity (can be found at 3 * max_move_size in the worst case) + induction + exceptions (preperiod)
  - closed form / formula -> $G(n)=f(n\ mod\ p)$
- sprague-grundy theorem -> nim-sum of grundy numbers for composite games

- why XOR instead of sum for nim game-> don't know why but reason is grundy numbers!!!
- in general you can win if and only if any of the next states is a losing state
  - how? you can force your opponent to a losing state
  - if all of the next states are winning states then you lose

## Problems

- subtraction game -> can pick [1, k] stones from x & who pick last stone wins (SOL: x % (k + 1) are all losing)
- nim game -> if nim-sum == 0 lose else win

- monty hall problem
- prisoner's dilemma
- Wythoff’s Game
