# 3D Tic-Tac-Toe

## Concept and Rules

3D tic-tac-toe is similar to its 2D counterpart, with the difference being that 3D tic-tac-toe is played on a cube of order 3 (3 X 3 X 3). This cube consists of 3 grids of squares of order 3x3 stacked upon each other. Our goal remains the same, i.e. to get 3-in-a-row but here, it can be on any of the levels or even between the layers. The player must line up their symbol (X or O) in three colinear blocks out of 27 blocks in the cube that line up vertically, horizontally, diagonally on a single grid or spaced evenly over all three grids. 

We can relate this game to a 3D magic cube concept, the same way we use the concept of magic square to create an AI for 2D tic-tac-toe. In case of 2D Tic-Tac-Toe, we check if 3 same symbols (X or O) form a collinear line by adding up the corresponding magic numbers and checking if the sum is equal to the magic constant  (in case of magic square of order 3, magic constant = 15). The corresponding magic numbers of the positions marked by the players are stored in separate lists called UserList and ComputerList, respectively. These lists are then checked iteratively after each move to check if the opponent is winning or if the AI itself is winning. Traditionally, the player to form such a combination first wins.

<img src="https://github.com/gssasank/Magic-Cube/blob/main/media/example.png" alt="example" style="zoom:67%;" />

To get "3-in-a-row" (a win) in 3D tic-tac-toe, one must put their symbol(X or O) in a way that those 3 blocks add up to form the magic constant (in our case 42) and are collinear. As mentioned above, the three points can be rows, columns, pillars or major diagonals.  The diagonals on the 6 faces of the magic cube do not add up to form the magic constant and hence, are not counted as a win (3-in-a-row) even though they are collinear. The first player to score 10 3-in-a-row's is considered the winner in the game.

## Core Algorithm

As mentioned above, the AI for playing 3D tic-tac-toe is very similar to its 2D counterpart. The core algorithm is almost the same.

```
1. The computer checks ComputerList and UserList to check whether either player is winning or not.
2. If computer is winning in the next move, it enters the value X or O in the appropriate block.
3. If the computer has not winning position and if human is going to win in the next move,computer blocks it.
4. If neither player has any winning move, then the computer makes a valid second move such that its chances of getting three in a row increases.
5. These steps repeat until either one player has 10 or more positions or the entire board is completely full.
```

## Detailed Functions 

### The Core Functions used are:

- **display():** Used to display the game board by printing all the rows, columns and layers in the 3D array iteratively. It also prints the Scores of each player at that point of time and also displays the UserList and ComputerList.

  

- **user_input():** Takes input from user in the form [Layer, Row, Column]. If the block that user wishes to occupy is filled then a warning in shown to replay the move otherwise the move is accepted.

  

- **possWin():** Iterates through all the possible combinations inside a given list(user/computer) and decides whether the computer has to play a winning move if possible or block the user from making a valid move. If size of the list is less than two, the list is not checked.



- **winningCombination():** The input list is scanned to check whether any three combinations of magic numbers in the list add up to 42. If they do then their collinearity is checked. Return true if condition is satisfied. If the conditions are satisfied, then the magic numbers are inserted in a vector as a single entity, and the number of elements in the vector is returned as the number of winning combinations present in the list. It essentially returns the number of wins for each list.

  

- **makeTwo():** Function called by the computer when neither UserList or ComputerList has any winning or blocking positions.  Takes the moves already played by the computer and scans entire rows,columns,pillars and major diagonals associated with them to  make a second move that increase chances of victory.

  

- **makeMove():** Sends user and computer list to be checked for the computer to decide its winning move or block the human move. If neither of the conditions are not applicable, computer starts checking rows, columns, pillars and major diagonals to make a valid move.

   

- **startPlayingComputer():** Function is invoked when the game starts and decided when it has to be stopped. If computer goes first, enter in 1,1,1. If human does not play in 1,1,1 enter there or else enter in 0,0,0.  After taking user input makeMove is invoked and computer checks if  it is winning (possWin) then the winning move is played,  if not, we check if opponent is winning and if he is, find winning position and block the move. If no one is winning, then makeTwo is invoked (in makeMove itself) and the scores of human and computer are updated.   

### The Helper Functions used are:

- **mod():** This function was added to prevent overflow of array positions outside of its initialised size.It was required since -1%3 is reported as -1 by the C++ language instead of 2. 

  

- **isFull():** Used to check whether the board is full or not. Returns true if at least one empty block is present and false if no empty blocks are present. This is done by iteratively searching the board for empty positions.

  

- **isAvailable():** Checks whether a position is available or taken. When user or computer tries to make move at a certain position, the function checks whether or not that position is taken.

  

- **go():** Similar to its 2D counterpart, the function is used to enter value (1 or 2) in particular position. It takes the value and coordinates as input and enters the value at said coordinates. 1 is used to represent human move and 2 is used to represent move made by computer. 

- **collinear():** Checks whether three points are collinear or not.



- **getCoordinate():** It is used to return coordinates of a number (magic number) in the magic cube in l,r,c format as a vector The function loops through each block iteratively while trying to match different values inside the array elements value with the value of the magic number provided. 

  

- **goMagicNumber():** The function is used to enter value (1 or 2) at the position of magic number inside the playing board. 1 is used to represent human move and 2 is used to represent move made by computer. 

  

- **check_if_already_in():** The function checks whether the value passed to it is already present in the UserList or ComputerList. 

  

- **checkRows():** Computer checks for available spaces in the row where its current magic number under inspection is.              Computer gives priority to rows where the entire row(the two remaining spaces) are empty. Priority then shifts to any available space inside the row. 

  

- **checkColumns():** Computer checks for available spaces in the column where its current magic number under inspection is. Computer gives priority to columns where the entire column(the two remaining spaces) are empty. Priority then shifts to any available space inside the column.

  

- **checkPillars():** Computer checks for available spaces in the pillar where its current magic number under inspection is.  		 Computer gives priority to pillars where the entire pillar(the two remaining spaces) are empty. Priority then shifts to any available space inside the pillar.

  

- **checkMajorDiagonals():** Computer checks for available spaces in the major diagonals where its current magic number under inspection is.  Computer gives priority to diagonals where the entire diagonal(the two remaining spaces) are empty. Priority then shifts to any available space inside the diagonal.

  

- **performChecks():** Module used to call on the different checks mentioned above collectively.



### makeTwo() and possWin() explained:

makeTwo() and possWin() are the two corner stones of the program and are by far the most complex ones. By invoking other helper functions, they decide which all moves the computer makes, whether it is to block the human from winning or making its own moves. The biggest difference from their 2D counterparts is that in 2D tic-tac-toe the functions only check if the sum equals magic constant or if the future sum will be equal to the magic constant and then make moves accordingly. In the case of 3D tic-tac-toe, along with the magic constant we also check for collinearity as we may have conditions where the sum is the magic constant, yet the elements are not collinear.

#### makeTwo():

```
1. Iterate through ComputerList and invoke performChecks() fucntion on each element of the list.
2. perfromChecks() then iterates through major diagonals, rows, columns and pillars and checks for available spaces for the computer by iterating through all the major diagonals, rows, columns and pillars.
3. Each sub-function then checks for available points and checks the collinearity condition iteratively. Once a vacant position is found, it is immediately occupied and the ComputerList is updated.
```

#### possWin():

```1
1. Iterate through ComputerList and find all possible combinations of two elements in a magic cube. 
2. Difference between sum both the elements and the magic constant is found (42- (a+b)). It is checked whether the result is available or not and if it is, then we check if it is collinear or not.
3. If both conditions are satisfied, we have a winning move and the value is placed into the tic-tac-toe board and the ComputerList.
4. If no winning move for computer is found, iterate through ComputerList and find all possible combinations of two elements in a magic cube.
5. The same steps as the ComputerList are followed and if it is found that there is a winning condition for the user, then the move is blocked by the computer.
```

