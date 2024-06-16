#ifndef A5_BLOCKSUBTYPES_H
#define A5_BLOCKSUBTYPES_H

#include "block.h"
class IBlock : public Block {
public:
    IBlock(int level);
};

class JBlock : public Block {
public:
    JBlock(int level);
};

class LBlock : public Block {
public:
    LBlock(int level);
};

class OBlock : public Block {
public:
    OBlock(int level);
};

class SBlock : public Block {
public:
    SBlock(int level);
};

class ZBlock : public Block {
public:
    ZBlock(int level);
};

class TBlock : public Block {
public:
    TBlock(int level);
};

class SplitBoardBlock : public Block {
public:
    SplitBoardBlock(int level);
};

#endif //A5_BLOCKSUBTYPES_H
