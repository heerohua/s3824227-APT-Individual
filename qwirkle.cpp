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
void startNewGame(bool &quit);
void startNewGameAI(bool &quit, unsigned int randSeed);
void loadGame(bool &quit);
void showCredits();
void handleMenuChoice(int choice, bool &quit, unsigned int randSeed);
void playTurn(Player *player, Player *opponent, TileBag *tileBag, GameBoard *board, bool &quit);
void playAITurn(Player *player, Player *opponent, TileBag *tileBag, GameBoard *board, bool &quit);
void playAIFirstTurn(Player *player, Player *opponent, TileBag *tileBag, GameBoard* gameBoard, bool &quit);
void gameLoop(Player *player1, Player *player2, TileBag *tileBag, GameBoard *board);
void printScores(Player *player1, Player *player2, TileBag *tileBag, GameBoard* GameBoard, bool &quit);
std::string handleInput(bool &quit);

int main(int argc, char **argv)
{
  bool quit = false;
  bool versusAI = false;
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
  }

  displayWelcomeMessage();

  while (!quit && !versusAI)
  {
    displayMainMenu();
    std::string input = handleInput(quit);

    if(!quit) {
      // Convert input to an integer
      try
      {
        int choice = std::stoi(input);
        handleMenuChoice(choice, quit, randSeed);
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
    }
  }

  if (versusAI){
    std::cout << "- Versus AI Mode -" << std::endl;
    startNewGameAI(quit, randSeed);
    // startNewGameAI(quit, 1); // REMOVE AFTER TESTING COMPELTE
  }

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
  std::cout << "3. Credits" << std::endl;
  std::cout << "4. Quit" << std::endl;
  std::cout << "> ";
}

void startNewGame(bool &quit, unsigned int randSeed)
{
  std::cout << "Enter a name for player 1 (uppercase characters only)" << std::endl;
  std::cout << "> ";
  std::string player1Name = handleInput(quit);

  while (!InputValidator::isValidName(player1Name) && !quit)
  {
    std::cout << "Invalid name. Please enter uppercase letters only." << std::endl;
    std::cout << "> ";
    player1Name = handleInput(quit);
  }

  std::cout << "Enter a name for player 2 (uppercase characters only)" << std::endl;
  std::cout << "> ";
  std::string player2Name = handleInput(quit);

  while (!InputValidator::isValidName(player2Name) && !quit)
  {
    std::cout << "Invalid name. Please enter uppercase letters only." << std::endl;
    std::cout << "> ";
    player2Name = handleInput(quit);
  }

  Player player1(player1Name);
  Player player2(player2Name);
  
  GameBoard gameBoard(NUM_BOARD_ROWS, NUM_BOARD_COLS);

  TileBag tileBag;
  // Shuffle the tile bag
  tileBag.shuffle(randSeed);

  std::cout << "Let's Play!" << std::endl;

  // Draws 6 tiles for each player to start the game
  player1.drawQuantityTiles(&tileBag, STARTING_HAND_SIZE);
  player2.drawQuantityTiles(&tileBag, STARTING_HAND_SIZE);

  // Primary functions used to run recursive gameplay operations
  gameLoop(&player1, &player2, &tileBag, &gameBoard);
}

void startNewGameAI(bool &quit, unsigned int randSeed)
{
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
  
  GameBoard gameBoard(NUM_BOARD_ROWS, NUM_BOARD_COLS);

  TileBag tileBag;
  // Shuffle the tile bag
  tileBag.shuffle(randSeed);

  std::cout << "Let's Play!" << std::endl;

  // Draws 6 tiles for each player to start the game
  player1.drawQuantityTiles(&tileBag, STARTING_HAND_SIZE);
  player2.drawQuantityTiles(&tileBag, STARTING_HAND_SIZE);

  // Primary functions used to run recursive gameplay operations
  gameLoop(&player1, &player2, &tileBag, &gameBoard);
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
        gameLoop(loadedPlayer1, loadedPlayer2, loadedTileBag, loadedBoard);
      } else
      {
        gameLoop(loadedPlayer2, loadedPlayer1, loadedTileBag, loadedBoard);
      }
    }
    
    delete loadedPlayer1;
    delete loadedPlayer2;
    delete loadedTileBag;
    delete loadedBoard;
    delete currentPlayer;
}

void playTurn(Player *player, Player *opponent, TileBag *tileBag, GameBoard* gameBoard, bool &quit)
{
  bool validInput = false;
  while (!validInput && !quit)
  {
    std::cout << gameBoard->displayBoard() << std::endl;
    std::cout << "Tiles in hand: " << player->getHand()->toString() << std::endl;
    std::cout << "Your move " << player->getName() << ": ";
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
      
      FileHandler fileHandler;
      fileHandler.saveGame(filename, player, opponent, tileBag, gameBoard, player);
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
        Tile *removedTile = player->removeTileFromHand(tile);
        if (removedTile != nullptr)
        {
          std::cout << removedTile->print() << " tile removed from hand and added to the bag." << std::endl;
          delete removedTile;
          tileBag->addTile(tile);
          Tile *newTile = tileBag->drawTile();
          if (newTile != nullptr)
          {
            player->addTileToHand(newTile);
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
        if (player->containsTile(tile)) 
        {
          if (Rules::validateMove(gameBoard, tile, row, col))
          {
            gameBoard->placeTile(row, col, tile);
            Tile* removedTile = player->removeTileFromHand(tile);
            if (removedTile != nullptr)
            {
              delete removedTile;
              Tile* newTile = tileBag->drawTile();
              if (newTile != nullptr)
              {
                player->addTileToHand(newTile);
              }
              int score = Rules::calculateScore(gameBoard, row, col);
              player->setScore(player->getScore() + score);
              if (score > 6)
              {
                std::cout << "QWIRKLE!!!" << std::endl;
              }
              validInput = true;
            }
            else
            {
              std::cout << "Error: Failed to remove tile from hand." << std::endl;
            }
          }
          else
          {
            std::cout << "Invalid move. Try again." << std::endl;
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


void gameLoop(Player *player1, Player *player2, TileBag *tileBag, GameBoard* gameBoard)
{
  int turn = 0;
  bool quit = false;
  while (!quit)
  {
    // If game is AI vs AI this ensures player 1 makes a valid 1st move
    if (player1->getName() == "AI" && turn == 0){
      printScores(player1, player2, tileBag, gameBoard, quit);
      playAIFirstTurn(player1, player2, tileBag, gameBoard, quit);
    }

    if (!quit && player1->getName() != "AI")
    {
    printScores(player1, player2, tileBag, gameBoard, quit);
    playTurn(player1, player2, tileBag, gameBoard, quit);
    } else {
      // CALL AI ENGINE HERE
      if (player1->getName() == "AI" && turn >0 && !quit){
        printScores(player1, player2, tileBag, gameBoard, quit);
        playAITurn(player1, player2, tileBag, gameBoard, quit);
      }
    }
    
    if (!quit && player2->getName() != "AI")
    {
      printScores(player1, player2, tileBag, gameBoard, quit);
      playTurn(player2, player1, tileBag, gameBoard, quit);
    }
    else {
      // CALL AI ENGINE HERE 
      if (!quit){
        playAITurn(player2, player1, tileBag, gameBoard, quit);
      }      
    }
    turn++;
  }
}

// Structures used to contain information related to potential moves for AI players
struct AIMoveList {
  Tile* tile;
  int row;
  int col;
  int score;

  AIMoveList(Tile* tile, int row, int col, int score) : tile(tile), row(row), col(col), score(score) {}
};

void playAIFirstTurn(Player *player, Player *opponent, TileBag *tileBag, GameBoard* gameBoard, bool &quit){
  LinkedList* playerHand = player->getHand();
  
  std::cout << "ATTEMPTING TO PLACE FIRST TILE FOR AI." << std::endl;

  char tileColour = playerHand->get(0)->getColour();
  char tileShape = playerHand->get(0)->getShape();
  Tile* tile = new Tile(tileColour,tileShape);
  gameBoard->placeTile(13,13,tile);
  Tile* removedTile = player->removeTileFromHand(tile);
  if (removedTile != nullptr)
  {
    delete removedTile;
    Tile* newTile = tileBag->drawTile();
    if (newTile != nullptr)
    {
      player->addTileToHand(newTile);
    }
    int score = Rules::calculateScore(gameBoard, 13,13);
    player->setScore(player->getScore() + score);
    if (score > 6)
    {
      std::cout << "QWIRKLE!!!" << std::endl;
    }
  }

}

void playAITurn(Player *player, Player *opponent, TileBag *tileBag, GameBoard* gameBoard, bool &quit)
{

  std::vector<AIMoveList> validMoves;
  LinkedList* playerHand = player->getHand();

  if (!playerHand->getLength()){
    std::cout << "I hope you enjoyed playing qwirkle versus AI. Please re-launch the application to play again." << std::endl << std::endl << "Exiting application...." << std::endl;
    return;
  }

  for (int row = 0; row < 26; row++){
    for (int col = 0; col < 26; col++){
      if (gameBoard->getTile(row,col) != nullptr){
        // std::cout << "TILE FOUND AT : " << row << " " << col << std::endl;
        // std::cout << "tileOptions size: " << playerHand->getLength() << std::endl;
        for (int tileOption = 0; tileOption < playerHand->getLength(); tileOption++){
          char tileColour = playerHand->get(tileOption)->getColour();
          char tileShape = playerHand->get(tileOption)->getShape();
          int score = 0;
          Tile* tile = new Tile (tileColour,tileShape);

          // std::cout << "Player hand, tile option: " << tileOption << " - is: " << tile->print() << std::endl;
          
          if (Rules::validateMove(gameBoard, tile, row+1, col)){
            // std::cout << "Valid move found at Row:" << row+1 << ", Col: " << col <<  "." << std::endl; 
            score = Rules::calculateScore(gameBoard, row+1, col);
            validMoves.push_back(AIMoveList(tile, row+1, col, score));          
          }

          if (Rules::validateMove(gameBoard, tile, row-1, col)){
            // std::cout << "Valid move found at Row:" << row-1 << ", Col: " << col <<  "." << std::endl; 
            score = Rules::calculateScore(gameBoard, row-1, col);
            validMoves.push_back(AIMoveList(tile, row-1, col, score));  
          }

          if (Rules::validateMove(gameBoard, tile, row, col+1)){
            // std::cout << "Valid move found at Row:" << row << ", Col: " << col+1 <<  "." << std::endl;
            score = Rules::calculateScore(gameBoard, row, col+1);
            validMoves.push_back(AIMoveList(tile, row, col+1, score));   
          }

          if (Rules::validateMove(gameBoard, tile, row, col-1)){
            // std::cout << "Valid move found at Row:" << row << ", Col: " << col-1 <<  "." << std::endl; 
            score = Rules::calculateScore(gameBoard, row, col-1);
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
  // std::cout << "Move " << i << ": is " << validMoves[i].tile->print() << " - Score: " << validMoves[i].score << std::endl;
    if (validMoves[i].score > highestScore){
      // std::cout << "New high score! for move " << i << " with a value of: " << validMoves[i].score << std::endl;
      highestScore = validMoves[i].score;
      highestScoreIndexValue = i;
    }
  }

  // std::cout << "The move with the highest score was: " << validMoves[highestScoreIndexValue].tile->print() << " and the highest score was " << highestScore << " at ROW " << validMoves[highestScoreIndexValue].row << ", COL " << validMoves[highestScoreIndexValue].col << std::endl;
  std::cout << "AI Player's hand: " << playerHand->toString() << std::endl;

  // Replace first tile in hand if no moves available
  if (moveListSize == 0 && !quit && playerHand->getLength() > 0){

    std::cout << "ATTEMPTING TO REPLACE TILE FOR AI." << std::endl;

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
      int score = Rules::calculateScore(gameBoard, validMoves[highestScoreIndexValue].row, validMoves[highestScoreIndexValue].col);
      player->setScore(player->getScore() + score);
      if (score > 6)
      {
        std::cout << "QWIRKLE!!!" << std::endl; // REMOVE AND ADD TO RULES -  THIS IS ACTUALLY INCORRECT SCORING
      }
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

void handleMenuChoice(int choice, bool &quit, unsigned int randSeed)
{
  if (choice == 1)
  {
    startNewGame(quit, randSeed);
  }
  else if (choice == 2)
  {
    loadGame(quit);
  }
  else if (choice == 3)
  {
    showCredits();
  }
  else if (choice == 4)
  {
    quit = true;
  }
  else
  {
    std::cout << "Invalid choice. Please try again." << std::endl;
  }
}

void printScores(Player* player1, Player* player2, TileBag *tileBag, GameBoard *gameBoard, bool &quit) {
    if (Rules::isGameOver(player1, player2, tileBag))
    {
      std::cout << gameBoard->displayBoard() << std::endl;
      Player* winner = player1->getScore() > player2->getScore() ? player1 : player2;
      std::cout << "\nGame over!" << std::endl;
      std::cout << "The winner is " << winner->getName() << " with a score of " << winner->getScore() << "!\n" << std::endl;
      
      quit = true;
      return;
    }
    std::cout << std::endl; 
    std::cout << "Score for " << player1->getName() << ": " << player1->getScore() << std::endl;
    std::cout << "Score for " << player2->getName() << ": " << player2->getScore() << std::endl;
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