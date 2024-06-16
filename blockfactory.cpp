#include "blockfactory.h"
#include "blocksubtypes.h"


unique_ptr<Block> BlockFactory::createSpecificBlock(BlockType type, int level) {
    switch (type) {
        case BlockType::I:
            return make_unique<IBlock>(level);
        case BlockType::J:
            return make_unique<JBlock>(level);
        case BlockType::L:
            return make_unique<LBlock>(level);
        case BlockType::O:
            return make_unique<OBlock>(level);
        case BlockType::S:
            return make_unique<SBlock>(level);
        case BlockType::Z:
            return make_unique<ZBlock>(level);
        case BlockType::T:
            return make_unique<TBlock>(level);
        case BlockType::Q:
            return make_unique<SplitBoardBlock>(level);
    }
}

// create a new block that is of type S or Z
unique_ptr<Block> BlockFactory::createSorZBlock(int level) {
    std::random_device rd;
    std::mt19937 randomGenerator(rd());

    uniform_int_distribution<int> distribution(0, 1);
    int blockIndex = distribution(randomGenerator);

    switch (blockIndex) {
        case 0:
            return createSpecificBlock(BlockType::S, level);
        case 1:
            return createSpecificBlock(BlockType::Z, level);
    }
}

// create a new block that is not of type S or Z
unique_ptr<Block> BlockFactory::createExcludeSorZBlock(int level) {
    std::random_device rd;
    std::mt19937 randomGenerator(rd());

    uniform_int_distribution<int> distribution(0, 4);
    int blockIndex = distribution(randomGenerator);

    switch (blockIndex) {
        case 0:
            return createSpecificBlock(BlockType::I, level);
        case 1:
            return createSpecificBlock(BlockType::J, level);
        case 2:
            return createSpecificBlock(BlockType::L, level);
        case 3:
            return createSpecificBlock(BlockType::O, level);
        case 4:
            return createSpecificBlock(BlockType::T, level);
    }
}

double BlockFactory::generateRandomProbability() {
    std::random_device rd;
    std::mt19937 randomGenerator(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(randomGenerator);
}

unique_ptr<Block> BlockFactory::readFromSequenceFile(int level, ifstream& sequenceFile) {
    string blockType;
    if (!(sequenceFile >> blockType)) {
        sequenceFile.clear();
        sequenceFile.seekg(0, ios::beg);
    }
    sequenceFile >> blockType;
    return createSpecificBlock(getBlockType(blockType), level);
}

// Create a board splitting block (used in level 4)
unique_ptr<Block> BlockFactory::createQBlock(int level) {
    return createSpecificBlock(BlockType::Q, level);
}

unique_ptr<Block> Level0::createBlock(int level) {
    return readFromSequenceFile(level, sequenceFile);
}


std::unique_ptr<Block> Level1::createBlock(int level) {
    double probability = generateRandomProbability();
    if (probability < 1.0 / 6.0) {
        return BlockFactory::createSorZBlock(level);
    } else {
        return createExcludeSorZBlock(level);
    }
}

std::unique_ptr<Block> Level2::createBlock(int level) {
    double probability = generateRandomProbability();
    if (probability < 2.0 / 7.0) {
        return createSorZBlock(level);
    } else {
        return createExcludeSorZBlock(level);
    }
}

std::unique_ptr<Block> Level3::createBlock(int level) {
    if (randomness) {
        double probability = generateRandomProbability();
        if (probability < 4.0 / 9.0) {
            return createSorZBlock(level);
        } else {
            return createExcludeSorZBlock(level);
        }
    } else {
        return readFromSequenceFile(level, sequenceFile);
    }
}

std::unique_ptr<Block> Level4::createBlock(int level) {
    if (randomness) {
        double probability = generateRandomProbability();
        if (probability < 4.0 / 9.0) {
            return createSorZBlock(level);
        } else {
            return createExcludeSorZBlock(level);
        }
    } else {
        return readFromSequenceFile(level, sequenceFile);
    }
}