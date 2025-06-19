#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define N 9

// Function to print the Sudoku grid with separators
void printSudoku(int grid[N][N]) {
    printf("+-------+-------+-------+\n");
    for (int i = 0; i < N; i++) {
        printf("| ");
        for (int j = 0; j < N; j++) {
            printf("%d ", grid[i][j]);
            if ((j + 1) % 3 == 0)
                printf("| ");
        }
        printf("\n");
        if ((i + 1) % 3 == 0)
            printf("+-------+-------+-------+\n");
    }
}

// Check if placing a number is safe
bool isSafe(int grid[N][N], int row, int col, int num) {
    for (int x = 0; x < N; x++) {
        if (grid[row][x] == num || grid[x][col] == num)
            return false;
    }

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num)
                return false;

    return true;
}

// Recursive Sudoku solver
bool solveSudoku(int grid[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (grid[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (isSafe(grid, row, col, num)) {
                        grid[row][col] = num;
                        if (solveSudoku(grid))
                            return true;
                        grid[row][col] = 0; // backtrack
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// Function to get user input as 81 characters and save to sudoku.txt
void getAutoInput(char filename[]) {
    char input[100];
    printf("Enter the 81-digit Sudoku (use 0 for blanks):\n");
    scanf("%s", input);

    if (strlen(input) != 81) {
        printf("❌ Invalid input! Must be exactly 81 digits.\n");
        return;
    }

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Error creating sudoku.txt");
        return;
    }

    for (int i = 0; i < 81; i++) {
        fprintf(fp, "%c", input[i]);
        if ((i + 1) % 9 == 0)
            fprintf(fp, "\n");
        else
            fprintf(fp, " ");
    }

    fclose(fp);
    printf("✅ sudoku.txt created successfully!\n");
}

int main() {
    int sudoku[N][N];
    char filename[] = "sudoku.txt";

    int choice;
    printf("Sudoku Solver\n");
    printf("==============\n");
    printf("1. Enter Sudoku manually (81 digits in one line)\n");
    printf("2. Use existing sudoku.txt file\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    if (choice == 1) {
        getAutoInput(filename);
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("❌ Error opening sudoku.txt");
        return 1;
    }

    // Load puzzle from file
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            fscanf(fp, "%d", &sudoku[i][j]);

    fclose(fp);

    printf("\n🧩 Input Puzzle:\n");
    printSudoku(sudoku);

    if (solveSudoku(sudoku)) {
        printf("\n✅ Solved Sudoku:\n");
        printSudoku(sudoku);
    } else {
        printf("\n❌ No solution exists for the given puzzle.\n");
    }

    return 0;
}