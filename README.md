# CConnect4
*"Connect 4" or "Vier Gewinnt" implemented in Objective-C with SDL2 for fancy custom graphics :)*

### (Detailed) description of the functionality of the program
**What does it do, what is the wanted/expected output?**
- Two player game, players place pieces on a vertical board alternately
- Each player has a particular colour (yellow or red)
- Because the board is vertical, pieces inserted in a given column always drop to the lowest unoccupied row of that column. As soon as a column contains 6 pieces, it is full and no other piece can be placed in the column.
- Winner is who connects four elements, in any direction, so either vertically, horizontally, or diagonally
- The board has 7 columns and 6 rows
- Both players begin with 21 pieces; if all pieces are played and no player was able to connect 4 pieces, the game ends in a draw

**Find around 8 significant parameters that indicate that the program is working well and that can be used to evaluate the program**
- It is possible to play the game with two human players (the computer is not playing, just making the moves that the human players dictate)
- The computer needs to recognize when four pieces of one player are connected and that this players wins the game
- The computer needs to recognize a draw (all pieces are played without a winning player)
- The game will have some graphic visualisation
- The game and the moves need to be displayed correctly in regard to the internal game engine
- It’s fluently playable/reacts fluently to user input
- Statistics of previous games can be stored in form of “highscores” and can be displayed to the user/player
- The game has a menu with several options needed to access all functionality
