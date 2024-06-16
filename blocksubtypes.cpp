#include "blocksubtypes.h"

IBlock::IBlock(int level) : Block(level){
    Block::topLeftRow = 3;
    Block::topLeftCol = 0;
    shape = { {'I', 'I', 'I', 'I'} };
}


JBlock::JBlock(int level) : Block(level){
    Block::topLeftRow = 2;
    Block::topLeftCol = 0;
    shape = { {'J', ' ', ' '},
              {'J', 'J', 'J'} };
}

LBlock::LBlock(int level) : Block(level) {
    Block::topLeftRow = 2;
    Block::topLeftCol = 0;
    shape = { {' ', ' ', 'L'},
              {'L', 'L', 'L'} };
}

OBlock::OBlock(int level) : Block(level) {
    Block::topLeftRow = 2;
    Block::topLeftCol = 0;
    shape = { {'O', 'O'},
              {'O', 'O'} };
}

SBlock::SBlock(int level) : Block(level) {
    Block::topLeftRow = 2;
    Block::topLeftCol = 0;
    shape = { {' ', 'S', 'S'},
              {'S', 'S', ' '} };
}

ZBlock::ZBlock(int level) : Block(level) {
    Block::topLeftRow = 2;
    Block::topLeftCol = 0;
    shape = { {'Z', 'Z', ' '},
              {' ', 'Z', 'Z'} };
}

TBlock::TBlock(int level) : Block(level) {
    Block::topLeftRow = 2;
    Block::topLeftCol = 0;
    shape = { {'T', 'T', 'T'},
              {' ', 'T', ' '} };
}

SplitBoardBlock::SplitBoardBlock(int level) : Block(level) {
    Block::topLeftRow = 0;
    Block::topLeftCol = 5;
    shape = { {'*'} };
}