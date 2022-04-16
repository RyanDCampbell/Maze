//-----------------------------------------
// NAME: Ryan Campbell
// STUDENT NUMBER: 7874398
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Dr. Mehdi Niknam
// ASSIGNMENT: assignment #2
// 
// REMARKS:Test using a backtracking algorithm
// to see if a mouse can escape from a
// rectangular maze.
//----------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

#define MAX_DIMENSION 20
#define MAX_INPUT 50      //This is used to read each line from stdin, because our input
                          //includes spaces, we need our input length to be greater than
                          //the maximum size of our 2D array.

// constant definitions for the different cell states
const char WALL    = '1';
const char SPACE   = '0';
const char VISITED = '.';
const char MOUSE   = 'm';
const char EXIT    = 'e';

typedef enum BOOL { false, true } Boolean;

struct CELL
{
  int row;
  int column;
};
typedef struct CELL Cell;

typedef struct CELL_NODE CellNode;
struct CELL_NODE
{
  Cell     cell;
  CellNode *next;
};

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

CellNode *top = NULL;

// a 2D array used to store the maze
char maze[MAX_DIMENSION][MAX_DIMENSION];
int mazeRows;
int mazeCols;

// holds the location of the mouse and escape hatch
Cell mouse;
Cell escape;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

// basic cell manipulation

// returns true if the cells are at the same position in our maze
Boolean equalCells(const Cell cell1, const Cell cell2);

// returns a new cell object
Cell makeCell(const int row, const int col);

// returns true if the cell is within our maze
Boolean validCell(const Cell theCell);



// routines for managing our backtracking

// returns true if there are no more cells to try
Boolean noMoreCells();

// returns the next cell to try for a path out of the maze
Cell nextCell();

// introduces a new cell to try
void addCell(const Cell cell);

// prints the maze to console
void printMaze();

// read the maze from stdin and populates the matrix
void loadMaze();

// returns true if there's a solution to the maze
Boolean solveMaze();

// our invariant checker
// Checks that we always have a vaild maze
void checkState();



//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
    loadMaze();

    printMaze();

    if ( solveMaze() )
      printf( "The mouse is free!!!!\n" );
    else
      printf( "The mouse is trapped!!!!\n" );
    
    printf( "\nEnd of processing\n" );
    
  return EXIT_SUCCESS;
}


//////////////////////////////////////////////
// Cell routines
//////////////////////////////////////////////


//------------------------------------------------------
// noMoreCells
//
// PURPOSE: 
// This function returns true if the stack is empty
//
// OUTPUT PARAMETERS:
// Returns true if the stack is empty, else false
//------------------------------------------------------
Boolean noMoreCells()
{
  return top == NULL;
}


//------------------------------------------------------
// nextCell
//
// PURPOSE: 
// This function returns the top cell from the stack
// and frees the memory of the unneeded cell
//
// OUTPUT PARAMETERS:
// Returns the next cell in the stack
//------------------------------------------------------
Cell nextCell()
{
    Cell temp = top -> cell;
    CellNode *oldTop = top; 

    top = top -> next;
   
    free(oldTop);

  return temp;
}


//------------------------------------------------------
// addCell
//
// PURPOSE: 
// This function adds a new cell to the top of the stack
//
// INPUT PARAMETERS:
// The method is passed the cell to be added to the top
// of the stack
//------------------------------------------------------
void addCell(const Cell cell)
{

  CellNode *newNode;
  newNode = ( CellNode *) malloc(sizeof(CellNode));

  assert(newNode != NULL);

  if(top == NULL)
  {
    top = ( CellNode *) malloc(sizeof(CellNode));

    assert(newNode != NULL);

    newNode -> cell = cell;
    newNode -> next = NULL;
  }
  else
  {
    newNode -> cell = cell;
    newNode -> next = top;
    top = newNode;
  }

}


//------------------------------------------------------
// equalCells
//
// PURPOSE: 
// This function compares the contents of the 2 passed
// cells, returning true if the contents are the same,
// else false
//
// INPUT PARAMETERS:
// This function is passed two Cells to compare
//
// OUTPUT PARAMETERS:
// Returns a Boolean value, true if the contents are
// the same, false otherwise
//------------------------------------------------------
Boolean equalCells(const Cell cell1, const Cell cell2)
{
  return ((cell1.row == cell2.row) && (cell1.column == cell2.column));
}


//------------------------------------------------------
// makeCell
//
// PURPOSE: 
// This function creates a new Cell with the desired
// row and column number
//
// INPUT PARAMETERS:
// The function is passed the row and column number to
// be assigned to the Cell
//
// OUTPUT PARAMETERS:
// Returns the newly created Cell
//------------------------------------------------------
Cell makeCell(const int row, const int col)
{

  assert(0 < row && row <= MAX_DIMENSION);
  assert(0 < col && col <= MAX_DIMENSION);

  Cell result;
  
  result.row = row;
  result.column = col;

  assert(validCell(result));

  return result;
}



//------------------------------------------------------
// validCell
//
// PURPOSE: 
// Verifies that the newly made cell is within the walls
// of the maze.
//
// INPUT PARAMETERS:
// This function is passed the Cell to be examined
//
// OUTPUT PARAMETERS:
// Returns true if the Cell is valid, else false
//------------------------------------------------------
Boolean validCell(const Cell theCell)
{

  return ((0 < theCell.row    && theCell.row    < mazeRows) &&
          (0 < theCell.column && theCell.column < mazeCols));

}



//////////////////////////////////////////////
// Maze routines
//////////////////////////////////////////////


//------------------------------------------------------
// loadMaze
//
// PURPOSE: 
// This function reads in the user input from stdin, 
// from this it determines the size of the maze (matrix)
// and populates the matrix with the values from stdin.
// It also determines the location of the mouse and the exit
//------------------------------------------------------
void loadMaze()
{

  int currRow = 0;
  int currCol = 0;
  char *nextChar;
  char line[MAX_INPUT];  

  fgets(line, MAX_INPUT, stdin);

  mazeRows = atoi(strtok(line, " "));

  mazeCols = atoi(strtok(NULL, " "));

  assert(mazeRows <= MAX_DIMENSION);
  assert(mazeCols <= MAX_DIMENSION);

  while(fgets(line, MAX_INPUT, stdin) != NULL){

    nextChar = strtok(line, " ");
    
    for(int i = 0; i < mazeCols; i++)
    {
       
      maze[currRow][currCol] = nextChar[0];

      if(maze[currRow][currCol] == MOUSE)
      {
        mouse.row = currRow;
        mouse.column = currCol;
      }
      else if(maze[currRow][currCol] == EXIT)
      {
        escape.row = currRow;
        escape.column = currCol;
      }

      nextChar = strtok(NULL, " ");
      currCol++;
    }
     currCol = 0;
     currRow++;
  }

  checkState();

}

//------------------------------------------------------
// printMaze
//
// PURPOSE: 
// This function in a simple nested loop that prints
// the contents of the maze (matrix) to stdout
//------------------------------------------------------
void printMaze()
{
  for(int i = 0; i < mazeRows; i++)
  {
    for(int j = 0; j < mazeCols; j++)
    {
      printf("%c", maze[i][j]);
    }
    printf("\n");
  }

  printf("\n");
}

//------------------------------------------------------
// solveMaze
//
// PURPOSE: 
// This function adds the open cells surrounding the
// mouse to the stack to be navigated. It is the main
// function in the program
//
// OUTPUT PARAMETERS:
// Returns true if the maze can be solved, else false
//------------------------------------------------------
Boolean solveMaze()
{

  Boolean mazeSolved = false;

  Cell currCell = makeCell(mouse.row, mouse.column);

  checkState();

    if(maze[currCell.row-1][currCell.column] == SPACE ||
       maze[currCell.row-1][currCell.column] == EXIT) // Checks the Cell directly above the current Cell
    {

      addCell(makeCell(currCell.row-1, currCell.column));

    }
    if(maze[currCell.row+1][currCell.column] == SPACE ||
       maze[currCell.row+1][currCell.column] == EXIT) //If the Cell below is open
    {

      addCell(makeCell(currCell.row+1, currCell.column));

    }

    if(maze[currCell.row][currCell.column-1] == SPACE ||
       maze[currCell.row][currCell.column-1] == EXIT)  //If the Cell to the left is open
    {
      addCell(makeCell(currCell.row, currCell.column-1));
    }

    if(maze[currCell.row][currCell.column+1] == SPACE ||
       maze[currCell.row][currCell.column+1] == EXIT) //If the Cell to the right is open
    {

      addCell(makeCell(currCell.row, currCell.column+1));
    }


  while(!equalCells(currCell, escape) && !noMoreCells())
  {

    maze[currCell.row][currCell.column] = VISITED;

    checkState();

    if(maze[currCell.row-1][currCell.column] == SPACE ||
       maze[currCell.row-1][currCell.column] == EXIT) // Checks the Cell directly above the current Cell
    {

      addCell(makeCell(currCell.row-1, currCell.column));

    }
    if(maze[currCell.row+1][currCell.column] == SPACE ||
       maze[currCell.row+1][currCell.column] == EXIT) //If the Cell below is open
    {

      addCell(makeCell(currCell.row+1, currCell.column));

    }

    if(maze[currCell.row][currCell.column-1] == SPACE ||
       maze[currCell.row][currCell.column-1] == EXIT)  //If the Cell to the left is open
    {
      addCell(makeCell(currCell.row, currCell.column-1));
    }

    if(maze[currCell.row][currCell.column+1] == SPACE ||
       maze[currCell.row][currCell.column+1] == EXIT) //If the Cell to the right is open
    {

      addCell(makeCell(currCell.row, currCell.column+1));
    }

    //If there are cells remaining, check them! (I would have named this cellsRemaining, but I am sticking to the provided naming conventions)
    if(!noMoreCells()){

       currCell = nextCell();
       mouse.column = currCell.column;
       mouse.row = currCell.column; 

    }
  
    printMaze();

  }


  if(equalCells(currCell, escape))
  {
    mazeSolved = true;
  }
  else
  {
    mazeSolved = false;
  }

  return mazeSolved;
}

//------------------------------------------------------
// checkState
//
// PURPOSE: 
// This function verifies that the contents of the matrix
// is valid
//------------------------------------------------------
void checkState()
{

  int numExits = 0;
  int wallErrors = 0;

  //Checks to ensure that matrix is filled with only valid chars
  for(int i = 0; i < mazeRows; i++)
  {
    for(int j = 0; j < mazeCols; j++)
    {
      assert((maze[i][j] == WALL)    ||
             (maze[i][j] == SPACE)   ||
             (maze[i][j] == VISITED) ||
             (maze[i][j] == MOUSE)   ||
             (maze[i][j] == EXIT));

      if(maze[i][j] == EXIT)
      {
        numExits++;
      }

    }
  }

  //Ensure the maze only has one exit
  assert(numExits == 1);

  //Ensure there are walls at the top and bottom of the maze
  for(int i = 0; i < mazeRows; i++) 
  {

    if(maze[i][0] != WALL)
    {
      wallErrors++;
    }

    if(maze[i][mazeCols-1] != WALL)
    {
      wallErrors++;
    }

  }

  //Ensure there are walls at the left and right side of the maze
  for(int i = 0; i < mazeCols; i++)
  {
    
    if(maze[0][i] != WALL)
    {
      wallErrors++;
    }

    if(maze[mazeRows-1][i] != WALL)
    {
      wallErrors++;
    }

  }

  //Ensure there are no errors in the surrounding walls
  assert(wallErrors == 0);

}
