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

# Individual changelog 

## 13/8/24

## qwirkle.cpp
- Modified main body to include a "versusAI" boolean and additional argv if statement to modify initial application behaviour to bypass initial menus and load game state to begin a new game with player1 versus AI
- Implemented startNewGameAI function to handle AI Games. 
	- Player 1 initialisation and Player 2 automatically set to AI
	- Initialises game state for Player 1 & 2 before starting the gameloop
- Modified gameLoop to manage calls to "playAITurn" function if the player(s) name is "AI", otherwise turns are performed manually as per usual
	- Added functionality to enable AI vs AI games, which required the inclusion of a turn counter to ensure player1 AI plays the first turn
	- gameLoop now manages AI turns correctly in player vs AI or AI vs AI games
- Created a new structure called "AIMoveList" to record available valid AI moves with the tile, row, col, and score for the move to help streamline the AI's logic
- Implemented playAIFirstTurn function to ensure in AI vs AI games, player 1 always plays the first turn and doesn't error out looking for valid moves around existing board tiles
	- Will play the first move as AI if player1 is also AI to ensure the game will proceed accordingly without errors
- Implemented playAITurn function which handles all AI move related logic
	- Loops through all valid combinations of available moves on the gameboard and the AI's hand, and records the move and corresponding score to the vector "validMoves"
	- Secondary loop through "validMoves" to find the recorded move with the highest score to choose as the AI's preferred move choice. Records index for re-use
	- If the AI has tiles in their hand and no valid moves to play, the AI will "replace" a tile, discarding their HEAD tile to the TileBag and retrieving a new tile, added the new tile to the end of their tile list
	- AI will "place" their highest scoring validated move onto the board and all game rules/logic/interactions will proceed as though AI is a player and turns will alternate
	- Modified announced actions for AI to reflect they are for the AI, rather than "player"
- Fixed minor bug introduced to testing with changes

# Remaining ToDo - Individual

- Fix qwirkle scoring logic. Remove output from place tile (both player and AI sections) and move to Rules section as cardinal checks already exists here. 
- Remove debugging / bloated comments
- Add appropriate commenting to newly added P1 vs AI and AI vs AI features
- Implement a P1 vs AI and AI vs AI set of automated tests using a specific seed 
- Check if anything is missing from changelog with regards to feature implementation
- MAJOR - Add Player 3 and Player 4  
- MAJOR - Add place multiple tiles (*If there's time to do extra milestones*)
- MINOR - Colour 
- MINOR - Better Invalid Input 
- MINOR - Help
- MINOR - Hints
- MINOR - High Scores