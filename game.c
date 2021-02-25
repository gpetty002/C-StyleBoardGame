#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX_LINE_SIZE 100

int checkInput(int yPos, int xPos, int endofRow, int endofCol, int board[endofRow + 1 ][endofCol + 1])
{
    if (yPos <= 0 || yPos >= endofRow)
    {
        return 1;
    } else if (xPos <= 0 || xPos >= endofCol)
    {
        return 1;
    } else if (board[yPos][xPos] == '-' || board[yPos][xPos] == '&')
    {
        return 1;
    } else if (board[yPos][xPos] == '*')
    {
        return 2;
    } else if (board[yPos][xPos] == 'X')
    {
        return 3;
    } else {
        return 0;
    }
    
}

void printBoard(int rows, int cols, int board[rows][cols])
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

int playGame(const char* filename)
{
    FILE *readF = fopen(filename, "r");

    char line[MAX_LINE_SIZE];

    int rows = 0;
    int cols = 0;

    char playerIcon = '\n';
    char poweredIcon = '\n';
    int playerYpos = 0;
    int playerXpos = 0;

    char left = '\n';
    char right = '\n';
    char up = '\n';
    char down = '\n';
    char quit = '\n';
    
    char iconHolder = '\n';
    int buildingYpos = 0;
    int buildingXpos = 0;

    int enemyYpos = 0;
    int enemyXpos = 0;

    int itemsYpos = 0;
    int itemsXpos = 0;

    int powerYpos = 0;
    int powerXpos = 0;

    int itemsRemain = 0;
    int numofEnemies = 0;
    
    int num = 1;
    fgets(line, MAX_LINE_SIZE, readF);
    sscanf(line, "%d %d", &rows, &cols);
    num++;
    
    rows = rows + 2;
    cols = cols + 2;
    int endofRow = rows - 1;
    int endofCols = cols - 1;

    int board[rows][cols];

    for (int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            board[i][j] = ' ';
        }
    }

    while(fgets(line, MAX_LINE_SIZE, readF))
    {
        if (num == 2)
        {
            if (*line == '\n')
            {
                left = 'a';
                right = 'd';
                up = 'w';
                down = 's';
                quit = 'q';
            } else {
                sscanf(line, "%c %c %c %c %c", &left, &right, &up, &down, &quit);
            }
        }
        if (num == 3)
        {
            sscanf(line, "%c %c %d %d", &playerIcon, &poweredIcon, &playerYpos, &playerXpos);
            playerYpos++;
            playerXpos++;
            board[playerYpos][playerXpos] = playerIcon;
        }

        if (line[0] == 'B')
        {   
            sscanf(line, "%c %d %d", &iconHolder, &buildingYpos, &buildingXpos);
            buildingYpos++;
            buildingXpos++;

            for (int r = 0; r < 4; r++)
            {
                for (int c = 0; c < 6; c++)
                {
                    if ((r == 2 || r == 3) && (c == 2 || c == 3))
                    {
                        board[buildingYpos + r][buildingXpos + c] = '&';
                    } else {
                        board[buildingYpos + r][buildingXpos + c] = '-';
                    }
                    
                }
            }
        }

        if (line[0] == 'E')
        {
            sscanf(line, "%c %d %d", &iconHolder, &enemyYpos, &enemyXpos);
            enemyYpos++;
            enemyXpos++;
            board[enemyYpos][enemyXpos] = 'X';
            numofEnemies++;
        }

        if (line[0] == 'I')
        {
            sscanf(line, "%c %d %d", &iconHolder, &itemsYpos, &itemsXpos);
            itemsRemain++;
            itemsYpos++;
            itemsXpos++;
            board[itemsYpos][itemsXpos] = '$';
        }
        
        if (line[0] == 'P')
        {
            sscanf(line, "%c %d %d", &iconHolder, &powerYpos, &powerXpos);
            powerYpos++;
            powerXpos++;
            board[powerYpos][powerXpos] = '*';
            
        }
        num++;
    }

    fclose(readF);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (i == 0 || j == 0 || i == endofRow || j == endofCols)
            {
                board[i][j] = '*';
            }
        }
    }

    char key = '\n';
    int validInput = 1;
    int score = 0;
    int sevenCounter = 0;
    bool takenPowerup = false;

    // GAME LOOP
    while(1 == 1)
    {
        printf("Score: %d\n", score);
        printf("Items remaining: %d\n", itemsRemain);

        printBoard(rows, cols, board);

        if (board[playerYpos][playerXpos] == '@' && sevenCounter <= 0)
        {
            printf("You have died.\nFinal score: %d\n", score);
            break;
        }

        while (validInput == 1)
        {
            printf("Enter input: ");
            scanf(" %c", &key);
            
            if (key == left || key == right || key == up || key == down || key == quit)
            {
                validInput = 0;
            } else {
                printf("Invalid input.\n");
            }
        }

        validInput = 1;

        if (key == left)
        {
            if (checkInput(playerYpos, playerXpos - 1, endofRow, endofCols, board) == 0)
            {
                if (board[playerYpos][playerXpos - 1] == '$')
                {
                    score++;
                    itemsRemain--;
                }
                sevenCounter--;
                if (board[playerYpos][playerXpos] == poweredIcon && sevenCounter > 0)
                {
                    board[playerYpos][playerXpos] = ' ';
                    playerXpos--;
                    board[playerYpos][playerXpos] = poweredIcon;
                } else {
                    board[playerYpos][playerXpos] = ' ';
                    playerXpos--;
                    board[playerYpos][playerXpos] = playerIcon;
                }
            } else if (checkInput(playerYpos, playerXpos - 1, endofRow, endofCols, board) == 2)
            {
                if (board[playerYpos][playerXpos - 1] == '$')
                {
                    score++;
                    itemsRemain--;
                }
                
                if (board[playerYpos][playerXpos - 1] == '*' && board[playerYpos][playerXpos] == playerIcon)
                {
                    takenPowerup = true;
                    board[playerYpos][playerXpos] = ' ';
                    playerXpos--;
                    board[playerYpos][playerXpos] = poweredIcon;
                    if (takenPowerup == true)
                    {
                        sevenCounter = 7;
                    }
                }
            } else if (board[playerYpos][playerXpos - 1] == 'X')
            {
                sevenCounter--;

                if (sevenCounter > 0)
                {
                    board[playerYpos][playerXpos] = ' ';
                    playerXpos--;
                    board[playerYpos][playerXpos] = poweredIcon;
                    score++;
                } else if (sevenCounter <= 0)
                {
                    board[playerYpos][playerXpos] = ' ';
                    playerXpos--;
                    board[playerYpos][playerXpos] = '@';
                    takenPowerup = false;
                }
                
            } else if ((checkInput(playerYpos, playerXpos - 1, endofRow, endofCols, board) == 1))
            {
                sevenCounter--;

                if (sevenCounter > 0)
                {
                    board[playerYpos][playerXpos] = poweredIcon;
                } else if (sevenCounter <= 0) {
                    board[playerYpos][playerXpos] = playerIcon;
                }
            }
        }

        if (key == right)
        {
            if (checkInput(playerYpos, playerXpos + 1, endofRow, endofCols, board) == 0)
            {
                if (board[playerYpos][playerXpos + 1] == '$')
                {
                    score++;
                    itemsRemain--;
                }
                
                sevenCounter--;

                if (board[playerYpos][playerXpos] == poweredIcon && sevenCounter > 0)
                {
                    board[playerYpos][playerXpos] = ' ';
                    playerXpos++;
                    board[playerYpos][playerXpos] = poweredIcon;
                    
                } else {
                    board[playerYpos][playerXpos] = ' ';
                    playerXpos++;
                    board[playerYpos][playerXpos] = playerIcon;
                }
            } else if (checkInput(playerYpos, playerXpos + 1, endofRow, endofCols, board) == 2)
            {
                if (board[playerYpos][playerXpos + 1] == '$')
                {
                    score++;
                    itemsRemain--;
                }
                if (board[playerYpos][playerXpos + 1] == '*' && board[playerYpos][playerXpos] == playerIcon)
                {
                    takenPowerup = true;
                    if (takenPowerup == true)
                    {
                        sevenCounter = 7;
                    }
                    board[playerYpos][playerXpos] = ' ';
                    playerXpos++;
                    board[playerYpos][playerXpos] = poweredIcon;
                }
                
            } else if (board[playerYpos][playerXpos + 1] == 'X')
            {
                sevenCounter--;

                if (sevenCounter > 0)
                {
                    board[playerYpos][playerXpos] = ' ';
                    playerXpos++;
                    board[playerYpos][playerXpos] = poweredIcon;
                    score++;
                } else if (sevenCounter <= 0){
                    board[playerYpos][playerXpos] = ' ';
                    playerXpos++;
                    board[playerYpos][playerXpos] = '@';
                    takenPowerup = false;
                }
               
            } else if ((checkInput(playerYpos, playerXpos + 1, endofRow, endofCols, board) == 1))
            {
                sevenCounter--;

                if (sevenCounter > 0)
                {
                    board[playerYpos][playerXpos] = poweredIcon;
                } else if (sevenCounter <= 0) {
                    board[playerYpos][playerXpos] = playerIcon;
                }
            }
        }


        if (key == up)
        {
            if (checkInput(playerYpos - 1, playerXpos, endofRow, endofCols, board) == 0)
            {
                if (board[playerYpos - 1][playerXpos] == '$')
                {
                    score++;
                    itemsRemain--;
                }
                sevenCounter--;

                if (board[playerYpos][playerXpos] == poweredIcon && sevenCounter > 0)
                {
                    board[playerYpos][playerXpos] = ' ';
                    playerYpos--;
                    board[playerYpos][playerXpos] = poweredIcon;
                } else {
                    board[playerYpos][playerXpos] = ' ';
                    playerYpos--;
                    board[playerYpos][playerXpos] = playerIcon;
                }
            } else if (checkInput(playerYpos - 1, playerXpos, endofRow, endofCols, board) == 2)
            {
                if (board[playerYpos - 1][playerXpos] == '$')
                {
                    score++;
                    itemsRemain--;
                }

                if (board[playerYpos - 1][playerXpos] == '*' && board[playerYpos][playerXpos] == playerIcon)
                {
                    takenPowerup = true;
                    if (takenPowerup == true)
                    {
                        sevenCounter = 7;
                    }
                    board[playerYpos][playerXpos] = ' ';
                    playerYpos--;
                    board[playerYpos][playerXpos] = poweredIcon;
                }
                
            } else if (board[playerYpos - 1][playerXpos] == 'X')
            {
                sevenCounter--;
                if (sevenCounter > 0)
                {
                    board[playerYpos][playerXpos] = ' ';
                    playerYpos--;
                    board[playerYpos][playerXpos] = poweredIcon;
                    score++;
                } else if (sevenCounter <= 0){
                    board[playerYpos][playerXpos] = ' ';
                    playerYpos--;
                    board[playerYpos][playerXpos] = '@';
                    takenPowerup = false;
                }
            } else if ((checkInput(playerYpos - 1, playerXpos, endofRow, endofCols, board) == 1))
            {
                sevenCounter--;

                if (sevenCounter > 0)
                {
                    board[playerYpos][playerXpos] = poweredIcon;
                } else if (sevenCounter <= 0) {
                    board[playerYpos][playerXpos] = playerIcon;
                }
            }
        }

        if (key == down)
        {
            if (checkInput(playerYpos + 1, playerXpos, endofRow, endofCols, board) == 0)
            {
                if (board[playerYpos + 1][playerXpos] == '$')
                {
                    score++;
                    itemsRemain--;
                }

                sevenCounter--;

                // MOVING AND DECREASING COUNTER WITHOUT ANY REPRECUSSIONS
                if (board[playerYpos][playerXpos] == poweredIcon && sevenCounter > 0)
                {
                    board[playerYpos][playerXpos] = ' ';
                    playerYpos++;
                    board[playerYpos][playerXpos] = poweredIcon;
                } else {
                    board[playerYpos][playerXpos] = ' ';
                    playerYpos++;
                    board[playerYpos][playerXpos] = playerIcon;
                }
            } else if (checkInput(playerYpos + 1, playerXpos, endofRow, endofCols, board) == 2)
            {
                if (board[playerYpos + 1][playerXpos] == '$')
                {
                    score++;
                    itemsRemain--;
                }
                if (board[playerYpos + 1][playerXpos] == '*' && board[playerYpos][playerXpos] == playerIcon)
                {
                    takenPowerup = true;
                    if (takenPowerup == true)
                    {
                        sevenCounter = 7;
                    }
                    board[playerYpos][playerXpos] = ' ';
                    playerYpos++;
                    board[playerYpos][playerXpos] = poweredIcon;
                }
                
            } else if (board[playerYpos + 1][playerXpos] == 'X')
            {
                sevenCounter--;

                if (sevenCounter > 0)
                {
                    board[playerYpos][playerXpos] = ' ';
                    playerYpos++;
                    board[playerYpos][playerXpos] = poweredIcon;
                    score++;
                } else if (sevenCounter <= 0){
                    board[playerYpos][playerXpos] = ' ';
                    playerYpos++;
                    board[playerYpos][playerXpos] = '@';
                    takenPowerup = false;
                }
            } else if ((checkInput(playerYpos + 1, playerXpos, endofRow, endofCols, board) == 1))
            {
                sevenCounter--;

                if (sevenCounter > 0)
                {
                    board[playerYpos][playerXpos] = poweredIcon;
                } else if (sevenCounter <= 0) {
                    board[playerYpos][playerXpos] = playerIcon;
                }
            }
        }

        if (key == quit)
        {
            printf("You have quit.\n");
            printf("Final score: %d\n", score);

            break;
        } else if (itemsRemain == 0)
        {
            printf("Congratulations! You have won.\n");
            printf("Final score: %d\n", score);
            break;
        }
    }
    
    return 0;
}
