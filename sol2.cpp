// === Region: Headers ===
// iostream and cstring are included.
// Do not include extra header files
// =======================
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
using namespace std;

// === Region: Constants ===
// Useful constants are defined for this project assignment
// =======================
const int N = 9;

// === Region: Helper Functions ===
// Do not change these given functions
// =======================
/* -------------------------------------------------------------- */
/* This part serves as a pseudo random number generator for auto grade purpose only */
/* You are suggested not to refer to these codes in this assignment. */
/* You are not allowed to use global variables in this course. */
unsigned int next_num = 1; // Here we initiate an unsigned integer to be used in the following functions.

unsigned int pa2_rand() // This function is used to return a pseudo random number from 0 to 32767.
{
    next_num = next_num * 1103515245 + 2011;
    return static_cast<unsigned int>(next_num / 65536) % 32768;
}
void pa2_srand(unsigned int seed) // This function is used to set a seed of the pseudo random number generator.
{
    next_num = seed;
}
/* Every time you call pa1_rand(), you will get a new pseudo random number. For the same seed, the sequence of pseudo
   random number is fixed. For example, when seed = 3, the sequence of pseudo random number is fixed to be [17746,
   30897, 9622, 18921, 4034, 17510, 24152, 14388, 23665, 31532, ...]. When seed = 5, the sequence of pseudo random
   number is fixed to be [18655, 32247, 9873, 9718, 26373, 27678, 5314, 22512, 31845, 22885, ...] */
/* -------------------------------------------------------------- */

/**
 * Copys the grid given by the @param grid to the memory location at @param grid_copy
 */
void copyGrid(const int grid[N][N], int grid_copy[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            grid_copy[i][j] = grid[i][j];
        }
    }
}

/**
 * Prints the sudoku grid by the @param grid
 */
void printSudokuGrid(const int grid[N][N])
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            if (col == 3 || col == 6)
                cout << "| ";
            if (grid[row][col] == 0)
                cout << '_';
            else
                cout << grid[row][col];
            cout << " ";
        }
        if (row == 2 || row == 5)
        {
            cout << endl;
            for (int i = 0; i < N * 2 + 4; i++)
                if (i == 6 || i == 14)
                    cout << "+";
                else
                    cout << "-";
        }
        cout << endl;
    }
}

/**
 * This is a helper function used to randomly sort the N * N cells in the grid. After function call, @param reference will be an 2D array for your reference when
 * you are making a grid with only one solution.
 * The first dimension stand for the order, and the second dimension stands for the location of cells we want to go through.
 * To get the correct order, you should go through this reference array one by one.
 * For example, reference[24][0]=8, reference[24][1]=7 means the (24)+1=25th position to go through is grid[8][7], which should be in the last row and the second
 * last column.
 */
void createReferenceList(int reference[N * N][2])
{
    for (int i = 0; i < N * N; i++)
        reference[i][0] = -1;
    int counter = 0;
    while (counter < N * N)
    {
        int order = pa2_rand() % (N * N);
        if (reference[order][0] == -1)
        {
            reference[order][0] = counter / N; // Represents the x in the grid
            reference[order][1] = counter % N; // Represents the y in the grid
            counter++;
        }
    }
}

/** This is a helper function ask about the random seed.
 * When the random seed is 0, pa2_rand() will randomly given a sequence of number when we continuously call it.
 * When the random seed is not 0, pa2_rand() will always give the same sequence across devices
 */
void getRandomSeed()
{
    cout << "Please give a random seed:" << endl;
    int seed;
    cin >> seed;
    if (seed == 0)
    {
        pa2_srand(time(NULL));
    }
    else
    {
        pa2_srand(seed);
    }
}

/** This is a helper function to load the grid from a txt file */
bool getGridFromFile(int grid[N][N])
{
    char input_case_path[100];
    cout << "The path for grid (e.g. ./grids/grid1.txt ) :" << endl;
    cin >> input_case_path;
    ifstream fin(input_case_path);
    if (!fin)
    {
        cout << "Error: Can not open " << input_case_path << endl;
        return false;
    }
    else
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                int get = -1;
                while ((get > 9 || get < 0) && !fin.eof())
                    get = fin.get() - 48;
                if (get <= 9 && get >= 0)
                    grid[i][j] = get;
                else
                {
                    cout << "Not enough digits in the file" << input_case_path << endl;
                    return false;
                }
            }
        }
        fin.close();
        cout << "Get the following grid:" << endl;
        printSudokuGrid(grid);
    }
    return true;
}

/** This is a helper function to save your current grid into a txt file */
bool saveGrid(const int grid[N][N])
{
    char save;
    while (save != 'Y' && save != 'y' && save != 'N' && save != 'n')
    {
        cout << "Do you want to save current grid? (Y/N)" << endl;
        cin >> save;
    }
    if (save == 'N' || save == 'n')
        return false;

    char output_path[100];
    cout << "Please give the path to save current grid (e.g.   ./grids/grid9.txt ):" << endl;
    cin >> output_path;
    if (strcmp(output_path, "./grids/grid1.txt") == 0)
    {
        cout << "Cannot overwrite " << output_path << endl;
        return false;
    }
    ofstream fout(output_path);
    if (!fout)
    {
        cout << "Error: Can not create " << output_path << endl;
        return false;
    }
    else
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                fout << grid[i][j];
            }
            fout << endl;
        }
        fout.close();
    }
    return true;
}

/*
 * You are given the @param grid and @param row, you need to return true if the @param num already exists in the given row ( @param row )
 * Return false otherwise
 */
bool isPresentInRow(const int grid[N][N], const int row, const int num)
{
    for (int col = 0; col < N; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}

/*
 * You are given the @param grid and @param col, you need to return true if the @param num already exists in the given column ( @param col )
 * Return false otherwise
 */
bool isPresentInCol(const int grid[N][N], const int col, const int num)
{
    for (int row = 0; row < N; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}

/*
 * You are given the @param grid, @param row, and @param col, you need to return true if the @param num already exists in the 3x3 box of the given position
 * Return false otherwise
 */
bool isPresentInBox(const int grid[N][N], const int row, const int col, const int num)
{
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int checkedRow = 0; checkedRow < 3; checkedRow++)
        for (int checkedCol = 0; checkedCol < 3; checkedCol++)
            if (grid[startRow + checkedRow][startCol + checkedCol] == num)
                return true;
    return false;
}

/*
 * You are given the @param grid, @param row, and @param col you need to return true if the @param num is valid to be filled in the given column ( @param col )
 * Return false otherwise
 * Please be reminded that you also need to consider the out-of-boundary case for @param row, @param col, and @param num
 */
bool isValidPlace(const int grid[N][N], const int row, const int col, const int num)
{
    if (num > 9 || num <= 0 || row >= 9 || row < 0 || col >= 9 || col < 0)
        return false;
    return !isPresentInRow(grid, row, num) && !isPresentInCol(grid, col, num) && !isPresentInBox(grid, row, col, num);
}

/*
 * You are given the @param grid, and you need to return true if this given grid is filled
 * Return false otherwise
 */
bool isFilled(const int grid[N][N])
{
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            if (grid[row][col] == 0)
                return false;
    return true;
}

/*
 * This will be used in the following recursive functions.
 * You could uncomment one of these two to continue.
 */
// int findNextEmpty(const int grid[N][N] )
// void findNextEmpty(const int grid[N][N], ...)
int findNextEmpty(const int grid[N][N])
{
    int emptySpaces[N * N] = {};
    int size = 0;
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            if (grid[row][col] == 0)
            {
                emptySpaces[size] = row * 10 + col;
                size++;
            }
    return emptySpaces[0];
}

/** Here is a given helper function to help you pick a random number from 1 to 9. It will be used to check
 * if the number can be filled into the cell.
 * You will be given a fixed random number @param rand_num. It is generated by pa2_rand(), so it should be a pseudo random number
 * from 0 to 32767. You need to convert this rand_num to be a random number from 1 to 9.
 * @param addition will be used in a for loop. It will go through 0 to 8. Together with @param rand_num and @param addition, you
 * need to generate a sequence starting from a random number from 1 to 9 and go through all the number from 1 to 9.
 * For example, when the @param rand_num is 18655 (seed=5), you can generate the first random number to be 8, and the sequence will
 * be  8 9 1 2 3 4 5 6 7
 * When the @param rand_num is 24107 (seed=17), you can generate the first random number to be 6, and the sequence will be
 * 6 7 8 9 1 2 3 4 5
 */
int picker(int rand_num, int addition)
{
    return (addition + rand_num) % 9 + 1;
}

/*
 * Note: You need to write a recursive function, or the grade will be deducted.
 * You are given the @param grid. You need to recursively find the empty cells in the row-major order,
 * and then randomly fill the empty cells with valid digits.
 * To do this random filling, you need to use the given @param rand_num (don't use pa2_rand() in this function).
 * You can use the helper function picker to check all the 9 numbers starting from a random number from 1 to 9.
 * And it must have at least one valid number. You should pick the first valid number you find in this way.
 * For example, when your @param rand_num is 18655, you can generate the first random number to be 8. You need to check
 * whether the number is a valid digit in that cell by this order: 8->9->1->2->3->4->5->6->7 until you find a valid digit.
 * If the valid digit of this cell is {2,5,7,9}, you should pick 9 to fill in the cell
 * This recursive function will return true if you find an answer
 */
bool fillSudoku(int grid[N][N])
{
    int rand_num = pa2_rand();
    /* Please replace the code below to your own work */
    if (isFilled(grid))
        return true;
    int location = findNextEmpty(grid);
    int row = location / 10;
    int col = location % 10;
    for (int addition = 0; addition < N; addition++)
    {
        if (isValidPlace(grid, row, col, (addition + rand_num) % 9 + 1))
        {
            grid[row][col] = (addition + rand_num) % 9 + 1;
            if (fillSudoku(grid))
                return true;
            grid[row][col] = 0;
        }
    }
    return false;
    /* Please replace the code above to your own work */
}

/*
 * Note: You need to write a recursive function, or the grade will be deducted.
 * Similar to fillSudoku(), you are given an @param grid. You need to recursively find out the number of solutions of this grid.
 * But unlike the fillSudoku(), after the implement of solveSudoku(), the @param grid should be remain unchanged.
 * This recursive function will return the number of solution for the given Sudoku grid.
 */
int solveSudoku(int grid[N][N])
{
    /* Please replace the code below to your own work */
    if (isFilled(grid))
        return 1;
    else
    {
        int location = findNextEmpty(grid);
        int row = location / 10;
        int col = location % 10;
        int solution = 0;
        for (int num = 1; num <= N; num++)
        {
            if (isValidPlace(grid, row, col, num))
            {
                grid[row][col] = num;
                solution += solveSudoku(grid);
                grid[row][col] = 0;
            }
        }
        return solution;
    }
    /* Please replace the code above to your own work */
}

/*
 * Given a filled @param grid and a reference reference array @param reference, you can generate a valid sudoku game with only one solution.
 * You should do the following:
 * - Go through all the @param grid in the order given by @param reference (Please check the createReferenceList() function instruction
 *   to know how to use this reference reference array)
 * - For each position, remove the digit and check if the solution of the grid is 1. If it is 1, go to the next position.
 *   Otherwise, put the digit back and go to the next position
 * Once you go through all the positions by this trick, you should be able to generate a valid sudoku grid with only one solution.
 */
void generateValidSudokuGame(int grid[N][N], const int reference[N * N][2])
{
    for (int i = 0; i < N * N; i++)
    {
        const int row = reference[i][0];
        const int col = reference[i][1];
        int num = grid[row][col];
        grid[row][col] = 0;
        if (solveSudoku(grid) != 1)
            grid[row][col] = num;
    }
}

// === Region: Main Function ===
// Do not change this. You need to do your own code above.
int main()
{
    cout << "Welcome to Sudoku! Please select an option to play! (5 for developer mode)" << endl;
    cout << "1.Load a grid and play" << endl;
    cout << "2.Randomly fill a loaded grid" << endl;
    cout << "3.Compute number of solutions of a loaded grid" << endl;
    cout << "4.Generate a Sudoku grid with only one solution" << endl;
    int option;
    cin >> option;
    int grid[N][N] = {};

    switch (option)
    {
    case 1:
        if (getGridFromFile(grid))
        {
            int grid_backup[N][N];
            copyGrid(grid, grid_backup);
            char input[4] = {};
            bool quit = false;
            while (!isFilled(grid))
            {
                cout << "Please input three numbers to represent the row, column, and number to be fill in (e.g. 074). Press Q to quit." << endl;
                cout << "You can erase the digit in the grid by giving 0 in number (last digit) you input." << endl;
                cin >> input;
                if (input[0] == 'Q' || input[0] == 'q')
                {
                    quit = true;
                    break;
                }
                const int row = input[0] - 48;
                const int col = input[1] - 48;
                const int num = input[2] - 48;
                if (num == 0)
                {
                    if (grid_backup[row][col] != 0)
                        cout << "Can't erase this digit" << endl;
                    else
                    {
                        grid[row][col] = num;
                        printSudokuGrid(grid);
                    }
                }
                else if (isValidPlace(grid, row, col, num) && grid_backup[row][col] == 0)
                {
                    grid[row][col] = num;
                    printSudokuGrid(grid);
                }
                else
                {
                    cout << "Input not valid" << endl;
                }
            }
            if (!quit)
                cout << "Congratulations! " << endl;
        }
        break;
    case 2:
        if (getGridFromFile(grid))
        {
            getRandomSeed();
            fillSudoku(grid);
            cout << "Solved grid:" << endl;
            printSudokuGrid(grid);
        }
        else
            return -1;
        break;
    case 3:
        if (getGridFromFile(grid))
        {
            cout << "The number of solution in this Sudoku grid is " << solveSudoku(grid) << endl;
        }
        else
            return -1;
        break;
    case 4:
        getRandomSeed();
        fillSudoku(grid);
        cout << "Generated filled grid:" << endl;
        printSudokuGrid(grid);
        int reference[N * N][2];
        createReferenceList(reference);
        generateValidSudokuGame(grid, reference);
        cout << "Sudoku grid with only one solution:" << endl;
        printSudokuGrid(grid);
        break;
    case 5:
        cout << "Developer mode, select an option to continue: " << endl;
        // cout << "1.[No Grade] Test the output of your picker function" << endl;
        cout << "1.[No Grade] Generate a filled grid from an empty grid" << endl;
        cout << "2.[No Grade] Generate a Sudoku grid with only one solution from loaded filled grid" << endl;
        int option;
        cin >> option;
        switch (option)
        {
        // case 1:
        // {
        //     getRandomSeed();
        //     int rand_num = pa2_rand();
        //     cout << "The given random number is: " << rand_num << endl;
        //     cout << "Set the addition to be from 0 to 8, the picker will return: " << endl;
        //     for (int addition = 0; addition < 9; addition++)
        //         cout << picker(rand_num, addition) << " ";
        //     cout << endl;
        //     return -1;
        // }
        // break;
        case 1:
            getRandomSeed();
            fillSudoku(grid);
            cout << "Generated filled grid:" << endl;
            printSudokuGrid(grid);
            break;
        case 2:
            if (getGridFromFile(grid))
            {
                getRandomSeed();
                fillSudoku(grid);
                cout << "Generated filled grid:" << endl;
                printSudokuGrid(grid);
                int reference[N * N][2];
                createReferenceList(reference);
                generateValidSudokuGame(grid, reference);
                cout << "Sudoku grid with only one solution:" << endl;
                printSudokuGrid(grid);
            }
            else
                return -1;
            break;
        default:
            cout << "Invalid option" << endl;
            return -1;
            break;
        }
        break;

    default:
        cout << "Invalid option" << endl;
        return -1;
    }

    saveGrid(grid);
}
