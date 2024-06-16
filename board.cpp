#include "board.h"
#include <iostream>
#include <algorithm>

Board::Board() : level{0}, score{0}, hiScore{0}, isBlind{false} {
    grid.resize(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' '));
}

// display the board to the terminal
void Board::display(Board& opponent) {
    cout << "Level:    " << level << "        " << "Level:    " << opponent.level << endl;
    cout << "Score:    " << score << "        " << "Score:    " << opponent.score << endl;
    cout << "-----------" << "        " << "-----------" << endl;
    for (int i=0; i<BOARD_HEIGHT; i++) {
        for (int j=0; j<BOARD_WIDTH; j++) {
            if (isBlind && i >= 2 && i < 9 && j >= 2 && j < 12) {
                cout << '?';
            } else {
                cout << grid[i][j];
            }
        }
        cout << "        ";
        for (int j=0; j<BOARD_WIDTH; j++) {
            if (opponent.getIsBlind() && i >= 2 && i < 9 && j >= 2 && j < 12) {
                cout << '?';
            } else {
                cout << opponent.grid[i][j];
            }
        }
        cout << endl;
    }
    cout << "-----------" << "        " << "-----------" << endl;
    cout << "Next:      " << "        " << "Next:      " << endl;
    if (nextBlock != nullptr) {
        for (int i=0; i < nextBlock->getShape().size(); i++) {
            for (int j=0; j < nextBlock->getShape()[0].size(); j++) {
                cout << nextBlock->getShape()[i][j];
            }
            cout << endl;
        }
    } else if (opponent.nextBlock != nullptr) {
        for (int i=0; i < opponent.nextBlock->getShape().size(); i++) {
            cout << "                   ";
            for (int j=0; j < opponent.nextBlock->getShape()[0].size(); j++) {
                cout << opponent.nextBlock->getShape()[i][j];
            }
            cout << endl;
        }
    }
    cout << endl;
}

// clear rows that are full
// droppedBlocks keeps track of the blocks that have been dropped
int Board::clearRows(vector<unique_ptr<Block>>& droppedBlocks) {
    bool didClearRows = false;
    int clearedRows = 0;
    vector<int> rowsToClear;
    for (int i=BOARD_HEIGHT-1; i>=0; i--) {
        bool fullRow = true;
        for (int j=0; j<BOARD_WIDTH; j++) {
            if (grid[i][j] == ' ') {
                fullRow = false;
                break;
            }
        }
        if (fullRow) {
            clearedRows++;
            rowsToClear.push_back(i);
            didClearRows = true;
        }
    }

    if(level == 4 && didClearRows) {
        resetBlocksDroppedSinceClear();
    }

    // Find the rows occupied by each dropped block
    // If the rows it occupied have been cleared, update the score accordingly
    for (auto row : rowsToClear) {
        for (auto& blockPtr : droppedBlocks) {
            Block &block = *blockPtr;
            vector<int> &occupiedRows = block.getOccupiedRows();
            for (int& oc : occupiedRows) {
                if (oc == row) {
                    oc = -1;
                } else if (oc < row) {
                    if (std::find(rowsToClear.begin(), rowsToClear.end(), oc) == rowsToClear.end()) {
                        oc++;
                    }
                }
            }
            occupiedRows.erase(
                    remove_if(occupiedRows.begin(), occupiedRows.end(),
                              [](int row) { return row == -1; }),
                    occupiedRows.end());

            if (occupiedRows.empty()) {
                updateScore(block);
            }
        }
    }

    // Remove the blocks whose rows occupied have been cleared from droppedBlocks
    droppedBlocks.erase(remove_if(droppedBlocks.begin(), droppedBlocks.end(),
                                  [](const unique_ptr<Block>& block) {
                                      return block->getOccupiedRows().empty();
                                  }),
                        droppedBlocks.end());

    // shift all blocks downwards
    int count = 0;
    for (auto row : rowsToClear) {
        for (int k = row+count; k > 0; k--) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                grid[k][j] = grid[k - 1][j];
            }
        }

        // Fill top row with empty spaces
        for (int j = 0; j < BOARD_WIDTH; j++) {
            grid[0][j] = ' ';
        }
        count++;
    }

    if (clearedRows > 0) {
        updateScoreLinesCleared(clearedRows);
    }
    return clearedRows;
}

void Board::setLevel(int newLevel) {
    level = newLevel;
}

void Board::setScore(int newScore) {
    score = newScore;
}

void Board::updateBlockPosition(Block &block) {
    int row = block.getTopLeftRow();
    int col = block.getTopLeftCol();
    auto shape = block.getShape();

    for (int i=0; i<shape.size(); i++) {
        for (int j=0; j<shape[i].size(); j++) {
            if (grid[row+i][col+j] == ' ') {
                grid[row+i][col+j] = shape[i][j];
            }
        }
    }
}

void Board::eraseBlock(Block &block) {
    int row = block.getTopLeftRow();
    int col = block.getTopLeftCol();
    auto shape = block.getShape();

    for (int i=0; i<shape.size(); i++) {
        for (int j=0; j<shape[i].size(); j++) {
            if (shape[i][j] != ' ') {
                grid[row+i][col+j] = ' ';
            }
        }
    }
}

char Board::getValue(int r, int c) {
    return grid[r][c];
}

// update score when a line has been cleared
void Board::updateScoreLinesCleared(int linesCleared) {
    score += (level + linesCleared) * (level + linesCleared);
    updateHiScore();
}

// update score when a block has been fulled cleared
void Board::updateScore(const Block& block) {
    score += (block.getLevelGenerated() + 1) * (block.getLevelGenerated() + 1);
    updateHiScore();
}

void Board::updateHiScore() {
    if (score > hiScore) {
        hiScore = score;
    }
}

int Board::getLevel() {
    return level;
}

int Board::getScore() {
    return score;
}

int Board::getHiScore() {
    return hiScore;
}

void Board::restart() {
    for (int i=0; i<BOARD_HEIGHT; i++) {
        for (int j=0; j<BOARD_WIDTH; j++) {
            grid[i][j] = ' ';
        }
    }
    level = 0;
    score = 0;
}

bool Board::getIsBlind() {
    return isBlind;
}

void Board::setIsBlind(bool blind) {
    isBlind = blind;
}