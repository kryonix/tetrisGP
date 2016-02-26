#ifndef FEATURES_H
#define FEATURES_H

class TetrisBoard;

void play(TetrisBoard* board, int move, int rotate);
int blocks(TetrisBoard* board);
double weightedHeight(TetrisBoard* board, double weight);
int holes(TetrisBoard* board);
int blockades(TetrisBoard* board);
int columnSpace(TetrisBoard* board);
int touchFloor(TetrisBoard* board);
int touchWall(TetrisBoard* board);
int touchPiece(TetrisBoard* board);
int rowSpace(TetrisBoard* board);
int well(TetrisBoard* board);
int maxHeight(TetrisBoard* board);
#endif // FEATURES_H
