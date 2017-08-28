#include <stdio.h>
#include <stdlib.h>

int matrix[10][10]; /* the 9 x 9 matrix */
int fixed[10][10]; /* initial given numbers */

int row_adding(int n)
{
    return (n-1) / 3;
}

int col_adding(int n)
{
    return (n-1) % 3;
}

int collision_square(int i, int j)
{
    // (i,j) can only be:
    // (1,1), (1,4), (1,7)
    // (4,1), (4,4), (7,4)
    // (7,1), (7,4), (7,7)

    int row_adding_k, col_adding_k;
    int row_adding_m, col_adding_m;
    int k, m;

    for (k=1; k<=8; ++k) {
        row_adding_k = row_adding(k);
        col_adding_k = col_adding(k);
        for (m=k+1; m<=9; ++m) {
            row_adding_m = row_adding(m);
            col_adding_m = col_adding(m);
            if (matrix[i+row_adding_k][j+col_adding_k] == matrix[i+row_adding_m][j+col_adding_m]
                && matrix[i+row_adding_k][j+col_adding_k] != 0)
                return 1;
        }
    }

    return 0;
}

int collision(int i, int j)
{
    int k, m;
    int start_row, start_col;

    // check the row i
    for (k=1; k<=8; ++k)
        for (m=k+1; m<=9; ++m)
            if (matrix[i][k] == matrix[i][m] && matrix[i][k] != 0)
                return 1; // collision
    // check the column j
    for (k=1; k<=8; ++k)
        for (m=k+1; m<=9; ++m)
            if (matrix[k][j] == matrix[m][j] && matrix[k][j] != 0)
                return 1; // collision

    // check the square
    start_row = ((i-1) % 3)*3 +1;
    start_col = ((j-1) % 3)*3 +1;
    return collision_square(start_row, start_col);
}

void print_matrix()
{
    int i, j;

    for (i=1; i<=9; ++i) {
        for (j=1; j<=9; ++j)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

void try(int i, int j)
{
    int k;

    if (i > 9) {
        // solved
        print_matrix();
        exit(0);
    }

    if (fixed[i][j] == 0) {
        for (k=1; k<=9; ++k) {
            matrix[i][j] = k;
            if (! collision(i, j)) {
                if (j < 9)
                    try (i, j+1);
                else
                    try (i+1, 1);
            }
        }
        matrix[i][j] = 0;
    }
    else {
        if (j < 9)
            try (i, j+1);
        else
        try (i+1, 1);
    }
}

int main()
{
    int i, j;

    printf("Please enter the Sudoku matrix: (use 0 to indicate the blank):\n");
    for (i=1; i<=9; ++i)
        for (j=1; j<=9; ++j)
            scanf("%d", &matrix[i][j]);

    printf("Calculating...\n");

    for (i=1; i<=9; ++i)
        for (j=1; j<=9; ++j)
            if (matrix[i][j] == 0)
                fixed[i][j] = 0;
            else
                fixed[i][j] = 1;

    try (1, 1); // start from (1, 1)
    exit(1);
}
