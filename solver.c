#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* The algorithm
 *
 * is pretty straight forward. The algorithm maintains helper arrays for each number to block them for col, row or subsquare
 * so for example a number 1 placed at a specific field, creates a block array like this
 *
 *  . . 1 | . . . | . . .    1 1 1 | 1 1 1 | 1 1 1
 *  . . . | . . . | . . .    1 1 1 | 0 0 0 | 0 0 0
 *  . . . | . . . | . . .    1 1 1 | 0 0 0 | 0 0 0
 *  ---------------------    ---------------------
 *  . . . | . . . | . . .    0 0 1 | 0 0 0 | 0 0 0
 *  . . . | . . . | . . . -> 0 0 1 | 0 0 0 | 0 0 0
 *  . . . | . . . | . . .    0 0 1 | 0 0 0 | 0 0 0
 *  ---------------------    ---------------------
 *  . . . | . . . | . . .    0 0 1 | 0 0 0 | 0 0 0
 *  . . . | . . . | . . .    0 0 1 | 0 0 0 | 0 0 0
 *  . . . | . . . | . . .    0 0 1 | 0 0 0 | 0 0 0
 *
 *  in the code the right array would be H[0] for the 1
 *  the block is also applied to block arrays H[1] to H[8] for the location of the 1, of course
 *
 *  once there is a solution the block array, would look like
 *
 *                                             *
 *  . . 1 | . . 1 | . . .    1 1 1 | 1 1 1 | 1 1 1
 *  . . . | . . . | 1 . .    1 1 1 | 1 1 1 | 1 1 1
 *  . . . | . . . | . . .    1 1 1 | 1 1 1 | 1 1 1
 *  ---------------------    ---------------------
 *  . . . | 1 . . | . . .    1 1 1 | 1 1 1 | 1 1 1
 *  . 1 . | . . . | . . . -> 1 1 1 | 1 1 1 | 1 1 1
 *  . . . | . . . | . . 1    1 1 1 | 1 1 1 | 1 1 1
 *  ---------------------    ---------------------
 *  . . . | . . . | . . .    1 1 1 | 1 1 1 | 0 0 0
 *  . . . | . 1 . | . . .    1 1 1 | 1 1 1 | 0 1 0
 *  1 . . | . . . | . . .    1 1 1 | 1 1 1 | 0 1 0
 *
 * now the secret is, to add up the ones for each subsquare, row and col and look for a sum of 8
 *
 * add up the first column you get 9, add up the first subsquare you get 9
 * add up the first line you get 9, add up column 8 (marked with *) you get 8, so column 8
 * must have only one unblocked field for the 1 in it, which means, it's the only possible place
 * where we can put the 1 in - the solution. 
 * A sum of less than eight would mean there are more than one locations, where the number could be placed, because
 * the row, col or subsquare have more than one unblocked entries in the H array for that number
 * Place the solution in the F array, which blocks more fields now, and start over again.
 *
 * The algorithm fails sometimes, when presented very hard puzzles
 * Maybe you must do a deep trial and error search for that kind of puzzles
 *
 * If no more 8s are found, the puzzle is solved, or unsolvable by that algorithm
 *
 */

char* F;
char* H[9];

void printArray(char* A,char index) {
  int x,y,offset=0;
  static const char col[]="\033[93m";
  static const char coloff[]="\033[m";
  char* out=malloc(9*2+10+13);
  memset(out,0,9*2+3);
  for(y=0;y<9;y++) {
    for(x=0;x<9;x++) {
      if(x%3==0) {
        sprintf((out+x*2+offset), "| ");
        offset+=2;
      }
      /*if(index==(9*y+x)) {
        sprintf((out+x*2+offset),"%s%d%s ",col,A[9*y+x],coloff);
        offset+=11;
      } else {*/
      if(A[9*y+x]>0) {
        sprintf((out+x*2+offset),"%d ",A[9*y+x]);
      } else {
        sprintf((out+x*2+offset),"  ");
      }
    }
    if(y%3==0) {
      printf("-------------------------\n");
    }
    offset=0;
    printf("%s|\n",out);
  }
  printf("-------------------------\n");
}

char sumSub(char* A) {
  int i,sum=0;
  for(i=0;i<9;i++) {
    sum+=A[i];
  }
  return sum;
}

void printSub(char* A) {
  int i;
  for(i=0;i<9;i++) {
    printf("%d ",A[i]);
  }
}

char* getRow(char* A,char row) {
  char *R;
  int col,index=0;
  R=malloc(9);
  memset(R,0,9);
  for(col=0;col<9;col++) {
    if(A[9*row+col]>0) {
      R[index++]=A[9*row+col];
    }
  }
  return R;
}

void blockRow(char* A,char row) {
  int col;
  for(col=0;col<9;col++) {
    A[9*row+col]=1;
  }
}

char* getCol(char* A,char col) {
  char *R;
  int row,index=0;
  R=malloc(9);
  memset(R,0,9);
  for(row=0;row<9;row++) {
    if(A[9*row+col]>0) {
      R[index++]=A[9*row+col];
    }
  }
  return R;
}

void blockCol(char* A, char col) {
  int row;
  for(row=0;row<9;row++) {
    A[9*row+col]=1;
  }
}

char* getSubsquare(char* A, char row, char col) {
  int x=0,y=0,index=0;
  char* R;
  R=malloc(9);
  memset(R,0,9);
  for(y=0;y<3;y++) {
    for(x=0;x<3;x++) {
      if(A[27*col+3*row+9*y+x]>0) {
        R[index++]=A[27*col+3*row+9*y+x];
      }
    }
  }
  return R;
}

void blockSubsquare(char* A, char row, char col) {
  int x,y;
  for(y=0;y<3;y++) {
    for(x=0;x<3;x++) {
      A[27*col+3*row+9*y+x]=1;
    }
  }
}

void blockIndex(char* A,char index) {
  A[index]=1;
}

char getRowForIndex(char index) {
  return index/9;
}

char getColForIndex(char index) {
  return index%9;
}

char getSubsquareXForIndex(char index) {
  return (index%9)/3;
}

char getSubsquareYForIndex(char index) {
  return index/27;
}

int main() {
  int x,i,j,solutions,zeros;
  F=malloc(81);
  char* tmp=malloc(9);

  memcpy(F,(char [])
  //{0,0,4,7,0,6,1,0,0,0,3,6,5,0,0,0,9,0,2,0,0,0,1,0,0,3,6,9,0,0,0,5,0,0,6,1,0,0,3,1,0,4,9,0,0,7,6,0,0,9,0,0,0,8,3,7,0,0,4,0,0,0,5,0,1,0,0,0,5,6,7,0,0,0,5,3,0,1,8,0,0}
  //{6,4,0,0,0,0,0,2,8,0,0,5,4,0,0,3,0,0,3,0,0,8,0,9,0,0,0,7,0,0,0,8,0,2,4,0,0,0,0,0,5,0,0,0,0,0,1,3,0,7,0,0,0,5,0,0,0,7,0,5,0,0,6,0,0,1,0,0,8,4,0,0,8,6,0,0,0,0,0,3,9}
   /*******************************
    * YOUR RIDDLE GOES HERE
    * *****************************/
  {###}
  ,81);
  // helper array for each number  
  for(x=0;x<9;x++) {
    H[x]=malloc(81);
    memset(H[x],0,81);
  }
  printArray(F,-1);
  solutions=1;
  while(1) {
    solutions=0;
    zeros=0;
    //for(x=0;x<9;x++) {
    //  memset(H[x],0,81);
    //}
    // block the row, column or subsquare, for the number (81 fields converted to row and column and subsquare position)
    // if there is a solution, the number should have no block in exactly one location in the 81 fields in the helper array
    // empty fields contain a 0
    for(i=0;i<81;i++) {
      if(F[i]>0) {
        blockRow(H[F[i]-1],getRowForIndex(i));
        blockCol(H[F[i]-1],getColForIndex(i));
        for(j=0;j<9;j++) {
          blockIndex(H[j],i);
        }
        blockSubsquare(H[F[i]-1],getSubsquareXForIndex(i),getSubsquareYForIndex(i));
        //if(F[i]-1==0) {
        //  printf("Block %d %d row %d col %d for index %d number %d\n",getSubsquareXForIndex(i),getSubsquareYForIndex(i),getRowForIndex(i),getColForIndex(i),i-1,F[i]);
        //  printArray(H[0]);
        //}
      }
    }
    // now we search for numbers with just one non blocked entry in its helper array H[i] (sum of 8), thats the solution, then start over again
    // if no such solution is found, the sudoku is unsolvable for this algorithm, that's the case if we don't find any sum of 8 in our helper arrays (H) anymore
    for(i=0;i<9;i++) {
      for(j=0;j<81;j++) {
	// not blocked
        if(H[i][j]==0) {
          // add up the numbers block entries in cols, rows and subsquares for each possible solution
	  // if there are eight blocks for the number in rows, cols or subsquares the number in question is the solution for that field
	  // because there is no other location, the number could be placed, either in row, col or subsquare
          int sumRow,sumCol,sumSquare;
          tmp=getRow(H[i],getRowForIndex(j));
          sumRow=sumSub(tmp);
          tmp=getCol(H[i],getColForIndex(j));
          sumCol=sumSub(tmp);
          tmp=getSubsquare(H[i],getSubsquareXForIndex(j),getSubsquareYForIndex(j));
          sumSquare=sumSub(tmp);
          if(sumRow==8 || sumCol==8 || sumSquare==8) {
            printf("\033cSolution found: index: %d sumR %d sumC %d sumS %d number %d\n",j,sumRow,sumCol,sumSquare,i+1);
            solutions++;
            F[j]=i+1;
            printArray(F,j);
            usleep(1000*200);
          }
        }
        if(solutions>0)
          break;
      }
      if(solutions>0)
        break;
    }
    if(solutions==0) {
      printf("no more solutions\n");
      break;
    }
  }
  printf("\n");
  //printArray(F,-1);
  return 0;
}
