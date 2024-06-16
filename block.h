#ifndef A5_BLOCK_H
#define A5_BLOCK_H

#include "board.h"
#include <vector>
#include <string>

using namespace std;

class Board;

enum class BlockType {
    I,
    J,
    L,
    O,
    S,
    Z,
    T,
    Q // alias for '*' block in level 4
};

BlockType getBlockType(const string& blockTypeStr);

class Block {

private:
    bool isHeavy;
    bool checkCollision(Board& board) const;
    bool checkCollision(Board board, int newRow, int newCol) const;
    bool checkCollision(Board& board, int row, int col, vector<vector<char>>& shape) const;


public:
    Block(int level) : isHeavy(false), levelGenerated(level) {}
    virtual ~Block() {}

    void moveLeft(Board& board);
    void moveRight(Board& board);
    void moveDown(Board& board);
    void rotate(Board& board, string direction);

    void drop(Board& board);

    int getTopLeftRow() const;
    int getTopLeftCol() const;

    vector<int>& getOccupiedRows();

    vector<vector<char>> getShape() const;

    bool placeBlock(Board& board);

    int getLevelGenerated() const;

    bool getIsHeavy();
    void setIsHeavy(bool heavy);



protected:
    vector<vector<char>> shape;
    int topLeftRow;
    int topLeftCol;
    int levelGenerated;
    vector<int> occupiedRows;

    virtual void doMoveLeft(Board& board);
    virtual void doMoveRight(Board& board);
    virtual void doMoveDown(Board& board);
    virtual void doRotate(Board& board, string direction);
};

#endif //A5_BLOCK_H
