#include "ScoreBoard.h"
#include <stdio.h>

ScoreBoard::ScoreBoard(Player* player1,
                       Player* player2 )
    : _player1(player1), _player2(player2) {}   
ScoreBoard::~ScoreBoard() {}

void ScoreBoard::update() { printf( "Updating ScoreBoard...\n" ); }