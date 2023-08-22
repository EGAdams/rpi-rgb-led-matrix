YOUR ROLE:
Act as an expert C++ Developer.

YOUR TASK:
Instead of reading all of these values from the command line, read values from that will serve as a test plan for running numerous tests.

Here is an example config.txt file:

``` config
test 1:
--player1_score 3
--player2_score 3
--player1_sets 2
--player2_sets 3
--player1_games 2
--player2_games 3
// run game...
// check results...
// show results...
// reset test

test 2:
--player1_score 2
--player2_score 1
--player1_sets 1
--player2_sets 4
--player1_games 5
--player2_games 1
// run game...
// check results...
// show results...
// reset test
```

Please rewrite the main() method that you wrote before to read the config.txt file and run the tests instead of reading the command line arguments.

