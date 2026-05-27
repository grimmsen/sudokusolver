# sudokusolver

C program that solves sudoku riddles. Developed just for fun. 


You can use command line sudoku in Ubuntu to do some test solves


```
apt install sudoku
```


Otherwise put your riddle into solver.c file where the three sharps are

```
### -> { 0,0,0, 0,1,0 ... }
```


Take your sudoku and put into the array line by line. So


```
 . 5 . | . . . | . . 3
 . . . | 1 4 2 | . . 9
 9 . . | . . 8 | 7 6 .
-------+-------+-------
 . 1 . | . . 5 | . . .
 . . 3 | . . . | 5 . .
 . . . | 9 . . | . 2 .
-------+-------+-------
 . 6 1 | 3 . . | . . 5
 8 . . | 4 6 7 | . . .
 7 . . | . . . | . 8 .
```

becomes


```
{0,5,0,0,0,0,0,0,3,0,0,0,1,4,2....}
```

Type


``` 
make solver
```


and execute the solver.


If you have command line sudoku installed type


```
./test.sh
```


to make a test solve. You can put the class of the sudoku command at the end. For example:


```
./test.sh hard
```


to let sudoku create a hard puzzle and let the program solve it.



The algorithm is explained in the c file.


There is a sleep command in the loop, to make the solve more visible. It solves each puzzle actually in a blink of an eye. The sleep is just for show.
