#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Tile.h"
#include "TileBag.h"
#include "Player.h"
#include "LinkedList.h"
#include "FileHandler.h"
#include "GameBoard.h"
#include "Rules.h"
#include "InputValidator.h"
#include "Student.h"
#include "StudentInfo.h"
#include "Rules.h"
#include "Tests.cpp"
#include <random>

#define EXIT_SUCCESS 0
#define NUM_BOARD_ROWS 26
#define NUM_BOARD_COLS 26
#define STARTING_HAND_SIZE 6

// Function prototypes
void displayWelcomeMessage();
void displayMainMenu();
void gameOptions(int randSeed, bool versusAI, bool colourMode);
void mainMenu(bool &quit, unsigned int randSeed, bool toggleAI, bool toggleColour);
void startNewGame(bool &quit,  bool versusAI, bool colourMode);
void startNewGameAI(bool &quit, unsigned int randSeed, bool versusAI, bool colourMode);
void loadGame(bool &quit);
void showCredits();
void handleMenuChoice(int choice, bool &quit, unsigned int randSeed, bool versusAI, bool colourMode);
void playTurn(Player player, std::vector<Player> playerList, TileBag *tileBag, GameBoard *board, bool &quit);
void playAITurn(Player *player, Player *opponent, TileBag *tileBag, GameBoard *board, bool &quit, unsigned int randSeed, bool toggleAI, bool toggleColour);
void playAIFirstTurn(Player *player, Player *opponent, TileBag *tileBag, GameBoard* gameBoard, bool &quit);
void gameLoop(Player currentPlayer, std::vector<Player> playerList, TileBag *tileBag, GameBoard *board, unsigned int randSeed, bool versusAI, bool colourMode);
void printScores(std::vector<Player> playerList, TileBag *tileBag, GameBoard* GameBoard, bool &quit, bool colourMode);
std::string handleInput(bool &quit);

int main(int argc, char **argv)
{
  bool quit = false;
  bool versusAI = false;
  bool colourMode = false;
  int randSeed = (unsigned int)time(NULL);
  
  if (argc > 1) {
    if (std::string(argv[1]) == "test") {
      // run unit tetsts
      Tests::run();
      return EXIT_SUCCESS;
    }
    if (std::string(argv[1]) == "e2etest") {
      randSeed = 0;
    }
    if (std::string(argv[1]) == "--ai"){
    versusAI = true;
    }
    if (std::string(argv[1]) == "--colour"){
    colourMode = true;
    }
  }

  displayWelcomeMessage();
  mainMenu(quit, randSeed, versusAI, colourMode);
  
  return EXIT_SUCCESS;
}

void displayWelcomeMessage()
{
  std::cout << "Welcome to Qwirkle!" << std::endl;
  std::cout << "-------------------" << std::endl;
}

void displayMainMenu()
{
  std::cout << "Menu" << std::endl;
  std::cout << "1. New Game" << std::endl;
  std::cout << "2. Load Game" << std::endl;
  std::cout << "3. Game Options" << std::endl;
  std::cout << "4. Credits" << std::endl;
  std::cout << "5. Quit" << std::endl;
  std::cout << "> ";
}

void gameOptions(int randSeed, bool versusAI, bool colourMode){
  std::string optionStatusAI = "\x1b[41mOff\x1b[0m";
  std::string optionStatusColour = "\x1b[41mOff\x1b[0m";
  bool quit = false;
  bool toggleAI = false;
  bool toggleColour = false;

  if (versusAI){
    optionStatusAI = "\x1b[42mOn\x1b[0m";
    toggleAI = true;
  }
  if (colourMode){
    optionStatusColour = "\x1b[42mOn\x1b[0m";
    toggleColour = true;
  }
  std::cout << "Game Options" << std::endl;
  std::cout << "1. Play versus AI - Status: " << optionStatusAI << std::endl;
  std::cout << "2. Toggle Game Colour On/Off  - Status: " << optionStatusColour << std::endl;
  std::cout << "3. Return to main menu" << std::endl;
  std::cout << "> ";
  std::string input = handleInput(quit);
  int choice = std::stoi(input);

  if (choice == 1)
  {
    if (versusAI){
    toggleAI = false;
    } else {
    toggleAI = true;
    }
    gameOptions(randSeed, toggleAI, toggleColour);
  }
  else if (choice == 2)
  {
    if (colourMode){
    toggleColour = false;
    } else {
    toggleColour = true;
    }
    gameOptions(randSeed, toggleAI, toggleColour);
  }
  else if (choice == 3)
  {
    mainMenu(quit, randSeed, toggleAI, toggleColour);
  }  
  else
  {
    std::cout << "Invalid choice. Please select an option between 1 and 3." << std::endl;
  }
}

void mainMenu(bool &quit, unsigned int randSeed, bool toggleAI, bool toggleColour){
  while (!quit)
  {
    if(!quit) {
      displayMainMenu();
      std::string input = handleInput(quit);
      
      try
      {
        // Convert input to an integer
        int choice = std::stoi(input);
        handleMenuChoice(choice, quit, randSeed, toggleAI, toggleColour);
      }
      catch (const std::invalid_argument &)
      {
        std::cout << "Invalid input. Please enter a number between 1 and 4." << std::endl;
      }
      catch (const std::out_of_range &)
      {
        std::cout << "Invalid input. Number is out of range. Please enter a number between 1 and 4." << std::endl;
      }
    }  
    if(quit) {
      std::cout << "Goodbye!" << std::endl;
      exit(EXIT_SUCCESS);
    }
  }
}

void startNewGame(bool &quit, unsigned int randSeed,  bool versusAI, bool colourMode)
{
  if (versusAI){
    startNewGameAI(quit, randSeed, versusAI, colourMode);
  }

  std::vector<Player> playerList;
  int numPlayers = 0; 

  if (versusAI && numPlayers == 0){
    return;
  }

  while (numPlayers == 0){
    std::cout << "Please select number of players (2-4)" << std::endl;
    std::cout << "> ";
    std::string selection = handleInput(quit); 
    if (std::stoi(selection) >1 && std::stoi(selection) <5){
      numPlayers = std::stoi(selection);
    } else {
      std::cout << "Invalid selection. Please enter a number between 2 and 4." << std:: endl;
    }
  }

  GameBoard gameBoard(NUM_BOARD_ROWS, NUM_BOARD_COLS);

  TileBag tileBag;
  // Shuffle the tile bag
  tileBag.shuffle(randSeed);

  for (int i = 0; i < numPlayers; i++){

    std::cout << "Enter a name for player " << i+1 << " (uppercase characters only)" << std::endl;
    std::cout << "> ";
    std::string playerName = handleInput(quit);
    while (!InputValidator::isValidName(playerName) && !quit)
      {
        std::cout << "Invalid name. Please enter uppercase letters only." << std::endl;
        std::cout << "> ";
        playerName = handleInput(quit);
      }
    playerList.push_back(playerName);
    playerList[i].drawQuantityTiles(&tileBag, STARTING_HAND_SIZE);
  }

  // Instantiates the 1st player as the player to go first
  Player currentPlayer = playerList[0];

  std::cout << "Let's Play!" << std::endl;

  gameLoop(currentPlayer, playerList, &tileBag, &gameBoard, randSeed, versusAI, colourMode); 
}

void startNewGameAI(bool &quit, unsigned int randSeed, bool versusAI, bool colourMode)
{
  std::vector<Player> playerList; 
  GameBoard gameBoard(NUM_BOARD_ROWS, NUM_BOARD_COLS);
  TileBag tileBag;
  // Shuffle the tile bag
  tileBag.shuffle(randSeed);
  
  std::cout << "Starting a new game - AI ENABLED - " << std::endl;
  std::cout << "Enter a name for player 1 (uppercase characters only)" << std::endl;
  std::cout << "> ";
  std::string player1Name = handleInput(quit);

  while (!InputValidator::isValidName(player1Name) && !quit)
  {
    std::cout << "Invalid name. Please enter uppercase letters only." << std::endl;
    std::cout << "> ";
    player1Name = handleInput(quit);
  }

  std::cout << "Player 2 will be played by 'AI'. Good luck!" << std::endl;

  Player player1(player1Name);
  Player player2("AI");

  playerList.push_back(player1);
  playerList[0].drawQuantityTiles(&tileBag, STARTING_HAND_SIZE);

  playerList.push_back(player2);
  playerList[1].drawQuantityTiles(&tileBag, STARTING_HAND_SIZE);

  std::cout << "Let's Play!" << std::endl;

  Player currentPlayer = playerList[0];

  // Primary functions used to run recursive gameplay operations
  gameLoop(currentPlayer, playerList, &tileBag, &gameBoard, randSeed, versusAI, colourMode);
}

void loadGame(bool& quit) {

    std::cout << "Enter the filename from which to load a game:" << std::endl;
    std::cout << "> ";
    std::string filename = handleInput(quit);

    if (!quit && !InputValidator::isFileNameValid(filename)) {
        std::cerr << "Error: Invalid file name or format." << std::endl;
        return;
    }

    FileHandler fileHandler;

    if (!fileHandler.fileExists(filename)) {
        std::cerr << "Error: File does not exist." << std::endl;
        return;
    }

    // Load the game state
    Player* loadedPlayer1 = new Player("Temp1");
    Player* loadedPlayer2 = new Player("Temp2");
    TileBag* loadedTileBag = new TileBag();
    GameBoard* loadedBoard = new GameBoard(); 
    Player* currentPlayer = new Player("Current");

    bool gameLoaded = fileHandler.loadGame(filename, loadedPlayer1, loadedPlayer2, loadedTileBag, loadedBoard, currentPlayer);
    
    if (!gameLoaded) {
        std::cerr << "Error: Invalid file format." << std::endl;
    } else {
      std::cout << "Qwirkle game successfully loaded" << std::endl;

      if (currentPlayer->getName() == loadedPlayer1->getName())
      {
        // gameLoop(loadedPlayer1, loadedPlayer2, loadedTileBag, loadedBoard, randSeed, versusAI, colourMode); // TEMP DISABLE
      } else
      {
        // gameLoop(loadedPlayer2, loadedPlayer1, loadedTileBag, loadedBoard, randSeed, versusAI, colourMode); // TEMP DISABLE
      }
    }
    
    delete loadedPlayer1;
    delete loadedPlayer2;
    delete loadedTileBag;
    delete loadedBoard;
    delete currentPlayer;
}

void playTurn(Player &player, std::vector<Player> &playerList, TileBag *tileBag, GameBoard *gameBoard, bool &quit, bool colourMode)
{
  bool validInput = false;
  while (!validInput && !quit)
  {
    std::cout << gameBoard->displayBoard(colourMode) << std::endl;
    std::cout << "Tiles in hand: " << player.getHand()->toString() << std::endl;
    std::cout << "Your move " << player.getName() << ": ";
    std::string playerMove = handleInput(quit);

    if (playerMove == "quit" || quit)
    {
        quit = true;
    }
    else if (playerMove == "save")
    {
        // Implement save game logic
        std::cout << "Enter filename to save: ";
        std::string filename = handleInput(quit);
        
        // FileHandler fileHandler;  // TEMP DISABLE
        // fileHandler.saveGame(filename, player, opponent, tileBag, gameBoard, player); // TEMP DISABLE
        std::cout << "Game saved to " << filename << std::endl;
    }
    else if (playerMove.substr(0, 7) == "replace")
    {
      std::string tileToReplace = playerMove.substr(8);
      // Ensure input is valid
      if (tileToReplace.size() == 2)
      {
        char colour = tileToReplace[0];
        int shape = tileToReplace[1] - '0';
        Tile *tile = new Tile(colour, shape);
        Tile *removedTile = player.removeTileFromHand(tile);
        if (removedTile != nullptr)
        {
            std::cout << removedTile->print() << " tile removed from hand and added to the bag." << std::endl;
            delete removedTile;
            tileBag->addTile(tile);
            Tile *newTile = tileBag->drawTile();
            if (newTile != nullptr)
            {
                player.addTileToHand(newTile);
                std::cout << newTile->print() << " tile drawn and added to your hand." << std::endl;
            }
            else
            {
                std::cout << "No tiles left to draw from the tile bag." << std::endl;
            }
            validInput = true;
        }
        else
        {
            std::cout << "You don't have that tile in your hand." << std::endl;
        }
      }
      else
      {
          std::cout << "Invalid tile format. Use <colour><shape>." << std::endl;
      }
    }
    else
    {
      std::stringstream stringstream(playerMove);
      std::string extractedWord;
      std::vector<std::string> moveBreakdown;

      while (stringstream >> extractedWord)
      {
          moveBreakdown.push_back(extractedWord);
      }

      if (moveBreakdown.size() == 4 && moveBreakdown[0] == "place" && moveBreakdown[2] == "at")
      {
        char tileColour = moveBreakdown[1][0];
        int tileShape = moveBreakdown[1][1] - '0';
        char rowChar = moveBreakdown[3][0];
        int col = std::stoi(moveBreakdown[3].substr(1));

        int row = rowChar - 'A';

        Tile* tile = new Tile(tileColour, tileShape);

        // Checks if the tile exists in the player's hand
        if (player.containsTile(tile)) 
        {
            if (Rules::validateMove(gameBoard, tile, row, col))
            {
                // Place the tile on the board
                gameBoard->placeTile(row, col, tile);

                // Remove the tile from the player's hand
                Tile* removedTile = player.removeTileFromHand(tile);
                if (removedTile != nullptr)
                {
                    delete removedTile;

                    // Draw a new tile from the bag
                    Tile* newTile = tileBag->drawTile();
                    if (newTile != nullptr)
                    {
                        player.addTileToHand(newTile);
                    }

                    // Calculate the score for this move
                    int score = Rules::calculateScore(gameBoard, row, col, player.getName());
                    player.setScore(player.getScore() + score);

                    std::cout << "Score for this move: " << score << std::endl;
                    std::cout << "Total score for " << player.getName() << ": " << player.getScore() << std::endl;

                    validInput = true;
                }
                else
                {
                    std::cout << "Error: Failed to remove tile from hand." << std::endl;
                }
            }
            else
            {
                std::cout << "Invalid move. Try making a valid move using the command 'place <tile> at <board location>'" << std::endl;
                delete tile;
            }
        } 
        else 
        {
            std::cout << "You don't have that tile in your hand." << std::endl;
            delete tile;
        }
      }
      else
      {
          std::cout << "Invalid move format. Use 'place <tile> at <position>'." << std::endl;
      }
    }
  }
}


// new gameloop
void gameLoop(Player currentPlayer, std::vector<Player> playerList, TileBag *tileBag, GameBoard *gameBoard, unsigned int randSeed, bool versusAI, bool colourMode) {
    int turn = 0;
    bool quit = false;
    int numPlayers = playerList.size();

    while (!quit) {
        for (int i = 0; i < numPlayers; i++) {
            Player &player = playerList[i];
            printScores(playerList, tileBag, gameBoard, quit, colourMode);

            if (player.getName() == "AI" && gameBoard->isEmpty()) {
                playAIFirstTurn(&player, &playerList[(i + 1) % numPlayers], tileBag, gameBoard, quit);
            } else if (!quit && player.getName() != "AI") {
                playTurn(player, playerList, tileBag, gameBoard, quit, colourMode);
            } else if (!quit) {
                playAITurn(&player, &playerList[(i + 1) % numPlayers], tileBag, gameBoard, quit, randSeed, versusAI, colourMode);
            }

            if (quit) {
                break;
            }
        }

        turn++;
    }
}

// Structure is used to contain requisite information related to potential moves for AI players
struct AIMoveList {
  Tile* tile;
  int row;
  int col;
  int score;

  AIMoveList(Tile* tile, int row, int col, int score) : tile(tile), row(row), col(col), score(score) {}
};

void playAIFirstTurn(Player *player, Player *opponent, TileBag *tileBag, GameBoard* gameBoard, bool &quit){
  LinkedList* playerHand = player->getHand();
  char tileColour = playerHand->get(0)->getColour();
  char tileShape = playerHand->get(0)->getShape();
  Tile* tile = new Tile(tileColour,tileShape);
  gameBoard->placeTile(9,9,tile);
  Tile* removedTile = player->removeTileFromHand(tile);

  if (removedTile != nullptr)
  {
    delete removedTile;
    Tile* newTile = tileBag->drawTile();
    if (newTile != nullptr)
    {
      player->addTileToHand(newTile);
    }
    int score = Rules::calculateScore(gameBoard, 13,13, player->getName());
    player->setScore(player->getScore() + score);
  }
}

void playAITurn(Player *player, Player *opponent, TileBag *tileBag, GameBoard* gameBoard, bool &quit, unsigned int randSeed, bool toggleAI, bool toggleColour)
{
  std::vector<AIMoveList> validMoves;
  LinkedList* playerHand = player->getHand();

  if (!playerHand->getLength()){
    return;
  }
  for (int row = 0; row < 26; row++){
    for (int col = 0; col < 26; col++){
      if (gameBoard->getTile(row,col) != nullptr){
        for (int tileOption = 0; tileOption < playerHand->getLength(); tileOption++){
          char tileColour = playerHand->get(tileOption)->getColour();
          char tileShape = playerHand->get(tileOption)->getShape();
          int score = 0;
          Tile* tile = new Tile (tileColour,tileShape);
          
          if (Rules::validateMove(gameBoard, tile, row+1, col)){
            score = Rules::calculateScore(gameBoard, row+1, col, player->getName());
            validMoves.push_back(AIMoveList(tile, row+1, col, score));          
          }

          if (Rules::validateMove(gameBoard, tile, row-1, col)){
            score = Rules::calculateScore(gameBoard, row-1, col, player->getName());
            validMoves.push_back(AIMoveList(tile, row-1, col, score));  
          }

          if (Rules::validateMove(gameBoard, tile, row, col+1)){
            score = Rules::calculateScore(gameBoard, row, col+1, player->getName());
            validMoves.push_back(AIMoveList(tile, row, col+1, score));   
          }

          if (Rules::validateMove(gameBoard, tile, row, col-1)){
            score = Rules::calculateScore(gameBoard, row, col-1, player->getName());
            validMoves.push_back(AIMoveList(tile, row, col-1, score));  
          }
        }        
      }
    }
  }

  int highestScore = 0;
  int highestScoreIndexValue = 0;
  int moveListSize = validMoves.size();

  for (int i = 0; i < moveListSize; i++){
    if (validMoves[i].score > highestScore){
      highestScore = validMoves[i].score;
      highestScoreIndexValue = i;
    }
  }

  std::cout << "AI Player's hand: " << playerHand->toString() << std::endl;

  // Replace first tile in hand if no moves available
  if (moveListSize == 0 && !quit && playerHand->getLength() > 0){

    char tileColour = playerHand->get(0)->getColour();
    char tileShape = playerHand->get(0)->getShape();
    Tile* tile = new Tile(tileColour,tileShape);
    Tile *removedTile = player->removeTileFromHand(tile);
    if (removedTile != nullptr)
    {
      std::cout << removedTile->print() << " tile removed from AI's hand and added to the bag." << std::endl;
      delete removedTile;
      tileBag->addTile(tile);
      Tile *newTile = tileBag->drawTile();
      if (newTile != nullptr)
      {
        player->addTileToHand(newTile);
        std::cout << newTile->print() << " tile drawn and added to AI's hand." << std::endl;
      }
      else
      {
        std::cout << "Attempted to replace a tile, however there are no tiles left to draw from the tile bag." << std::endl;
      }
    }
  }
  else
  {
    gameBoard->placeTile(validMoves[highestScoreIndexValue].row, validMoves[highestScoreIndexValue].col, validMoves[highestScoreIndexValue].tile);
    Tile* removedTile = player->removeTileFromHand(validMoves[highestScoreIndexValue].tile);
    if (removedTile != nullptr)
    {
      delete removedTile;
      Tile* newTile = tileBag->drawTile();
      if (newTile != nullptr)
      {
        player->addTileToHand(newTile);
      }
      int score = Rules::calculateScore(gameBoard, validMoves[highestScoreIndexValue].row, validMoves[highestScoreIndexValue].col, player->getName());
      player->setScore(player->getScore() + score);
    }
    else
    {
      std::cout << "Error: Failed to remove tile from hand." << std::endl;
    }
  }  
}

void showCredits()
{
  std::cout << "---------------------------------------" << std::endl;
  for (const Student &student : students)
  {
    student.display();
  }
  std::cout << "---------------------------------------" << std::endl;
}

void handleMenuChoice(int choice, bool &quit, unsigned int randSeed, bool versusAI, bool colourMode)
{
  if (choice == 1)
  {
    startNewGame(quit, randSeed, versusAI, colourMode);
  }
  else if (choice == 2)
  {
    loadGame(quit);
  }
  else if (choice == 3)
  {
    gameOptions(randSeed, versusAI, colourMode);
  }  
  else if (choice == 4)
  {
    showCredits();
  }
  else if (choice == 5)
  {
    quit = true;
    return;
  }
  else
  {
    std::cout << "Invalid choice. Please enter a menu choice between 1 and 5." << std::endl;
  }
}

void printScores(std::vector<Player> playerList, TileBag *tileBag, GameBoard *gameBoard, bool &quit, bool colourMode) {
    int listSize = playerList.size();

    if (Rules::isGameOver(playerList, tileBag))
    {
      std::cout << gameBoard->displayBoard(colourMode) << std::endl;
      Player winner = playerList[0];

      for (int i = 1; i < listSize; i++){
        if (playerList[i].getScore() > winner.getScore()){
          winner = playerList[i];
        }
      }

      std::cout << "\nGame over!" << std::endl;
      std::cout << "The winner is " << winner.getName() << " with a score of " << winner.getScore() << "!\n" << std::endl;
      
      quit = true;
      return;
    }
    std::cout << std::endl; 

    for (int i = 0; i < listSize; i++){
      std::cout << "Score for " << playerList[i].getName() << ": " << playerList[i].getScore() << std::endl;
    }
}

std::string handleInput(bool &quit)
{
  std::string input;
  std::getline(std::cin, input);
  std::cout << std::endl;

  // Check for EOF
  if (std::cin.eof())
  {
    quit = true;
  }
  return input;
}