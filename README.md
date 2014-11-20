Fiesty
======

Fiesty is a modern chess engine that I have started writing in order to investigate the following:

1) To understand the kinds of mistakes that amateur players make.  The idea is to analyze chess game databases and try to determine the depth, character, and frequency of the tactical errors relative to the player ratings.  Eventually the goal would be to come up with a chess engine that can realistically simulate a weak human.

2) To improve static evaluation (make it more human-like and strategic), perhaps by dynamically changing the static eval according to long term strategic goals.

3) To experiment with using neural network techniques to optimize static evaluation.

4) To experiment with endgames. See (2) above.

Currently, development has just started, with the following plan to get a simple engine up and running

[X] basic framework
[/] move generation
[ ] static eval
[ ] search
