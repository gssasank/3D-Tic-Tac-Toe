#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <bits/stdc++.h>
#include <algorithm>

#define N 3

int*** cubePtr = NULL;
int*** boardPtr = NULL;
char inp;


using namespace std;


int counterHuman = 0;
int counterComp = 0;
vector<int> userList, computerList;

int mod(int a, int b){
  // this function was added to prevent overflow since -1%3 is reported as -1 by the C++ language
  int r = a%b;
  return r<0?r+b:r;
}

bool isFull(){
  //checks whether the playing board has been completely filled or not
  //the function loops iteratively checking each block and returns false the instant it finds an empty block
  int i,j,k,flag=0;

  for(i=0;i<3;i++){
    for(j=0;j<3;j++){
      for(k=0;k<3;k++){
        if(boardPtr[i][j][k] == 0)
          flag = 1;
      }
    }
  }
  
  if(flag!=0){
    return true;
  }
  return false;
}



void display(int*** cube){
  //module to print board
  //function loops iteratively through each block and prints the element inside
  int i,j,k;

  printf("\n");  

  for(i=0;i<3;i++){
    cout << "Layer: " << i+1 << endl;
    for(j=0;j<3;j++){
      for(k=0;k<3;k++){

        if(cube[i][j][k] == 0)
        cout << " _ ";

        //display scheme is changed according to who makes the first input (human/computer)

        if(inp == 'X'){
          if(cube[i][j][k] == 1){
            cout << " X " ;
            }
          else if(cube[i][j][k] == 2){
            cout << " O " ;
          }
         
        }

        if(inp == 'O'){
          if(cube[i][j][k] == 2){
            cout << " X " ;
            }
          else if(cube[i][j][k] == 1){
            cout << " O " ;
          }
         
        }

        
        }
      printf("\n");
    }
    printf("\n");
  }

//printing of moves made by players in terms of the position's magic number
  cout << "Block(s) visited by user wrt Magic Cube:" << endl;
  for (int i: userList)
    std::cout << i << ' ';
  printf("\n");
  cout << "Block(s) visited by computer wrt Magic Cube:" << endl;
  for (int i: computerList)
    std::cout << i << ' ';
  printf("\n");

  //print scores of both players

  cout << "User score: " << counterHuman << endl;
  cout << "Computer score: " << counterComp;
  printf("\n");
  printf("\n");

}

vector<int> getCoordinate(int value){
  // returns coordinates of a number in the magic cube in l,r,c format as a vector
  // function loops through each block iteratively while trying to match the given value with the value of the magic number inside the cube
  int i,j,k;
  vector<int> arr;
  for(i=0;i<3;i++){
    for(j=0;j<3;j++){
      for(k=0;k<3;k++)
        if (cubePtr[i][j][k] == value){
         arr.push_back(i);
          arr.push_back(j);
           arr.push_back(k);
          return arr;
        }
    } 
  }
  return arr;
}

bool isAvailable(int l,int r, int c){
  //checks whether the block at provided co-ordinates is empty or not
  
  if(boardPtr[l][r][c] == 0){
    return true;
  }
  else{
    return false;
  }
}
void goMagicNumber(int value, int blip){
  //simple function to enter value at the position of magic number inside the playing board (blip)
  //the value entered is 2 if computer plays or 1 if human plays
  vector<int> arr = getCoordinate(blip);
  boardPtr[arr[0]][arr[1]][arr[2]] = value;
}

void go(int value, int l, int r, int c){
  // simple function to enter value at provided co-ordinates
  //removed condition for checking if block occcupied because it is already taken care of in user_input
    boardPtr[l][r][c] = value;

}

bool collinear(int val1,int val2,int val3){
  //checks collinearity of given three points(in terms of magic number of the position)

   vector<int> arr1 = getCoordinate(val1);//arr1=<x1,y1,z1>
   vector<int> arr2 = getCoordinate(val2);//arr2=<x2,y2,z2>
   vector<int> arr3 = getCoordinate(val3);//arr3=<x3,y3,z3>

  int i,j,k;

  for(i=0;i<3;i++){
    arr3[i] -= arr2[i];
    arr2[i]-= arr1[i];    
  }

  //after the above operation
  //arr3=<x3-x2,y3-y2,z3-z2>
  //arr2=<x2-x1,y2-y1,z2-z1>

  //cross-product arr3 cross arr2
  //cross-product of two linear vectors is always zero

  i=arr2[1]*arr3[2] - arr3[1]*arr2[2];
  j=arr3[0]*arr2[2] - arr2[0]*arr3[2];
  k=arr2[0]*arr3[1] - arr3[0]*arr2[1];
  
  if(i==0&&j==0&&k==0)
    return true;
  else
    return false;
}

void user_input(vector<int> &userList){
  //takes care of user input and checks whether user move is valid or not
  int l, r, c;
  cout << "Enter the layer, row, column: " << endl;
  cin >> l >> r >> c;
  int input_l = l-1;
  int input_r = r-1;
  int input_c = c-1;

//if the block that user wishes to occupy is filled then a warning in shown to replay the move otherwise the move is accepted
  if(boardPtr[input_l][input_r][input_c] == 0)
  {    
    go(1,input_l,input_r,input_c);
    userList.push_back(cubePtr[input_l][input_r][input_c]);
  }
  else
  {
    cout << "Invalid Input: Block already Occupied." <<endl;
    user_input(userList);
    
  }
}


bool check_if_already_in(int blip,vector<int> &userList, vector<int> &computerList){
  //checks whether the given difference(blip) is already present in the user or computer list
  int i;
  for(i=0;i<userList.size();i++){
    if(userList[i]==blip)
      return true;
  }
  for(i=0;i<computerList.size();i++){
    if(computerList[i]==blip)
      return true;
  }

  return false;
}


int possWin(vector<int> &list){
  //iterates through all the possible combinations inside a given list(user/computer) and decides whether the computer has to play a winning move if possible or block the user from completing a winning combination
 
  int blip,size,i,j;

  //for userlist.size()<=1 first for loop is skipped

  for(int i = 0; i< list.size()-1; i++){
    for(int j = i+1; j< list.size(); j++){
      blip = 42 - (list[i] + list[j]);

      if(blip > 0 && blip <= 27){
        if(!check_if_already_in(blip,userList,computerList)){          
          if(collinear(blip,list[i],list[j])){
            return blip;
          }
        }
      }
    }
  }
   return 0;
}




int winningCombination(vector<int> &list){

  //the input list is scanned to check whether any three combinations of magic numbers in the list add up to 42.
  //If they do then their collinearity is checked.
  //return true if condition is satisfied
  //if the above conditions are satisfied the magic numbers are inserted in a vector as a single entity
  
  int i,j,k;

  vector<vector<int>> bigArray;

  int size = list.size();
  for(i=0;i<size-2;i++){
    for(j=i+1;j<size-1;j++){
      for(k=j+1;k<size;k++){
        if(list[i]+list[j]+list[k]==42){
          if(collinear(list[i],list[j],list[k])){
            vector<int> temp;
            temp.push_back(i);
            temp.push_back(j);
            temp.push_back(k);
            bigArray.push_back(temp);
          }
        }
      }
    }
  }

  return bigArray.size();
  
}

//all check functions are called when the userlist and computerlist give no conclusive winning or blocking positions



bool checkRows(int val){
  //l,r fixed
  //computer checks for available spaces in the row where its current magic number under inspection is.
  //computer gives priority to rows where the entire row(the two remaining spaces) are empty
  // priority then shifts to any available space inside the row
  vector<int> arr = getCoordinate(val);
  if(isAvailable(arr[0],arr[1],(arr[2]+1)%3) && isAvailable(arr[0],arr[1],(arr[2]+2)%3)){
    go(2,arr[0],arr[1],(arr[2]+1)%3);
    computerList.push_back(cubePtr[arr[0]][arr[1]][(arr[2]+1)%3]);
    return true;
  }
  else if(isAvailable(arr[0],arr[1],(arr[2]+2)%3)){
    go(2,arr[0],arr[1],(arr[2]+2)%3);
    computerList.push_back(cubePtr[arr[0]][arr[1]][(arr[2]+2)%3]);
    return true;
  }
  else if(isAvailable(arr[0],arr[1],(arr[2]+1)%3)){
    go(2,arr[0],arr[1],(arr[2]+1)%3);
    computerList.push_back(cubePtr[arr[0]][arr[1]][(arr[2]+1)%3]);
    return true;
  }

  return false;

}

bool checkColumns(int val){
  //l,c fixed
  //computer checks for available spaces in the column where its current magic number under inspection is.
  //computer gives priority to columns where the entire column(the two remaining spaces) are empty
  // priority then shifts to any available space inside the column
  vector<int> arr = getCoordinate(val);

  if(isAvailable(arr[0],(arr[1]+1)%3,arr[2]) && isAvailable(arr[0],(arr[1]+2)%3,arr[2])){
    go(2,arr[0],(arr[1]+1)%3,arr[2]);
    computerList.push_back(cubePtr[arr[0]][(arr[1]+1)%3][arr[2]]);
    return true;
  }
  else if(isAvailable(arr[0],(arr[1]+2)%3,arr[2])){
    go(2,arr[0],(arr[1]+2)%3,arr[2]);
    computerList.push_back(cubePtr[arr[0]][(arr[1]+2)%3][arr[2]]);
    return true;
  }
  else if(isAvailable(arr[0],(arr[1]+1)%3,arr[2])){
    go(2,arr[0],(arr[1]+1)%3,arr[2]);
    computerList.push_back(cubePtr[arr[0]][(arr[1]+1)%3][arr[2]]);
    return true;
  }

  return false;

}

bool checkPillars(int val){
  //r.c fixed
  //computer checks for available spaces in the pillar where its current magic number under inspection is.
  //computer gives priority to pillars where the entire pillar(the two remaining spaces) are empty
  // priority then shifts to any available space inside the pillar
  vector<int> arr = getCoordinate(val);

  if(isAvailable((arr[0]+1)%3,arr[1],arr[2]) && isAvailable((arr[0]+2)%3,arr[1],arr[2])){
    go(2,(arr[0]+1)%3,arr[1],arr[2]);
    computerList.push_back(cubePtr[(arr[0]+1)%3][arr[1]][arr[2]]);
    return true;
  }
  else if(isAvailable((arr[0]+2)%3,arr[1],arr[2])){
    go(2,(arr[0]+1)%3,arr[1],arr[2]);
    computerList.push_back(cubePtr[(arr[0]+1)%3][arr[1]][arr[2]]);
    return true;
  }
  else if(isAvailable((arr[0]+1)%3,arr[1],arr[2])){
    go(2,(arr[0]+1)%3,arr[1],arr[2]);
    computerList.push_back(cubePtr[(arr[0]+1)%3][arr[1]][arr[2]]);
    return true;
  }

  return false;

}


bool checkMajorDiagonals(int val){
  //computer checks for available spaces in the major diagonals where its current magic number under inspection is.
  //computer gives priority to diagonals where the entire diagonal(the two remaining spaces) are empty
  // priority then shifts to any available space inside the diagonal

  int blip;
  int mid = cubePtr[1][1][1];
  vector<int> vect{20,18,4,26,12,23,7,15,6,17,11,22,16,5,21,13,8,10,24,2};
  vector<int> pos1,pos2;
  if(val == mid){
    for(int i =0;i<vect.size();i++){
      blip = 42 - (val + vect[i]);
      pos1 = getCoordinate(blip);
      pos2 = getCoordinate(vect[i]);
      if(isAvailable(pos1[0],pos1[1],pos1[2]) && isAvailable(pos2[0],pos2[1],pos2[2]) && collinear(val,blip,vect[i])){
        go(2,pos1[0],pos1[1],pos1[2]);
        computerList.push_back(cubePtr[pos1[0]][pos1[1]][pos1[2]]);
        return true;
      }
    }
    for(int i = 0;i<vect.size();i++){
      pos1 = getCoordinate(vect[i]);
      if(isAvailable(pos1[0],pos1[1],pos1[2])){
        go(2,pos1[0],pos1[1],pos1[2]);
        computerList.push_back(cubePtr[pos1[0]][pos1[1]][pos1[2]]);
        return true;
      }
    }
  }


  blip = 42 - (val + mid);
  pos1 = getCoordinate(val);
  pos2 = getCoordinate(blip);
  
  if(isAvailable(pos2[0],pos2[1],pos2[2]) && collinear(val,blip,mid)){
    go(2,pos2[0],pos2[1],pos2[2]);
    computerList.push_back(cubePtr[pos2[0]][pos2[1]][pos2[2]]);
    return true;
  }
    

  return false;

}




bool performChecks(int val){
  // module to call on different checks collectively
  //function will always returns true by nature of the game
  if(checkMajorDiagonals(val))
    return true;
  else if(checkRows(val))
    return true;
  else if(checkPillars(val))
    return true;
  else if(checkColumns(val))
    return true;

return false;
}


void makeTwo(){
  // function called by the computer when neither userlist or computerlist has any winning or blocking positions
  //takes the moves already played by the computer and scans entire rows,columns,pillars and majordiagonals associated with them to increase chances of victory

  int i;
  for(i=0;i<computerList.size();i++){
    if(performChecks(computerList[i]))
      return;
  }

}

void makeMove(vector<int> &computerList){
  //sends user and computer list to be checked for the computer to decide its move to win or block in ts current move.
  //if neither of the above conditions are not applicable, computer starts checking rows,cols,pillars and major diagonals 
  if(possWin(computerList)){
    
    goMagicNumber(2,possWin(computerList));
    computerList.push_back(possWin(computerList));
  }
  else if(possWin(userList)){
    
    goMagicNumber(2,possWin(userList));
    computerList.push_back(possWin(userList));
  }
  else{
    
    makeTwo();
  }
}

void startPlayingComputer(int*** boardPtr, int*** magicCube, vector<int> &userList, vector<int> &computerList){
  //function which decides when the game has to be stopped
  //if computer goes first, enter in 1,1,1
  //if human does not play in 1,1,1 enter there or else enter in 0,0,0
  if(boardPtr[1][1][1] == 0){
    go(2,1,1,1);
    computerList.push_back(cubePtr[1][1][1]);
  }
  else{
    go(2,0,0,0);
    computerList.push_back(cubePtr[0][0][0]);
  }
  
  int ***ticTacToe = boardPtr;

  display(boardPtr);

  while(counterHuman < 10 && counterComp < 10 && isFull()){

    // taking user input
    // check if we are winning (posswin) then make move, if not
    // check if opponent is winning
    // if he is, find winning position and block the move
    // if no one is winning, then make two
    // updating scores of the human and the computer

    user_input(userList);
    makeMove(computerList);

    if(userList.size() >= 3){
      counterHuman = winningCombination(userList);
    }
    
    if(computerList.size() >= 3){
      counterComp = winningCombination(computerList);
    }

    display(boardPtr);

      if(counterHuman >= 10 && counterComp < 10)
      {cout << "-----------------------------"<< endl;
      cout << "Game Over! Winner is Human!!" << endl;
      cout << "-----------------------------"<< endl;}
      else if(counterComp >= 10 && counterHuman < 10)
      {cout << "-------------------------------"<< endl;
      cout << "Game Over! Winner is Computer!!" << endl;
      cout << "-------------------------------"<< endl;}
      else if(!isFull())
      {cout << "----------------------------------------------------------"<< endl;
      cout << "Game Over! It's a Draw with scores Human: " << counterHuman << " Computer: " << counterComp << endl;
      cout << "----------------------------------------------------------"<< endl;}
      else if(counterComp >= 10 && counterHuman >= 10)
    {cout << "----------------------------------------------------------"<< endl;
      cout << "Game Over! It's a Draw with scores Human: " << counterHuman << " Computer: " << counterComp << endl;
      cout << "----------------------------------------------------------"<< endl;}
    

  }
}


int main(){
int in,l,r,c,s,i,j;

  cout << "Order of Cube: 3" << endl;

  //memory initialisation of magic cube elements
 
  int ***magicCube = (int***)calloc(N,sizeof(int**));

  for (i = 0; i < N; i++) {
    // Assign to magicCube[i], not *magicCube[i] (that would dereference an uninitialized pointer)
    magicCube[i] = (int**)calloc(N,sizeof(int*));
    for (j = 0; j < N; j++) {
      magicCube[i][j] = (int*)calloc(N,sizeof(int));
    }
  }

  if(N == 3){

    l=0;//layer
    r=N/2;//row
    c=N/2;//column

    for(int s=1;s <= pow(N,3) ;s++){
    if(magicCube[l][r][c]){
      if(magicCube[mod(l-1,N)][r][mod(c-1,N)]){
        if(magicCube[mod(l-1,N)][mod(r-1,N)][c]){
          l=mod(l+1,N);
          magicCube[l][r][c]=s;
        }
        else{
          l=mod(l-1,N);
          r=mod(r-1,N);
          magicCube[l][r][c]=s;
        }
      }
      else{
        l=mod(l-1,N);
        c=mod(c-1,N);
        magicCube[l][r][c]=s;
      }
    }
    else
      magicCube[l][r][c]=s;
  }
}
cubePtr = magicCube;

// ---------------------END OF MAGIC CUBE-------------------------------

//memory initialisation of board elements

int ***ticTacToe = (int***)calloc(N,sizeof(int**));

 // creating empty tic toc toe board

 for (i = 0; i < N; i++) {
    ticTacToe[i] = (int**)calloc(N,sizeof(int*));
    for (j = 0; j < N; j++) {
      ticTacToe[i][j] = (int*)calloc(N,sizeof(int));
    }
  }

  boardPtr = ticTacToe;

//----------------------END OF TIC-TAC-TOE BOARD------------------------



cout << "Choose X or O (X goes first): " << endl;
cin >> inp;
if (inp == 'X'){

  user_input(userList);
  startPlayingComputer(ticTacToe,  magicCube,userList,computerList );
}
else if (inp == 'O'){

startPlayingComputer(ticTacToe,  magicCube,userList,computerList );
}

else{
  cout<< "Program terminated due to incorrect input, please enter correct value (X or O) on the next run." << endl;
  exit(0);
}

}
