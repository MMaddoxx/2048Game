#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#define screen_x 55
#define screen_y 28
#define esc 27
#define keyup 72
#define keydown 80
#define keyleft 75
#define keyright 77
///////////////////PROTORYPE FUNCTION ////////////////////////////////
int setConsole(); //ตั้งกรอบหน้าจอ
int setMode();
void initnumber();
int check(char moving);
void drawframe();
void movenum_up();
void movenum_down();
void movenum_left();
void movenum_right();
void newnumaftermove();
void fill_number_to_screen();
void setcursor(bool visible); //เปิดปิดcursor
void gotoxy(SHORT x, SHORT y);
void setcolor(int fg, int bg);
//////////////////////GLOBAL VARIABLE/////////////////////////////////
unsigned int score = 0;
int numberposition_x[4] = {9, 21, 33, 45};
int numberposition_y[4] = {9, 14, 19, 24};
int numberonscreen[4][4] = {{0, 0, 2, 0},
                            {0, 0, 2, 0},
                            {0, 0, 4, 0},
                            {0, 0, 0, 0}}; //mini 2048  [y][x]
int flag[4][4] = {{0, 0, 0, 0},
                  {0, 0, 0, 0},
                  {0, 0, 0, 0},
                  {0, 0, 0, 0}}; //สร้างarry เพิ่มบอกposition ห้ามบวกทับ
bool playing = true;
HANDLE wHnd;
HANDLE rHnd;
SMALL_RECT windowSize = {0, 0, screen_x - 1, screen_y - 1};
DWORD fdwMode;
//////////////////////////////MAIN////////////////////////////////////
int main()
{
    char ch;
    setcursor(false);
    setConsole();
    setMode();
    drawframe();
    initnumber();
    fill_number_to_screen();
    while (playing)
    {
        if (kbhit())
        {
            ch = getch();
            if (ch == keyup)
            {
                movenum_up();
            }
            else if (ch == keydown)
            {
                movenum_down();
            }
            else if (ch == keyleft)
            {
                movenum_left();
            }
            else if (ch == keyright)
            {
                movenum_right();
            }
            else if (ch == esc)
            {
                playing = false;
            }
        }
        fflush(stdin);
    }
    return 0;
}
//////////////////////////FUNCTION////////////////////////////////////
void setcursor(bool visible)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = 18;
    SetConsoleCursorInfo(console, &lpCursor); //0คือปิด 1 คือเปิด
}
int setConsole()
{
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
}
int setMode()
{
    rHnd = GetStdHandle(STD_INPUT_HANDLE);
    fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    SetConsoleMode(rHnd, fdwMode);
    return 0;
}
void drawframe()
{
    int i, j;
    gotoxy(0, 6);
    printf("    _______________________________________________\n");
    for (j = 1; j <= 4; j++)
    {
        for (i = 1; i <= 4; i++)
        {
            printf("   |           |           |           |           |\n");
        }
        printf("   |___________|___________|___________|___________|\n");
    }
}
void initnumber()
{
    srand(time(NULL));
    int i, x, y;
    for (i = 0; i < 2; i++)
    {

        x = rand() % 4;
        y = rand() % 4;
        while (numberonscreen[y][x] != 0)
        {
            x = rand() % 4;
            y = rand() % 4;
        }
        numberonscreen[y][x] = (rand() % 2) * 2 + 2;
    }
}
void fill_number_to_screen()
{
    int x, y;
    for (y = 0; y < 4; y++)
    {
        for (x = 0; x < 4; x++)
        {
            if (numberonscreen[y][x] == 0)
            {
                setcolor(0, 0);
            }
            else if (numberonscreen[y][x] == 2)
            {
                setcolor(0, 6);
            }
            else if (numberonscreen[y][x] == 4)
            {
                setcolor(7, 1);
            }
            else if (numberonscreen[y][x] == 8)
            {
                setcolor(7, 2);
            }
            else if (numberonscreen[y][x] == 16)
            {
                setcolor(7, 5);
            }
            else if (numberonscreen[y][x] == 32)
            {
                setcolor(7, 12);
            }
            else if (numberonscreen[y][x] == 64)
            {
                setcolor(7, 13);
            }
            else if (numberonscreen[y][x] == 128)
            {
                setcolor(7, 3);
            }
            else if (numberonscreen[y][x] == 256)
            {
                setcolor(0, 11);
            }
            else if (numberonscreen[y][x] == 512)
            {
                setcolor(7, 4);
            }
            else if (numberonscreen[y][x] == 1024)
            {
                setcolor(0, 7);
            }
            else if (numberonscreen[y][x] == 2048)
            {
                setcolor(0, 15);
            }
            if (numberonscreen[y][x] < 10)
            {
                gotoxy(numberposition_x[x] - 3, numberposition_y[y] - 1);
                printf("       ");
                gotoxy(numberposition_x[x] - 3, numberposition_y[y]);
                printf("   %d   ", numberonscreen[y][x]);
                gotoxy(numberposition_x[x] - 3, numberposition_y[y] + 1);
                printf("       ");
            }
            else if (numberonscreen[y][x] < 100)
            {
                gotoxy(numberposition_x[x] - 3, numberposition_y[y] - 1);
                printf("       ");
                gotoxy(numberposition_x[x] - 3, numberposition_y[y]);
                printf("  %d   ", numberonscreen[y][x]);
                gotoxy(numberposition_x[x] - 3, numberposition_y[y] + 1);
                printf("       ");
            }
            else if (numberonscreen[y][x] < 1000)
            {
                gotoxy(numberposition_x[x] - 3, numberposition_y[y] - 1);
                printf("       ");
                gotoxy(numberposition_x[x] - 3, numberposition_y[y]);
                printf("  %d  ", numberonscreen[y][x]);
                gotoxy(numberposition_x[x] - 3, numberposition_y[y] + 1);
                printf("       ");
            }
            else if (numberonscreen[y][x] < 2300)
            {
                gotoxy(numberposition_x[x] - 3, numberposition_y[y] - 1);
                printf("       ");
                gotoxy(numberposition_x[x] - 3, numberposition_y[y]);
                printf("  %d ", numberonscreen[y][x]);
                gotoxy(numberposition_x[x] - 3, numberposition_y[y] + 1);
                printf("       ");
            }
        }
    }
}
void movenum_up()
{
    int x, y;
    bool loop = false;
    bool newnum = false;
    do
    {
        for (y = 1; y < 4; y++)
        {
            for (x = 0; x < 4; x++)
            {
                if (numberonscreen[y][x] != 0 && numberonscreen[y - 1][x] == 0)
                {
                    numberonscreen[y - 1][x] = numberonscreen[y][x];
                    numberonscreen[y][x] = 0;

                    Sleep(100);
                    newnum = true;
                }
                else if (numberonscreen[y][x] == numberonscreen[y - 1][x] && numberonscreen[y][x] != 0 && flag[y - 1][x] == 0 && flag[y][x] == 0)
                {
                    numberonscreen[y - 1][x] *= 2;
                    numberonscreen[y][x] = 0;

                    Sleep(100);
                    newnum = true;
                    flag[y - 1][x] = 1;
                }
            }
            fill_number_to_screen();
        }
        if (check('u') == 0)
        {
            loop = true;
        }
        else
        {
            loop = false;
        }
    } while (loop);
    if (newnum)
    {
        newnumaftermove();

        fill_number_to_screen();
    }
    for (x = 0; x < 4; x++)
    {
        for (y = 0; y < 4; y++)
        {
            flag[y][x] = 0;
        }
    }
}
void movenum_down()
{
    int x, y;
    bool loop = false;
    bool newnum = false;
    do
    {
        for (y = 2; y >= 0; y--)
        {
            for (x = 0; x < 4; x++)
            {
                if (numberonscreen[y][x] != 0 && numberonscreen[y + 1][x] == 0)
                {
                    numberonscreen[y + 1][x] = numberonscreen[y][x];
                    numberonscreen[y][x] = 0;
                    Sleep(100);
                    newnum = true;
                }
                else if (numberonscreen[y][x] == numberonscreen[y + 1][x] && numberonscreen[y][x] != 0 && flag[y + 1][x] == 0 && flag[y][x] == 0)
                {
                    numberonscreen[y + 1][x] *= 2;
                    numberonscreen[y][x] = 0;

                    Sleep(100);
                    newnum = true;
                    flag[y + 1][x] = 1;
                }
            }
            fill_number_to_screen();
        }
        if (check('d') == 0)
        {
            loop = true;
        }
        else
        {
            loop = false;
        }
    } while (loop);
    if (newnum)
    {
        newnumaftermove();

        fill_number_to_screen();
    }
    for (x = 0; x < 4; x++)
    {
        for (y = 0; y < 4; y++)
        {
            flag[y][x] = 0;
        }
    }
}
void movenum_left()
{
    int x, y;
    bool loop = false;
    bool newnum = false;
    do
    {
        for (x = 1; x < 4; x++)
        {
            for (y = 0; y < 4; y++)
            {
                if (numberonscreen[y][x] != 0 && numberonscreen[y][x - 1] == 0)
                {
                    numberonscreen[y][x - 1] = numberonscreen[y][x];
                    numberonscreen[y][x] = 0;

                    Sleep(100);
                    newnum = true;
                }
                else if (numberonscreen[y][x] == numberonscreen[y][x - 1] && numberonscreen[y][x] != 0 && flag[y][x - 1] == 0 && flag[y][x] == 0)
                {
                    numberonscreen[y][x - 1] *= 2;
                    numberonscreen[y][x] = 0;
                    newnum = true;
                    flag[y][x - 1] = 1;
                    Sleep(100);
                }
            }
            fill_number_to_screen();
        }
        if (check('l') == 0)
        {
            loop = true;
        }
        else
        {
            loop = false;
        }
    } while (loop);
    if (newnum)
    {
        newnumaftermove();

        fill_number_to_screen();
    }
    for (x = 0; x < 4; x++)
    {
        for (y = 0; y < 4; y++)
        {
            flag[y][x] = 0;
        }
    }
}
void movenum_right()
{
    int x, y;
    bool loop = false;
    bool newnum = false;
    do
    {
        for (x = 2; x >= 0; x--)
        {
            for (y = 0; y < 4; y++)
            {
                if (numberonscreen[y][x] != 0 && numberonscreen[y][x + 1] == 0)
                {
                    numberonscreen[y][x + 1] = numberonscreen[y][x];
                    numberonscreen[y][x] = 0;
                    Sleep(100);
                    newnum = true;
                }
                else if ((numberonscreen[y][x] == numberonscreen[y][x + 1]) && numberonscreen[y][x] != 0 && flag[y][x + 1] == 0 && flag[y][x] == 0)
                {
                    numberonscreen[y][x + 1] *= 2;
                    numberonscreen[y][x] = 0;
                    Sleep(100);
                    newnum = true;
                    flag[y][x + 1] = 1;
                }
            }
            fill_number_to_screen();
        }
        if (check('r') == 0)
        {
            loop = true;
        }
        else
        {
            loop = false;
        }
    } while (loop);
    if (newnum)
    {
        newnumaftermove();

        fill_number_to_screen();
        newnum = false;
    }
    for (x = 0; x < 4; x++)
    {
        for (y = 0; y < 4; y++)
        {
            flag[y][x] = 0;
        }
    }
}
void newnumaftermove()
{
    srand(time(NULL));
    int x, y;
    x = rand() % 4;
    y = rand() % 4;
    while (numberonscreen[y][x] != 0)
    {
        x = rand() % 4;
        y = rand() % 4;
    }
    numberonscreen[y][x] = (rand() % 2) * 2 + 2;
}
int check(char moving)
{
    int x, y;
    if (moving == 'u')
    {
        for (y = 1; y < 4; y++)
        {
            for (x = 0; x < 4; x++)
            {
                if ((numberonscreen[y][x] != 0 && numberonscreen[y - 1][x] == 0) || (numberonscreen[y][x] == numberonscreen[y - 1][x] && numberonscreen[y][x] != 0 && flag[y][x] == 0))
                {
                    return 0;
                }
            }
        }
    }
    else if (moving == 'd')
    {
        for (y = 2; y >= 0; y--)
        {
            for (x = 0; x < 4; x++)
            {
                if ((numberonscreen[y][x] != 0 && numberonscreen[y + 1][x] == 0 || (numberonscreen[y][x] == numberonscreen[y + 1][x] && numberonscreen[y][x] != 0 && flag[y][x] == 0)))
                {
                    return 0;
                }
            }
        }
    }
    else if (moving == 'l')
    {
        for (x = 1; x < 4; x++)
        {
            for (y = 0; y < 4; y++)
            {
                if ((numberonscreen[y][x] != 0 && numberonscreen[y][x - 1] == 0) || (numberonscreen[y][x] == numberonscreen[y][x - 1] && numberonscreen[y][x] != 0 && flag[y][x] == 0))
                {
                    return 0;
                }
            }
        }
    }
    else if (moving == 'r')
    {
        for (x = 2; x >= 0; x--)
        {
            for (y = 0; y < 4; y++)
            {
                if ((numberonscreen[y][x] != 0 && numberonscreen[y][x + 1] == 0 || (numberonscreen[y][x] == numberonscreen[y][x + 1] && numberonscreen[y][x] != 0 && flag[y][x] == 0)))
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}
void gotoxy(SHORT x, SHORT y)
{
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void setcolor(int fg, int bg)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}