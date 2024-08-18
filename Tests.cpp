#include <iostream>
#include <fstream>
#include "TileBag.h"
#include "TileCodes.h"
#include "FileHandler.h"

class Tests
{
    public:
    static void run()
    {
        std::cout << "Running tests\n" << std::endl;
        
        tileBagConstructorOverloadTest();
        tileBagShuffleTest();
        readFileContentTest();
        saveGameTest();
    }
    
    static void tileBagConstructorOverloadTest()
    {
        std::cout << "#tileBagConstructorOverloadTest" << std::endl;
        // given
        std::vector<Tile*> tileVector = std::vector<Tile*>({ new Tile(RED, CIRCLE), new Tile(RED, STAR_4)});
        
        // when
        TileBag *tilebag = new TileBag(tileVector);
        
        // then
        std::string tileString = tilebag->getTiles()->toString();
        std::cout << "Tilebag instantiated with tiles: " << tileString << std::endl;
        
        assert_equality("R1, R2", tileString);
    }
    
    static void tileBagShuffleTest()
    {
        std::cout << "#tileShuffleTest" << std::endl;
        // given
        TileBag *tilebag = new TileBag();        
        LinkedList *tiles = tilebag->getTiles();
        std::string tileString = tiles->toString();
        int randSeed = (unsigned int)time(NULL);
        
        // when
        tilebag->shuffle(randSeed);
        std::string shuffledTileString = tiles->toString();
        
        // then
        std::cout << "Original: " << tileString << std::endl;
        std::cout << "Shuffled: " << shuffledTileString << std::endl;
        
        assert_inequality(tileString, shuffledTileString);
    }
    
    static void readFileContentTest()
    {
        std::cout << "#readFileContentTest" << std::endl;
        // given
        FileHandler fileHandler;
        std::string savedGame = "ABBY\n3\nR1,G2\nBILL\n1\nB3,Y4\n12,12\nR1@A0,G2@B1,B3@C2,Y4@D3\nR1,G2,B3,Y4\nABBY";
        
        // when
         std::string fileContent = fileHandler.readFileContent("tests/stubs/load-game-test-stub.txt");
        
        // then
        std::cout << "File content: " << fileContent << std::endl;
        
        assert_equality(savedGame, fileContent);
    }
    
    static void saveGameTest() 
    {
        std::cout << "#saveGameTest" << std::endl;

        // Given
        Player player1("ALICE");
        Player player2("BOB");

        Tile* tile1 = new Tile('R', 1); // Red, Shape 1
        Tile* tile2 = new Tile('G', 2); // Green, Shape 2
        Tile* tile3 = new Tile('B', 3); // Blue, Shape 3
        Tile* tile4 = new Tile('Y', 4); // Yellow, Shape 4

        player1.addTileToHand(tile1);
        player2.addTileToHand(tile2);

        GameBoard board(6, 6);
        board.placeTile(3, 3, tile3);

        std::vector<Tile*> tiles = { tile4 };
        TileBag* tileBag = new TileBag(tiles);

        Player currentPlayer("ALICE");
        
        // Create player list for saving
        std::vector<Player> playerList = { player1, player2 };

        // Set boolean flags
        bool colourMode = true;
        bool versusAI = false;
        
        // When
        std::string filename = "./tests/stubs/save-game-test-stub.txt";
        FileHandler fileHandler;
        fileHandler.saveGame(filename, &currentPlayer, playerList, tileBag, &board, colourMode, versusAI);

        // Then
        std::string fileContent = fileHandler.readFileContent("tests/stubs/save-game-test-stub.txt");
        std::string expectedContent = "ColourMode:true\nVersusAI:false\nALICE\n0\nR1\nBOB\n0\nG2\n6,6\nB3@D3\nY4\nALICE";
        
        assert_equality(expectedContent, fileContent);
    }

    static void assert_equality(std::string expected, std::string actual)
    {
        if (expected != actual) {
            std::cout << "\033[91m" << "Failed \n" << "\033[0m" << std::endl;
            throw std::runtime_error("Expected: \n" + expected + "\nReceived: \n" + actual);
        } else {
            std::cout << "\033[32m" << "Passed \n" << "\033[0m" << std::endl;
        }
    }
    
    static void assert_inequality(std::string expected, std::string actual)
    {
        if (expected == actual) {
            std::cout << "\033[91m" << "Failed \n" << "\033[0m" << std::endl;
            throw std::runtime_error("Expected: \n" + expected + "\nNot to equal: \n" + actual);
        } else {
            std::cout << "\033[32m" << "Passed \n" << "\033[0m" << std::endl;
        }
    }
};