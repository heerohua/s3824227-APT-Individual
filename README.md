# Advanced-Programming-A2
Advanced-Programming-A2

Save/Load Game File Format: An assumption has been made by them team and the file input/output validation has been scoped to include only files with the .txt extension. This has been done to ensure that the save file has a valid extension that ensures operational consistency across multiple platforms

To Compile: 

 `make`<br>
 
OR<br>
 g++ -Wall -Wextra -std=c++14 -O -o qwirkle qwirkle.cpp Tile.cpp Node.cpp LinkedList.cpp TileBag.cpp Player.cpp FileHandler.cpp Rules.cpp InputValidator.cpp Student.cpp GameBoard.cpp Tests.cpp
 
To clean up: `make clean`
 
Run unit tests: `./qwirkle.exe test`

Run all end-to-end tests: `./tests/run`

Or run individually:<br>
 `./tests/credits/test` Test credits menu<br>
 `./tests/qwirkle/test` Test scoring a QWIRKLE<br>
 `./tests/load-game/test` Test loading game<br>
 `./tests/new-game/test` Test create new game<br>
 `./tests/save-game/test` Test saving game<br>
 `./tests/game-end/test` Test game ends when tiles run out<br>
 `./tests/line-validation/test` Test tile placement is valid based on neighboring tiles<br>

# Milestone 3 - Gameplay Enhamcents Completed
- MAJOR: Player1 versus AI (alt) AI versus AI (supports 2-4 player AI games)
- MAJOR: Multiplayer  			TODO CONDITIONALS
- MINOR: Coloured tiles 
- MINOR: Better invalid input 

# Individual changelog 
- New features include the option to play 1v1 versus AI, Multiplayer mode (supporting 2-4 players), coloured tile mode, and to resize the gameboard. 
- Numerous modifications to the existing game structure were required to capture new game modes and functions associated with individual milestones and their associated requirements
- New args when launching qwirkle.exe include '--ai', '--mp', and '--colour'.
- Minor bugfixes to original code

## GameBoard 
- DisplayBoard function now accepts a boolean arg to toggle colour mode on / off when printing the gameboard state

## Rules
- isGameOver function has been refactored to cater for multiplayer mode
- calculateScore function modified to include announcements for 'QWIRKLE!!!' appropriately when horizontal/vertical qwirkles are scored. Does not announce for AI to reduce potential for spam during move checks.

## Tile
- Implemented two new functions to support the appropriate colouring of game tiles, should colourMode game options be enabled
- tileStringCOlourOptions function contains a switch case which determines the colour a tile should be using the TileCodes colour definitions
- tileStringToColour returns the appropriate concatenated colour output for a tile being converted to colour

## TileCodes
- Updated definitions to include custom ANSI colours for each tile (approx. colour range referenced from https://talyian.github.io/ansicolors/)

## Qwirkle
- mainMenu function was implemented which accepts args while enabling it to be recalled after changing game options to manage the flow of the game's state 
- handleMenuChoice function was update to accomodate changes to the main game menu and aassociated new routes with new gameplay features and options
- gameOptions function was implemented to serve as a sub-menu choice to enable players to toggle game enhancements on/off without restarting the application
- startNewGame was modified to accept args to manage the game options / enhancements toggled on/off by the user
- startNewGameAI was implemented to manage the initialisation of 1v1 AI game mode without overloading/bloating the existing startNewGame function
- gameLoop function was modified to manage games with the new game modes (AI and Multiplayer) as well as other game enhancements
- playTurn was modified to accomodate new game modes and enhancements 
- playAIFirstTurn function was implemented to accomodate for games where AI is player1 and must choose an appropriate first move 
- AIMoveList structure was implemented to store an array of potential moves for AI when examining game board and move options 
- playAITurn function was implemented to handle the logic associated with AI players making their best possible move based on the gameboard and tiles in hand
- printScores function was updated to accomodate the multiplayer game mode and colour option enhancement

## Tests
- Updated existing test suite to accomodate new game modes
- Imeplemented new tests <<<<<<>>>>>>

# Remaining ToDo - Individual

- Update README to include commands for running new game modes
- Remove debugging / bloated comments
- Add appropriate commenting to newly added features in qwirkle.cpp
- Fix save/load function and re-enable in requisite sections (such as playTurn function)
- Fix tests to accomodate changes in formatting / game modes