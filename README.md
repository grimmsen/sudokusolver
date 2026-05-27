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
make
```


and execute the solver.


The algorithm is explained in the c file.


