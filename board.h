#ifndef A5_BOARD_H
#define A5_BOARD_H

#include <vector>
#include <memory>
#include "block.h"
using namespace std;

class Block;

class Board {
private:
    bool isBlind;
    vector<vector<char>> grid;
    int level;
    int score;
    int hiScore;
    int blocksDroppedSinceClear = 0;

    Block* nextBlock = nullptr;

    void updateScoreLinesCleared(int linesCleared);
    void updateScore(const Block& block);
    void updateHiScore();

public:
    static const int BOARD_WIDTH = 11;
    static const int BOARD_HEIGHT = 18;

    Board();
    void display(Board& opponent);
    int clearRows(vector<unique_ptr<Block>>& droppedBlocks);

    void setLevel(int newLevel);
    void setScore(int newScore);

    int getLevel();
    int getScore();
    int getHiScore();

    void updateBlockPosition(Block& block);
    void eraseBlock(Block& block);
    char getValue(int r, int c);
    Block* getNextBlock() { return nextBlock; }

    void restart();

    void incrementBlocksDroppedSinceClear() { blocksDroppedSinceClear++; }
    void resetBlocksDroppedSinceClear() { blocksDroppedSinceClear = 0; }
    int getBlocksDroppedSinceClear() { return blocksDroppedSinceClear; }

    void setNextBlock(Block* block) { nextBlock = block; }

    bool getIsBlind();
    void setIsBlind(bool blind);

};

#endif //A5_BOARD_H
