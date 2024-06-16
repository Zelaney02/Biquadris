#ifndef A5_BLOCKFACTORY_H
#define A5_BLOCKFACTORY_H

#include "block.h"
#include <memory>
#include <random>
#include <fstream>

using namespace std;

class BlockFactory {
protected:
    unique_ptr<Block> createSorZBlock(int level);
    unique_ptr<Block> createExcludeSorZBlock(int level);
    unique_ptr<Block> readFromSequenceFile(int level, ifstream& sequenceFile);
    double generateRandomProbability();

public:
    virtual unique_ptr<Block> createBlock(int level) = 0;
    virtual ~BlockFactory() = default;
    unique_ptr<Block> createQBlock(int level);
    unique_ptr<Block> createSpecificBlock(BlockType type, int level);
};

class Level0 : public BlockFactory {
private:
    ifstream sequenceFile;
public:
    Level0(const string& filename) : sequenceFile(filename) {};
    ~Level0() { sequenceFile.close(); };
    unique_ptr<Block> createBlock(int level) override;
};

class Level1 : public BlockFactory {
public:
    std::unique_ptr<Block> createBlock(int level) override;
};

class Level2 : public BlockFactory {
public:
    std::unique_ptr<Block> createBlock(int level) override;
};

class Level3 : public BlockFactory {
private:
    bool randomness = true;
    ifstream sequenceFile;
public:
    Level3(): randomness{true} {}
    Level3(const string &filename) : randomness{false}, sequenceFile(filename) {}
    ~Level3() { sequenceFile.close(); }
    std::unique_ptr<Block> createBlock(int level) override;
};

class Level4 : public BlockFactory {
private:
    bool randomness = true;
    ifstream sequenceFile;
public:
    Level4(): randomness{true} {}
    Level4(const string &filename) : randomness{false}, sequenceFile(filename) {}
    ~Level4() { sequenceFile.close(); }
    std::unique_ptr<Block> createBlock(int level) override;
};

#endif //A5_BLOCKFACTORY_H
