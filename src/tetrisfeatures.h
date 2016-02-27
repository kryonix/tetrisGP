#ifndef FEATURES_H
#define FEATURES_H

#include "tetrisboard.h"
#include <bitset>

void play(TetrisBoard* board, int move, int rotate);
int blocks(const std::bitset<TetrisBoard::Fields> *board);
double weightedHeight(const std::bitset<TetrisBoard::Fields>* board, double weight);
int holes(const std::bitset<TetrisBoard::Fields> *board);
int blockades(const std::bitset<TetrisBoard::Fields> *board);

double holesBlockades(const std::bitset<TetrisBoard::Fields>* board, double holeW, double blockadeW);

double holesBlockadesColumn(const std::bitset<TetrisBoard::Fields>* board, double holeW, double blockadeW, double columnW);

int columnSpace(const std::bitset<TetrisBoard::Fields> *board);
int touchFloor(const std::bitset<TetrisBoard::Fields> *board);
int touchWall(const std::bitset<TetrisBoard::Fields> *board);
int touchPiece(const std::bitset<TetrisBoard::Fields> *board);
int rowSpace(const std::bitset<TetrisBoard::Fields> *board);
int well(const std::bitset<TetrisBoard::Fields> *board);
int maxHeight(TetrisBoard *board);
#endif // FEATURES_H
