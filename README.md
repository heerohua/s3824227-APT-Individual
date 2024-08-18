# Advanced-Programming-A2 Group 4
Advanced-Programming-A2

Save/Load Game File Format: An assumption has been made by them team and the file input/output validation has been scoped to include only files with the .txt extension. This has been done to ensure that the save file has a valid extension that ensures operational consistency across multiple platforms

To Compile: 

 `make`<br>
 
OR<br>
 g++ -Wall -Wextra -std=c++14 -O -o qwirkle qwirkle.cpp Tile.cpp Node.cpp LinkedList.cpp TileBag.cpp Player.cpp FileHandler.cpp Rules.cpp InputValidator.cpp Student.cpp GameBoard.cpp Tests.cpp
 
To clean up: `make clean`
 
Run unit tests: `./qwirkle.exe test`

Make tests executabe and run: `chmod +x ./tests/run && ./tests/run`

Or run individually:<br>
 `chmod +x ./tests/credits/test && ./tests/credits/test` Test credits menu<br>
 `chmod +x ./tests/qwirkle/test && ./tests/qwirkle/test` Test scoring a QWIRKLE<br>
 `chmod +x ./tests/load-game/test && ./tests/load-game/test` Test loading game<br>
 `chmod +x ./tests/new-game/test && ./tests/new-game/test` Test create new game<br>
 `chmod +x ./tests/save-game/test && ./tests/save-game/test` Test saving game<br>
 `chmod +x ./tests/game-end/test && ./tests/game-end/test` Test game ends when tiles run out<br>
 `chmod +x ./tests/line-validation/test && ./tests/line-validation/test` Test tile placement is valid based on neighboring tiles<br>

# Milestone 3 - Gameplay Enhancements Completed
- MAJOR: Player1 versus AI (alt) AI versus AI (supports 2-4 player AI games)
- MAJOR: Multiplayer  			
- MINOR: Coloured tiles 
- MINOR: Better invalid input 

# New Commands & Features

## AI 
- You can enable this feature at runtime by using "./qwirkle.exe --ai" or while in-game through the Game Options (Main Menu 3) and toggling this feature on/off. 
- This feature is also accessible manually by naming any player "AI". This features works for 2-4 players. 
- If AI is enabled in the main menu, multiplayer will inherently be disabled. Turn this feature off and manually name players "AI" should you wish to play any combination of AI and multiplayer content. 

## Multiplayer
- You can enable this feature at runtime by using "./qwirkle.exe --mp" or while in-game through the Game Options (Main Menu 3) and toggling this feature on/off. 
- Multiplayer supports between 2 and 4 players. Users will be prompted for their desired number of players once they attempt to begin a new game. 
- Multiplayer is disabled by default if the "AI" is enabled. Toggle AI off and manually name players "AI" should you wish to play any combination of AI and multiplayer content. 

## Coloured Tiles
- You can enable this feature at runtime by using "./qwirkle.exe --colour" or while in-game through the Game Options (Main Menu 3) and toggling this feature on/off. 
- This feature will provide vibrant coloured tiles matching their base "colour type" instead of the default terminal colours defined by the users system

## Better Invalid Input
- This feature is not shown in game and instead refers to appending more meaningful error messagings to user engagement and prompts throughout the application

## Game Options
- The "Game Options" is a new sub-menu from the Main Menu, offering the user access toggle the new game features and enhancement options on and off, while also viewing their current toggled status
- This menu is available from the main menu and selecting menu otion "3"

# Individual changelog 
- New features include the option to play 1v1 versus AI, Multiplayer mode (supporting 2-4 players), coloured tile mode, and to resize the gameboard. 
- Numerous modifications to the existing game structure were required to capture new game modes and functions associated with individual milestones and their associated requirements
- New args when launching qwirkle.exe include '--ai', '--mp', and '--colour'.
- Minor bugfixes to original code
- Refactored QWIRKLE!!! requirements to be in-line with game rules

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
- Updated to latest commands with 'chmod +x' on whole test suite to ensure tests have permission to run
- Updated existing test suite to accomodate new game modes and changes to game/menu structure
- Updated Save Game and Load Game functionality to work with new game modes and enhancements