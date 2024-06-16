#include "board.h"
#include "blockfactory.h"
#include "block.h"
#include "windows.h"
#include <iostream>
#include <unordered_map>
#include <sstream>

const unordered_map<string, string> validCommands = {
        {"left", "lef"},
        {"right", "ri"},
        {"down", "do"},
        {"clockwise", "cl"},
        {"counterclockwise", "co"},
        {"drop", "dr"},
        {"levelup", "levelu"},
        {"leveldown", "leveld"},
        {"norandom", "n"},
        {"random", "ra"},
        {"sequence", "s"},
        {"restart", "re"}
};

// extract the multiplier from the user input
// remove it from the userInput string
int extractMultiplier(string& userInput) {
    stringstream ss(userInput);
    int multiplier = 0;
    int mult_num_digits = 1;
    char c;
    ss >> c;
    if (isdigit(c)) {
        if (c == '0') {
            userInput = userInput.substr(1);
            return 0;
        }
        else multiplier = c - '0';
    } else {
        return 1;
    }
    while (ss >> c && isdigit(c)) {
        multiplier = multiplier * 10 + (c - '0');
        mult_num_digits++;
    }
    userInput = userInput.substr(mult_num_digits);
    return multiplier;
}

// process each command and return its multiplier
// and its full command name
pair<int, string> processCommand(string& userInput) {
    string command;
    if (userInput.size() == 1) {
        if (userInput == "I" || userInput == "J" || userInput == "L" || userInput == "O" ||
            userInput == "S" || userInput == "Z" || userInput == "T") {
            command = userInput;
        } else {
            command = "invalid";
        }
        return make_pair(1, command);
    }

    int multiplier = extractMultiplier(userInput);
    for (auto& pair : validCommands) {
        if (pair.second.size() > userInput.size() ||
            pair.first.size() < userInput.size()) continue;
        bool found = true;
        for (int i=0; i<userInput.size(); i++) {
            if (userInput.at(i) != pair.first.at(i)) found = false;
        }
        if (found) {
            command = pair.first;
            break;
        }
    }

    return make_pair(multiplier, command);
}

std::unordered_map<char, int> blockColors = {
        {'I', Xwindow::Blue},
        {'J', Xwindow::Red},
        {'L', Xwindow::Green},
        {'O', Xwindow::Yellow},
        {'S', Xwindow::Cyan},
        {'Z', Xwindow::Magenta},
        {'T', Xwindow::Black}
};

void drawGame(Xwindow& window, Board& board, Board& board_op,
              unique_ptr<Block>& nextBlock, unique_ptr<Block>& nextBlock_op) {
    window.fillRectangle(40, 40, 960, 80, Xwindow::White);
    window.drawString(40, 60, "Level: " + to_string(board.getLevel()));
    window.drawString(40, 80, "Score: " + to_string(board.getScore()));
    window.drawString(40, 100, "High Score: " + to_string(board.getHiScore()));
    window.drawString(560, 60, "Level: " + to_string(board_op.getLevel()));
    window.drawString(560, 80, "Score: " + to_string(board_op.getScore()));
    window.drawString(560, 100, "High Score: " + to_string(board_op.getHiScore()));
    const int blocksize = 40;
    const int offset = 2;

    // draw the vertical lines for the board grid
    for (int i=40; i<=480; i+=blocksize) {
        window.fillRectangle(i, 120, offset, 720, Xwindow::Black);
    }

    for (int i=560; i<=1000; i+=blocksize) {
        window.fillRectangle(i, 120, offset, 720, Xwindow::Black);
    }

    // draw the horizontal lines for the board grid
    for (int i=120; i<=840; i+=blocksize) {
        window.fillRectangle(40, i, 440, offset, Xwindow::Black);
    }

    for (int i=120; i<=840; i+=blocksize) {
        window.fillRectangle(560, i, 440, offset, Xwindow::Black);
    }


    // draw the blocks on the first player's board
    // check if the first player's board is blind or not and display and according display
    for (int i=0; i<board.BOARD_HEIGHT; i++) {
        for (int j=0; j<board.BOARD_WIDTH; j++) {
            if (board.getValue(i, j) == '*') {
                window.fillRectangle(40 + blocksize * j + offset, 120 + blocksize * i + offset, blocksize - offset, blocksize - offset, Xwindow::White);
                window.drawString(40 + blocksize * j + 16, 120 + blocksize * i + 26, "*");
            } else if (board.getValue(i, j) != ' ' && (!board.getIsBlind() || i < 2 || i >= 12 || j < 2 || j >= 9)) {
                window.fillRectangle(40 + blocksize * j + offset, 120 + blocksize * i + offset, blocksize - offset, blocksize - offset, blockColors[board.getValue(i, j)]);
            } else if (board.getIsBlind() && j >= 2 && j < 9 && i >= 2 && i < 12) {
                window.fillRectangle(40 + blocksize * j + offset, 120 + blocksize * i + offset, blocksize - offset, blocksize - offset, Xwindow::White);
                window.drawString(40 + blocksize * j + 16, 120 + blocksize * i + 26, "?");
            } else {
                window.fillRectangle(40 + blocksize * j + offset, 120 + blocksize * i + offset, blocksize - offset, blocksize - offset, Xwindow::White);
            }
        }
    }

    // draw the blocks on the second player's board
    // check if the second player's board is blind or not and display and according display
    for (int i=0; i<board_op.BOARD_HEIGHT; i++) {
        for (int j=0; j<board_op.BOARD_WIDTH; j++) {
            if (board_op.getValue(i, j) == '*') {
                window.fillRectangle(560 + blocksize * j + offset, 120 + blocksize * i + offset, blocksize - offset, blocksize - offset, Xwindow::White);
                window.drawString(560 + blocksize * j + 16, 120 + blocksize * i + 26, "*");
            } else if (board_op.getValue(i, j) != ' ' && (!board_op.getIsBlind() || i < 2 || i >= 12 || j < 2 || j >= 9)) {
                window.fillRectangle(560 + blocksize * j + offset, 120 + blocksize * i + offset, blocksize - offset, blocksize - offset, blockColors[board_op.getValue(i, j)]);
            } else if (board_op.getIsBlind() && j >= 2 && j < 9 && i >= 2 && i < 12) {
                window.fillRectangle(560 + blocksize * j + offset, 120 + blocksize * i + offset, blocksize - offset, blocksize - offset, Xwindow::White);
                window.drawString(560 + blocksize * j + 16, 120 + blocksize * i + 26, "?");
            }
            else {
                window.fillRectangle(560 + blocksize * j + offset, 120 + blocksize * i + offset, blocksize - offset, blocksize - offset, Xwindow::White);
            }
        }
    }

    // erase and redraw next block display
    window.fillRectangle(40, 860, 240, 600, Xwindow::White);
    window.fillRectangle(560, 860, 240, 600, Xwindow::White);
    window.drawString(40, 860, "Next: ");
    window.drawString(560, 860, "Next: ");

    // draw the next block for the player whose turn it is
    if (board.getNextBlock() != nullptr) {
        for (int i=0; i<nextBlock->getShape().size(); i++) {
            for (int j=0; j<nextBlock->getShape()[0].size(); j++) {
                if (nextBlock->getShape()[i][j] != ' ') {
                    window.fillRectangle(40 + blocksize * j + offset, 880 + blocksize * i + offset, blocksize - offset, blocksize - offset, blockColors[nextBlock->getShape()[i][j]]);
                }
            }
        }
    } else {
        for (int i=0; i<nextBlock_op->getShape().size(); i++) {
            for (int j=0; j<nextBlock_op->getShape()[0].size(); j++) {
                if (nextBlock_op->getShape()[i][j] != ' ') {
                    window.fillRectangle(560 + blocksize * j + offset, 880 + blocksize * i + offset, blocksize - offset, blocksize - offset, blockColors[nextBlock_op->getShape()[i][j]]);
                }
            }
        }
    }

}

bool handlePlayerTurn(bool& eof, Board& board, Board& board_op, unique_ptr<BlockFactory>& blockFactory,
                      unique_ptr<Block>& currentBlock, unique_ptr<Block>& nextBlock,
                      unique_ptr<Block>& currentBlock_op, vector<unique_ptr<Block>>& droppedBlocks, string sequenceFile,
                      bool turn, Block* nextBlock_op, Xwindow* window=nullptr) {
    bool turnFinished = false;
    while (!turnFinished) {
        string userInput;
        cout << "Enter command: " << endl;
        if(!(cin >> userInput)) {
            eof = true;
            return true;
        }
        pair<int, string> result = processCommand(userInput);

        if (result.first == 0) return false;
        userInput = result.second;
        if (userInput == "left") {
            for (int i=0; i<result.first; i++) {
                currentBlock->moveLeft(board);
            }
        } else if (userInput == "right") {
            for (int i=0; i<result.first; i++) {
                currentBlock->moveRight(board);
            }
        } else if (userInput == "down") {
            for (int i=0; i<result.first; i++) {
                currentBlock->moveDown(board);
            }
        } else if (userInput == "drop") {
            bool lost = false;
            int cr = 0;
            for (int i=0; i<result.first; i++) {
                currentBlock->drop(board);
                droppedBlocks.push_back(move(currentBlock));

                cr = board.clearRows(droppedBlocks);
                if (board.getLevel() == 4 && board.getBlocksDroppedSinceClear() == 5) {
                    currentBlock = blockFactory->createQBlock(board.getLevel());
                    currentBlock->drop(board);
                    droppedBlocks.push_back(move(currentBlock));
                    board.resetBlocksDroppedSinceClear();
                }

                // create the next block
                currentBlock = move(nextBlock);
                nextBlock = blockFactory->createBlock(board.getLevel());
                board.setNextBlock(nextBlock.get());
                lost = !(currentBlock->placeBlock(board));
            }
            board.setNextBlock(nullptr);
            board_op.setNextBlock(nextBlock_op);
            turnFinished= true;
            if (board.getIsBlind()) {
                board.setIsBlind(false);
            }
            if (cr >= 2) {
                cout << "Special action triggered. Enter desired action: " << endl;
                string specialaction;
                cin >> specialaction;
                if (specialaction == "force") {
                    string type;
                    cin >> type;
                    board_op.eraseBlock(*currentBlock_op);
                    BlockType forcedBlockType = getBlockType(type);
                    currentBlock_op = blockFactory->createSpecificBlock(forcedBlockType, board_op.getLevel());
                    currentBlock_op->placeBlock(board_op);
                } else if (specialaction == "heavy") {
                    currentBlock_op->setIsHeavy(true);
                } else if (specialaction == "blind") {
                    board_op.setIsBlind(true);
                }
            }
            if (turn) {
                board.display(board_op);
                if (window != nullptr) {
                    drawGame(*window, board, board_op, nextBlock, reinterpret_cast<unique_ptr<Block> &>(nextBlock_op));
                }
            } else {
                board_op.display(board);
                if (window != nullptr) {
                    drawGame(*window, board_op, board, reinterpret_cast<unique_ptr<Block> &>(nextBlock_op), nextBlock);
                }
            }
            return lost;
        } else if (userInput == "clockwise") {
            for (int i=0; i<result.first; i++) {
                currentBlock->rotate(board, "c");
            }
        } else if (userInput == "counterclockwise") {
            for (int i=0; i<result.first; i++) {
                currentBlock->rotate(board, "cc");
            }
        } else if (userInput == "levelup" || userInput == "leveldown") {
            for (int i=0; i<result.first; i++) {
                if (board.getLevel() < 4 && userInput == "levelup") {
                    board.setLevel(board.getLevel()+1);
                } else if (board.getLevel() > 0 && userInput == "leveldown") {
                    board.setLevel(board.getLevel()-1);
                }
                blockFactory.reset();
                switch (board.getLevel()) {
                    case 0:
                        blockFactory = std::make_unique<Level0>(sequenceFile);
                        break;
                    case 1:
                        blockFactory = std::make_unique<Level1>();
                        break;
                    case 2:
                        blockFactory = std::make_unique<Level2>();
                        break;
                    case 3:
                        blockFactory = std::make_unique<Level3>();
                        break;
                    case 4:
                        blockFactory = std::make_unique<Level4>();
                        break;
                }
            }
        } else if ((board.getLevel() == 3
                    && (userInput == "norandom" || userInput == "random"))) {
            if (userInput == "norandom") {
                string filename; cin >> filename;
                blockFactory = make_unique<Level3>(filename);
            } else {
                blockFactory = make_unique<Level3>();
            }
        } else if ((board.getLevel() == 4
                    && (userInput == "norandom" || userInput == "random"))) {
            if (userInput == "norandom") {
                string filename;
                cin >> filename;
                blockFactory = make_unique<Level4>(filename);
            } else {
                blockFactory = make_unique<Level4>();
            }
        } else if (userInput == "restart") {
            board.restart();
            blockFactory = std::make_unique<Level0>(sequenceFile);
            currentBlock = blockFactory->createBlock(board.getLevel());
            currentBlock->placeBlock(board);
        } else if (userInput.size() == 1) {
            BlockType blockType = getBlockType(userInput);
            board.eraseBlock(*currentBlock);
            currentBlock = blockFactory->createSpecificBlock(blockType, board.getLevel());
            currentBlock->placeBlock(board);
        }
        if (turn) {
            board.display(board_op);
            if (window != nullptr) {
                drawGame(*window, board, board_op, nextBlock, reinterpret_cast<unique_ptr<Block> &>(nextBlock_op));
            }
        } else {
            board_op.display(board);
            if (window != nullptr) {
                drawGame(*window, board_op, board, reinterpret_cast<unique_ptr<Block> &>(nextBlock_op), nextBlock);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    bool textOnly = false;
    int seed = -1;
    string scriptfile1, scriptfile2;
    int startLevel = 0;

    for (int i=1; i<argc; i++) {
        string arg = argv[i];
        if (arg == "-text") {
            textOnly = true;
        } else if (arg == "-seed" && i + 1 < argc) {
            seed = stoi(argv[++i]);
        } else if (arg == "-scriptfile1" && i + 1 < argc) {
            scriptfile1 = argv[++i];
        } else if (arg == "-scriptfile2" && i + 1 < argc) {
            scriptfile2 = argv[++i];
        } else if (arg == "-startlevel" && i + 1 < argc) {
            startLevel = std::stoi(argv[++i]);
        }
    }

    bool eof = false;
    int currLevel = startLevel;
    if (scriptfile1 == "") {
        scriptfile1 = "sequence1.txt";
    }
    if (scriptfile2 == "") {
        scriptfile2 = "sequence2.txt";
    }
    Board board;
    Board board_op;

    unique_ptr<BlockFactory> blockFactory;
    unique_ptr<BlockFactory> blockFactory_op;

    if (currLevel == 0) {
        blockFactory = make_unique<Level0>(scriptfile1);
        blockFactory_op = make_unique<Level0>(scriptfile2);
    } else if (currLevel == 1) {
        blockFactory = make_unique<Level1>();
        blockFactory_op = make_unique<Level1>();
    } else if (currLevel == 2) {
        blockFactory = make_unique<Level2>();
        blockFactory_op = make_unique<Level2>();
    } else if (currLevel == 3) {
        blockFactory = make_unique<Level3>(scriptfile1);
        blockFactory_op = make_unique<Level3>(scriptfile2);
    } else if (currLevel == 4) {
        blockFactory = make_unique<Level4>(scriptfile1);
        blockFactory_op = make_unique<Level4>(scriptfile2);
    }

    // Create first block
    unique_ptr<Block> currentBlock = blockFactory->createBlock(currLevel);
    currentBlock->placeBlock(board);
    unique_ptr<Block> nextBlock = blockFactory->createBlock(currLevel);
    board.setNextBlock(nextBlock.get());
    vector<unique_ptr<Block>> droppedBlocks;
    board.setLevel(currLevel);

    unique_ptr<Block> currentBlock_op = blockFactory_op->createBlock(currLevel);
    currentBlock_op->placeBlock(board_op);
    unique_ptr<Block> nextBlock_op = blockFactory_op->createBlock(currLevel);
    vector<unique_ptr<Block>> droppedBlocks_op;
    board_op.setLevel(currLevel);

    board.display(board_op);


    bool turn = true;

    if (textOnly) {
        while (true) {
            if (turn) {
                if (handlePlayerTurn(eof, board, board_op, blockFactory, currentBlock, nextBlock, currentBlock_op, droppedBlocks, scriptfile1, turn,
                                     nextBlock_op.get(), nullptr)) {
                    cout << "Game Ended: Player1 lost" << endl;
                }
            } else {
                if (handlePlayerTurn(eof, board_op, board, blockFactory_op, currentBlock_op, nextBlock_op, currentBlock, droppedBlocks_op, scriptfile2, turn,
                                     nextBlock.get(), nullptr)) {
                    cout << "Game Ended: Player2 lost" << endl;
                }
            }
            if (eof) {
                break;
            }
            turn = !turn;
        }
    } else {
        Xwindow window(1080, 1040);
        drawGame(window, board, board_op, nextBlock, nextBlock_op);
        while (true) {
            if (turn) {
                if (handlePlayerTurn(eof, board, board_op, blockFactory, currentBlock, nextBlock, currentBlock_op, droppedBlocks, scriptfile1, turn, nextBlock_op.get(), &window)) {
                    cout << "Game Ended: Player1 lost" << endl;
                }
            } else {
                if (handlePlayerTurn(eof, board_op, board, blockFactory_op, currentBlock_op, nextBlock_op, currentBlock, droppedBlocks_op, scriptfile2, turn, nextBlock.get(), &window)) {
                    cout << "Game Ended: Player2 lost" << endl;
                }
            }
            if (eof) {
                break;
            }
            turn = !turn;
        }
    }
}
