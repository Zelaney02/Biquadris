#include "block.h"
#include <iostream>

BlockType getBlockType(const string& blockTypeStr) {
    if (blockTypeStr == "I") {
        return BlockType::I;
    } else if (blockTypeStr == "J") {
        return BlockType::J;
    } else if (blockTypeStr == "L") {
        return BlockType::L;
    } else if (blockTypeStr == "O") {
        return BlockType::O;
    } else if (blockTypeStr == "S") {
        return BlockType::S;
    } else if (blockTypeStr == "Z") {
        return BlockType::Z;
    } else if (blockTypeStr == "T") {
        return BlockType::T;
    } else {
        cerr << "Unknown block type: " << blockTypeStr << endl;
    }
}

// check collision when a block is first placed
bool Block::checkCollision(Board &board) const {
    auto shape = getShape();
    for (int i=0; i<shape.size(); i++) {
        for(int j=0; j<shape[i].size(); j++) {
            int row = topLeftRow + i;
            int col = topLeftCol + j;

            if (row < 0 || row >= board.BOARD_HEIGHT
                || col < 0 || col >= board.BOARD_WIDTH) {
                return true;
            } else if (board.getValue(row, col) != ' ' && shape[i][j] != ' ') {
                return true;
            }
        }
    }
    return false;
}

// check collision for block translations
bool Block::checkCollision(Board board, int newRow, int newCol) const {
    auto shape = getShape();

    for (int i=0; i<shape.size(); i++) {
        for (int j=0; j<shape[i].size(); j++) {
            int targetRow = newRow + i;
            int targetCol = newCol + j;

            board.eraseBlock(const_cast<Block &>(*this));

            if (targetRow < 0 || targetRow >= board.BOARD_HEIGHT
                || targetCol < 0 || targetCol >= board.BOARD_WIDTH) {
                return true;
            } else if (shape[i][j] != ' ' && board.getValue(targetRow, targetCol) != ' ') {
                return true;
            }
        }
    }
    return false;
}

// check collision for block rotations given the new shape of the block
bool Block::checkCollision(Board& board, int row, int col, vector<vector<char>>& shape) const {
    int shapeRow = shape.size();
    int shapeCols = shape[0].size();

    for (int i=0; i<shapeRow; i++) {
        for (int j=0; j<shapeCols; j++) {
            int targetRow = row + i;
            int targetCol = col + j;

            if (targetRow < 0 || targetRow >= board.BOARD_HEIGHT ||
                targetCol < 0 || targetCol >= board.BOARD_WIDTH) {
                return true;
            }

            if (shape[i][j] != ' ' && board.getValue(targetRow, targetCol) != ' ') {
                return true;
            }
        }
    }
    return false;
}

int Block::getTopLeftRow() const {
    return topLeftRow;
}

int Block::getTopLeftCol() const {
    return topLeftCol;
}

vector<vector<char>> Block::getShape() const {
    return shape;
}

// place block on bard at the start of each players turn
bool Block::placeBlock(Board &board) {
    if (!checkCollision(board)) {
        board.updateBlockPosition(*this);
        return true;
    }
    return false;
}

void Block::moveLeft(Board& board) {
    doMoveLeft(board);
    if (board.getLevel() > 2) {
        doMoveDown(board);
    }
    if (isHeavy) {
        doMoveDown(board);
        doMoveDown(board);
    }
}

void Block::moveRight(Board& board) {
    doMoveRight(board);
    if (board.getLevel() > 2) {
        doMoveDown(board);
    }
    if (isHeavy) {
        doMoveDown(board);
        doMoveDown(board);
    }
}

void Block::moveDown(Board& board) {
    doMoveDown(board);
}

void Block::rotate(Board& board, string direction) {
    doRotate(board, direction);
    if (board.getLevel() > 2) {
        doMoveDown(board);
    }
}

void Block::doMoveLeft(Board& board) {
    if (!checkCollision(board, topLeftRow, topLeftCol-1)) {
        board.eraseBlock(*this);
        Block::topLeftCol--;
        board.updateBlockPosition(*this);
    }
}

void Block::doMoveRight(Board& board) {
    if (!checkCollision(board, topLeftRow, topLeftCol+1)) {
        board.eraseBlock(*this);
        Block::topLeftCol++;
        board.updateBlockPosition(*this);
    }
}
void Block::doMoveDown(Board& board) {
    if (!checkCollision(board, topLeftRow+1, topLeftCol)) {
        board.eraseBlock(*this);
        Block::topLeftRow++;
        board.updateBlockPosition(*this);
    }
}

void Block::doRotate(Board& board, string direction) {
    int row = shape.size();
    int col = shape[0].size();

    int initial_topLeftRow = topLeftRow;
    int initial_topLeftCol = topLeftCol;
    vector<vector<char>> initial_shape = shape;
    board.eraseBlock(*this);

    // calculate the new shape of the block given the rotation
    vector<vector<char>> newShape(col, vector<char>(row));
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
            if (direction == "c") {
                newShape[j][row-1-i] = shape[i][j];
            } else if (direction == "cc") {
                newShape[col-1-j][i] = shape[i][j];
            }
        }
    }

    int newTopLeftRow = topLeftRow + row - newShape.size();
    int newTopLeftCol = topLeftCol;

    topLeftRow = newTopLeftRow;
    topLeftCol = newTopLeftCol;
    shape = newShape;

    if (!checkCollision(board, newTopLeftRow, newTopLeftCol, newShape)) {
        board.updateBlockPosition(*this);
    } else {
        topLeftRow = initial_topLeftRow;
        topLeftCol = initial_topLeftCol;
        shape = initial_shape;
        board.updateBlockPosition(*this);
    }
}

void Block::drop(Board& board) {
    while (!checkCollision(board, topLeftRow+1, topLeftCol)) {
        board.eraseBlock(*this);
        Block::topLeftRow++;
        board.updateBlockPosition(*this);
    }

    for (int i=0; i<shape.size(); i++) {
        occupiedRows.push_back(topLeftRow+i);
    }
    board.incrementBlocksDroppedSinceClear();
}


vector<int>& Block::getOccupiedRows() {
    return occupiedRows;
}

int Block::getLevelGenerated() const {
    return levelGenerated;
}

bool Block::getIsHeavy() {
    return isHeavy;
}
void Block::setIsHeavy(bool heavy) {
    isHeavy = heavy;
}