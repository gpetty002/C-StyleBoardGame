#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_SIZE 100



char *loadKeys(const char* filename, char* left, char* right, char* up, char* down, char* quit)
{
    FILE *readF = fopen(filename, "r");

    int numLine = 1;
    char placeholder[MAX_LINE_SIZE];
    
    while (fgets(placeholder, MAX_LINE_SIZE, readF))
    {
        if (numLine == 2)
        {            
            if (*placeholder == '\n')
            {
                *left = 'a';
                *right = 'd';
                *up = 'w';
                *down = 's';
                *quit = 'q';

            } else {
                sscanf(placeholder, "%c %c %c %c %c", left, right, up, down, quit);
            }
        }
        numLine++;
    }

    fclose(readF);

    return 0;
}

int getInput(const char* filename, int *moveLeft, int *moveRight, int *moveUp, int *moveDown, int *quitGame, char *movement)
{
    char key = '\n';
    char left = '\n';
    char right = '\n';
    char up = '\n';
    char down = '\n';
    char quit = '\n';

    printf("Enter input: ");

    scanf(" %c", &key);

    char *checkKeys = loadKeys(filename, &left, &right, &up, &down, &quit);

    if (key == left)
    {
        *moveLeft = *moveLeft + 1;
        *movement = 'l';
    } else if (key == right)
    {
        *moveRight = *moveRight + 1;
        *movement = 'r';
    } else if (key == up)
    {
        *moveUp = *moveUp + 1;
        *movement = 'u';
    } else if (key == down)
    {
        *moveDown = *moveDown + 1;
        *movement = 'd';
    } else if (key == quit)
    {
        *quitGame = 1;
        printf("You have quit.\n");
    } else {
        free(checkKeys);
        return 1;
    }

    free(checkKeys);
    return 0;

}


char *playerStuff(const char* filename, char *playerIcon, char *poweredIcon, int* yPos, int *xPos)
{
    FILE *readF = fopen(filename, "r");

    int numLine = 1;
    char value[MAX_LINE_SIZE];
    
    while (fgets(value, MAX_LINE_SIZE, readF))
    {
        if (numLine == 3)
        {
            sscanf(value, "%c %c %d %d", playerIcon, poweredIcon, yPos, xPos);
        }
        
        numLine++;
    }

    fclose(readF);

    return 0;
}

int *boardDimensions(const char* filename, int *cols, int *rows)
{
    FILE *readF = fopen(filename, "r");

    char line[MAX_LINE_SIZE];
    int num = 0;

    while (fgets(line, MAX_LINE_SIZE, readF))
    {
        if (num == 0)
        {
            sscanf(line, "%d %d", rows, cols);
        }

        num++;
    }
    
    fclose(readF);

    return 0;
}


int parseForItems(const char* filename, char symbol, int arr[][2])
{
    FILE *readF = fopen(filename, "r");

    char line[MAX_LINE_SIZE];
    char placeholder;
    int i = 0;
    int j = 0;

    while (fgets(line, MAX_LINE_SIZE, readF))
    {
        if (line[0] == symbol)
        {
            sscanf(line, "%c %d %d", &placeholder, &arr[i][j], &arr[i][j+1]);
            i++;
            j = 0;
        }
    }
    fclose(readF);
    return 0;
}

int *occurences(const char* filename, int *b, int *e, int* iChar, int* p)
{
    FILE *readF = fopen(filename, "r");
    char line[MAX_LINE_SIZE];

    while (fgets(line, MAX_LINE_SIZE, readF))
    {
        if (line[0] == 'B')
        {
            *b = *b + 1;
            
        } else if (line[0] == 'E')
        {
           *e = *e + 1;
        } else if (line[0] == 'I')
        {
            *iChar = *iChar + 1;
        } else if (line[0] == 'P')
        {
            *p = *p + 1;
        }    
    }
    fclose(readF);
    return 0;
}


int *coordinates(
    const char* filename, int buildings[][2], int enemies[][2], 
    int item[][2], int powerOrb[][2])
{
    int b = 0;
    int e = 0;
    int iChar = 0; 
    int p = 0;

    int *checkOccurence = occurences(filename, &b, &e, &iChar, &p);
    //printf("b=%d e=%d i=%d p=%d\n", b, e, iChar, p);

    if (b > 0)
    {
        parseForItems(filename, 'B', buildings);
    } else if (b == 0)
    {
        buildings = NULL;
    }

    if (e > 0)
    {
        parseForItems(filename, 'E', enemies);
    } else if (e == 0)
    {
        enemies = NULL;
    }

    if (iChar > 0)
    {
        parseForItems(filename, 'I', item);
    } else if (iChar == 0)
    {
        item = NULL;
    }

    if (p > 0)
    {
        parseForItems(filename, 'P', powerOrb);
    } else if (p == 0)
    {
        powerOrb = NULL;
    }

    free(checkOccurence);
    return 0;
}

void setupBoard(const char* filename, int *left, int *right, int *up, int *down, int *score, char movement)
{
    // ROWS AND COLUMNS DECLARATION

    int cols = 0;
    int rows = 0;
    
    int *dimensions = boardDimensions(filename, &cols, &rows);
   
    // PLAYER POSITION SET UP

    char playerIcon, poweredIcon;
    int playerYpos, playerXpos;

    char *playerInfo = playerStuff(filename, &playerIcon, &poweredIcon, &playerYpos, &playerXpos);    
    
    // ADD PLAYER MOVEMENT 
    if (*left > 0)
    {
        playerXpos = playerXpos - *left;
    }
    if (*right > 0)
    {
        playerXpos = playerXpos + *right;
    }
    if (*up > 0)
    {
        playerYpos = playerYpos - *up;
    }
    if (*down > 0)
    {
        playerYpos = playerYpos + *down;
    }

    // BUILDING, ENEMY, ITEM, POWER ORB COORDINATES

    int b = 0;
    int e = 0;
    int iChar = 0;
    int p = 0;

    int *getOccurences = occurences(filename, &b, &e, &iChar, &p);

    int buildings[b][2];
    int enemies[e][2];
    int items[iChar][2];
    int powerOrbs[p][2];

    int *itemCoordinates = coordinates(filename, buildings, enemies, items, powerOrbs);

    // BUILDING SET UP

    int buildingYpos = 0;
    int buildingXpos = 0;
    int countBuilding = 0;
    int rb = 0;

    // ENEMY SET UP
    int enemyYpos = 0;
    int enemyXpos = 0;
    int countEnemy = 0;
    int rE = 0;

    // ITEM SET UP
    int itemYpos = 0;
    int itemXpos = 0;
    int countItem = 0;
    int rI = 0;

    // POWER ORB SET UP
    int powerYpos = 0;
    int powerXpos = 0;
    int countPower = 0;
    int rP = 0;

    // SCORE SET UP
    printf("Score: %d\n", *score);
    printf("Items remaining: %d\n", iChar);

    //MAKE BOARD
    for (int i = 1; i <= rows + 2; i++)
    {
        if (i == 1 || i == rows + 2)
        {
            for (int j = 0; j < cols + 2; j++)
            {
                printf("*");
            }
            printf("\n");
        } else {
            for (int j = 1; j <= cols + 2; j++)
            {
                if (j == 1)
                {
                    printf("*");
                } else if (j == cols + 2)
                {
                    printf("*\n");
                } else {
                    if (!(b == countBuilding))
                    {
                        buildingYpos = buildings[rb][0];
                        buildingXpos = buildings[rb][1];
                    }

                    if (!(e == countEnemy))
                    {
                        enemyYpos = enemies[rE][0];
                        enemyXpos = enemies[rE][1];
                    }

                    if (!(iChar == countItem))
                    {
                        itemYpos = items[rI][0];
                        itemXpos = items[rI][1];
                    }

                    if (!(p == countPower))
                    {
                        powerYpos = powerOrbs[rP][0];
                        powerXpos = powerOrbs[rP][1];
                    }

                    if (1 == playerYpos + 2)
                    {
                        playerYpos = 0;
                    } else if (rows + 2 == playerYpos + 2)
                    {
                        playerYpos = rows - 1;
                    }

                    if (1 == playerXpos + 2)
                    {
                        playerXpos = 0;
                    } else if (cols + 2 == playerXpos + 2)
                    {
                        playerXpos = cols - 1;
                    }

                    

                    if ((playerYpos + 2 >= buildingYpos + 2 && playerYpos + 2 <= buildingYpos + 5) && (playerXpos + 2 >= buildingXpos + 2 && playerXpos + 2 <= buildingXpos + 7))
                    {
                        if (playerYpos + 1 == buildingYpos + 1)
                        {
                            playerYpos = buildingYpos + 1;
                        } else if (playerYpos + 1 == buildingYpos + 6)
                        {
                            playerYpos = buildingYpos + 6;
                        }

                        if (playerXpos + 1 == buildingXpos + 1)
                        {
                            playerXpos = buildingXpos + 1;
                        } else if (playerXpos + 1 == buildingXpos + 8)
                        {
                            playerXpos = buildingXpos + 8;
                        }
                        
                        
                    }


                    // IF player's coordinates MATCH current row and col then PRINT icon
                    if (i == playerYpos + 2 && j == playerXpos + 2)
                    {
                        printf("%c", playerIcon);
                    } else if (i == enemyYpos + 2 && j == enemyXpos + 2)
                    {
                        printf("X");
                            
                        if (rE < e)
                        {
                            rE++;
                            countEnemy++;
                        }
                    } else if (i == itemYpos + 2 && j == itemXpos + 2)
                    {
                        printf("$");

                        if (rI < iChar)
                        {
                            rI++;
                            countItem++;
                        }
                        // THIS CAUSES A MEM LEAK IN CODE
                    } else if (i == powerOrbs[rP + 1][0] + 2 && j == powerOrbs[rP + 1][1] + 2) 
                    {
                        printf("*");
                    } else if (i == powerYpos + 2 && j == powerXpos + 2)
                    {
                        printf("*");

                        if (rP < p)
                        {
                            rP++;
                            countPower++;
                        }
                        // THIS ALSO CAUSES A MEM LEAK IN CODE
                    } else if ((i >= buildings[rb + 1][0] + 2 && i <= buildings[rb + 1][0] + 5) && (j >= buildings[rb + 1][1] + 2 && j <= buildings[rb + 1][1] + 7))
                    {
                        printf("-");
                    } else if ((i >= buildingYpos + 2 && i <= buildingYpos + 5) && (j >= buildingXpos + 2 && j <= buildingXpos + 7))
                    {
                        if ((i == buildingYpos + 4 || i == buildingYpos + 5) && (j == buildingXpos + 4 || j == buildingXpos + 5))
                        {
                            printf("&");
                        } else {
                            printf("-");
                        }    
                        if (i == buildingYpos + 5 && j == buildingXpos + 7)
                        {
                            if (rb < b)
                            {
                                rb++;
                                countBuilding++;
                            }
                        }
                    } else {
                        printf(" ");
                    }
                }
            }
        }
    }
    free(playerInfo);
    free(dimensions);
    free(itemCoordinates);
    free(getOccurences);
}

int playGame(const char* filename)
{
    int left = 0;
    int right = 0;
    int up = 0;
    int down = 0;
    int quit = 0;
    int input = 1;
    int score = 0;
    char movement = '\n';

    while (1 == 1)
    {
        setupBoard(filename, &left, &right, &up, &down, &score, movement);
        
        while (input == 1)
        {
            input = getInput(filename, &left, &right, &up, &down, &quit, &movement);

            if (input == 1)
            {
                printf("Invalid input.\n");
            }
            
        }

        if (input == 0)
        {
            input = 1;
        }

        if (quit == 1)
        {
            break;
        }
    }
    return 0;
}