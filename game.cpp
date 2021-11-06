#define _WIN32_WINNT 0x0601
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#define screen_x 55
#define screen_y 28
#define screen_x_ingame 100
#define screen_y_ingame 28
#define esc 27
#define keyup 72
#define keydown 80
#define keyleft 75
#define keyright 77
#define cherry 0
#define seven 1
#define bell 2
#define dollar 3
///////////////////PROTORYPE FUNCTION ////////////////////////////////
int setConsole();                      //ตั้งกรอบหน้าจอ
int setConsoleingame();                //ตั้งกรอบให้ใหญ่ขึ้น
int islose();                          //ตรวจสอบการแก้
int check(char moving);                //เอาไว้ตรวจสอบว่าขยับได้อยู่ไหม
void menu();                           //สร้างหน้าเมนู
void initnumber();                     //สุ่มตำแหน่งบล็อกเลขเริ่มต้น 2 ตัว
void drawframe();                      //วาดเฟรม
void movenum_up();                     //ขยับบล็อกขึ้น
void movenum_down();                   //ขยับบล็อกลง
void movenum_left();                   //ขยับบล็อกซ้าย
void movenum_right();                  //ขยับบล็อกขวา
void newnumaftermove();                //สุ่มตัวเลขใหม่หลังจากขยับเสร็จ
void fill_number_to_screen();          //นำตัวเลขหลังการขยับเอาเอามาขึ้นจอ
void show_score();                     //แสดงคะแนนที่ผู้เล่นทำได้
void arange_score();                   //เรียงคะแนน
void setcursor(bool visible);          //เปิดปิดcursor
void readfile();                       //อ้่านไฟล์ScoreBoard
void writefile();                      //เขียนคะแนน
void enteryourname();                  //เอาไวั้รับชื่อผู้เล่น
void displayitem(int item);            //เอาitemขึ้นจอ
void randomitem();                     //สุ่ม item
int choseblock(int chosing, int item); //เลือกบล็อคที่จะทำลาย
void highlightnum(int x, int y);       //สร้างกรอบตอนเลือก
void clearitem();                      //ล้างหน้า item
void setfont();                        // font
void scoreboard();
void gotoxy(SHORT x, SHORT y);
void setcolor(int fg, int bg);
//////////////////////GLOBAL VARIABLE/////////////////////////////////
unsigned int score = 0;
unsigned int fakescore = 0;
unsigned int readcount = 0;
unsigned int doublescore = 0;
int numberposition_x[4] = {9, 21, 33, 45};
int numberposition_y[4] = {9, 14, 19, 24};
int numberonscreen[4][4] = {{0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}}; // mini 2048  [y][x]
int flag[4][4] = {{0, 0, 0, 0},
                  {0, 0, 0, 0},
                  {0, 0, 0, 0},
                  {0, 0, 0, 0}}; //สร้างarry เพิ่มบอกposition ห้ามบวกทับ
bool playing = false;
bool in_menu = true;
bool in_scoreboard = false;
bool runing = true;
bool gameover = false;
struct score
{
    char playername[20];
    unsigned int playerscore;
} player[6];
typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT FontFamily;
    UINT FontWeight;
    WCHAR FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;
#ifdef __cplusplus
extern "C"
{
#endif
    BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
#ifdef __cplusplus
}
#endif
FILE *fp;
HANDLE wHnd;
HANDLE rHnd;
SMALL_RECT windowSize = {0, 0, screen_x - 1, screen_y - 1};
SMALL_RECT windowSizeingame = {0, 0, screen_x_ingame - 1, screen_y_ingame - 1};
DWORD fdwMode;
DWORD numEvents = 0;
DWORD numEventsRead = 0;
//////////////////////////////MAIN////////////////////////////////////
int main()
{
    char ch;
    HWND consoleWindow = GetConsoleWindow();
    setcursor(false);
    setConsole();
    readfile();
    setfont();
    while (runing)
    {
        setConsole();
        SetWindowPos(consoleWindow, 0, 540, 180, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        while (in_menu)
        {
            menu();
            if (kbhit())
            {
                ch = getch();
                if (ch == '1')
                {
                    system("cls");
                    enteryourname();
                    in_menu = false;
                    playing = true;
                }
                else if (ch == '2')
                {
                    system("cls");
                    in_scoreboard = true;
                    while (in_scoreboard)
                    {
                        scoreboard();
                        if (kbhit())
                        {
                            ch = getch();
                            if (ch == esc)
                            {
                                in_scoreboard = false;
                            }
                        }
                    }
                    system("cls");
                }
                else if (ch == esc)
                {
                    in_menu = false;
                    runing = false;
                }
            }
            fflush(stdin);
            Sleep(100);
        }
        system("cls");
        setConsoleingame();
        drawframe();
        initnumber();
        fill_number_to_screen();
        show_score();
        SetWindowPos(consoleWindow, 0, 400, 150, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        while (playing)
        {
            if (kbhit())
            {
                ch = getch();
                if (ch == keyup && gameover == false)
                {
                    movenum_up();
                }
                else if (ch == keydown && gameover == false)
                {
                    movenum_down();
                }
                else if (ch == keyleft && gameover == false)
                {
                    movenum_left();
                }
                else if (ch == keyright && gameover == false)
                {
                    movenum_right();
                }
                else if (ch == esc)
                {
                    setcolor(7, 0);
                    system("cls");
                    fakescore = 0;
                    playing = false;
                    in_menu = true;
                    gameover = false;
                    for (int y = 0; y < 4; y++)
                    {
                        for (int x = 0; x < 4; x++)
                        {
                            numberonscreen[y][x] = 0;
                        }
                    }
                    if (score != 0)
                    {
                        player[readcount].playerscore = score;
                        writefile();
                        score = 0;
                    }
                }
                else if (ch == 'r')
                {
                    for (int y = 0; y < 4; y++)
                    {
                        for (int x = 0; x < 4; x++)
                        {
                            numberonscreen[y][x] = 0;
                        }
                    }
                    if (score != 0)
                    {
                        player[readcount].playerscore = score;
                        writefile();
                        score = 0;
                    }
                    setcolor(7, 0);
                    system("cls");
                    drawframe();
                    initnumber();
                    fill_number_to_screen();
                    score = 0;
                    fakescore = 0;
                    show_score();
                    gameover = false;
                }
                else if (ch == 'i' && fakescore >= 500 && gameover == false)
                {
                    randomitem();
                }
                fflush(stdin);
                if (islose() == 1)
                {
                    Sleep(2000);
                    for (int y = 0; y < 4; y++)
                    {
                        for (int x = 0; x < 4; x++)
                        {
                            numberonscreen[y][x] = 0;
                        }
                    }
                    fill_number_to_screen();
                    setcolor(7, 4);
                    for (int i = 13; i <= 15; i++)
                    {
                        gotoxy(16, i);
                        printf("                       ");
                    }
                    gotoxy(16, 16);
                    printf("       Game Over       ");
                    for (int i = 17; i <= 19; i++)
                    {
                        gotoxy(16, i);
                        printf("                       ");
                    }
                }
            }
            Sleep(100);
        }
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
    SetConsoleCursorInfo(console, &lpCursor); // 0คือปิด 1 คือเปิด
}
int setConsole()
{
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
}
int setConsoleingame()
{
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleWindowInfo(wHnd, TRUE, &windowSizeingame);
}
void menu()
{
    //////////////2048///////////////////////////
    gotoxy(10, 2);
    setcolor(7, 0);
    printf("___________________________________");
    gotoxy(9, 3);
    printf("|");
    setcolor(0, 6);
    printf("                                   ");
    setcolor(7, 0);
    printf("|");
    gotoxy(9, 4);
    printf("|");
    setcolor(7, 6);
    printf("               2048                ");
    setcolor(7, 0);
    printf("|");
    gotoxy(9, 5);
    printf("|");
    setcolor(7, 6);
    printf("___________________________________");
    setcolor(7, 0);
    printf("|");
    //////////////////Press 1///////////////////
    gotoxy(11, 9);
    printf(" ______________________________");
    gotoxy(11, 10);
    printf("|");
    setcolor(0, 5);
    printf("                              ");
    setcolor(7, 0);
    printf("|");
    gotoxy(10, 11);
    printf("<");
    setcolor(7, 5);
    printf("     Press 1 to start game      ");
    setcolor(7, 0);
    printf(">");
    gotoxy(11, 12);
    printf("|");
    setcolor(7, 5);
    printf("______________________________");
    setcolor(7, 0);
    printf("|");
    //////////////////Press 2/////////////////
    gotoxy(11, 15);
    printf(" ______________________________");
    gotoxy(11, 16);
    printf("|");
    setcolor(0, 3);
    printf("                              ");
    setcolor(7, 0);
    printf("|");
    gotoxy(10, 17);
    printf("<");
    setcolor(7, 3);
    printf("   Press 2 to see scoreboard    ");
    setcolor(7, 0);
    printf(">");
    gotoxy(11, 18);
    printf("|");
    setcolor(7, 3);
    printf("______________________________");
    setcolor(7, 0);
    printf("|");
}
void drawframe()
{
    int i, j;
    gotoxy(0, 6);
    setcolor(7, 0);
    printf("    _______________________________________________\n");
    for (j = 1; j <= 4; j++)
    {
        for (i = 1; i <= 4; i++)
        {
            printf("   |           |           |           |           |\n");
        }
        printf("   |___________|___________|___________|___________|\n");
    }
    gotoxy(0, 1);
    setcolor(2, 0);
    printf("    __________________\n");
    printf("   |                  |\n");
    printf("   |                  |\n");
    printf("   |__________________|\n");
    setcolor(1, 0);
    gotoxy(29, 1);
    printf("Use %c %c <- -> to navigate", 24, 25);
    gotoxy(29, 2);
    printf("Press R to restart");
    gotoxy(29, 3);
    printf("Press ESC to quit");
    gotoxy(29, 4);
    printf("Press I to random an item ");
    gotoxy(29, 5);
    printf("Press SpaceBar to use item");
    setcolor(7, 0);
    gotoxy(69, 4);
    printf("Spacial item");
    gotoxy(61, 6);
    printf("____________________________");
    for (int i = 7; i <= 18; i++)
    {
        gotoxy(60, i);
        printf("|                            |");
    }
    gotoxy(60, 19);
    printf("|____________________________|");
    gotoxy(58, 25);
    printf("Every 500 points you can get 1 item");
    gotoxy(58, 26);
    printf("Each item has it's own unique effect");
}
void initnumber()
{
    srand(time(NULL));
    int i, x, y;
    for (i = 0; i < 2; i++)
    {
        x = rand() % 4;
        y = rand() % 4;
        while (numberonscreen[y][x] != 0) //ถ้าได้ตำแหน่งที่ไม่ใช่ 0 ให้สุ่มใหม่จนกว่าจะได้
        {
            x = rand() % 4;
            y = rand() % 4;
        }
        numberonscreen[y][x] = (rand() % 2) * 2 + 2; //สุ่มว่าจะเป็น 2 หรือ 4
    }
}
void fill_number_to_screen()
{
    int x, y;
    for (y = 0; y < 4; y++)
    {
        for (x = 0; x < 4; x++)
        {
            if (numberonscreen[y][x] == 0) //เซทสี
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
                setcolor(0, 10);
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
                if (numberonscreen[y][x] != 0 && numberonscreen[y - 1][x] == 0) //ถ้าหากข้างหน้าเป็นช่องว่าง (0) ก็จะขยับขึ้นไปได้เลย
                {
                    numberonscreen[y - 1][x] = numberonscreen[y][x];
                    numberonscreen[y][x] = 0;
                    Sleep(75);
                    newnum = true;
                }
                else if (numberonscreen[y][x] == numberonscreen[y - 1][x] && numberonscreen[y][x] != 0 && flag[y - 1][x] == 0 && flag[y][x] == 0)
                { //ถ้าหากว่าเป็นตัวที่เหมือนกันและไม่ติดflag(ไม่เคยบวกมาก่อน)
                    numberonscreen[y - 1][x] *= 2;
                    numberonscreen[y][x] = 0;
                    if (doublescore > 0)
                    {
                        score = score + (numberonscreen[y - 1][x] * 2);
                        fakescore = fakescore + (numberonscreen[y - 1][x] * 2);
                        doublescore--;
                    }
                    else
                    {
                        score = score + numberonscreen[y - 1][x];
                        fakescore = fakescore + numberonscreen[y - 1][x];
                    }
                    newnum = true;
                    flag[y - 1][x] = 1;
                    Sleep(75);
                    if (numberonscreen[y - 1][x] == 2048) //ถ้าวกแล้วได้2048
                    {
                        score = score + numberonscreen[y - 1][x];
                        fill_number_to_screen();
                        show_score();
                        gameover = true;
                        Sleep(2000);
                        for (int y = 0; y < 4; y++)
                        {
                            for (int x = 0; x < 4; x++)
                            {
                                numberonscreen[y][x] = 0;
                            }
                        }
                        fill_number_to_screen();
                        setcolor(7, 4);
                        for (int i = 13; i <= 15; i++)
                        {
                            gotoxy(16, i);
                            printf("                       ");
                        }
                        gotoxy(16, 16);
                        printf("        YOU WIN        ");
                        for (int i = 17; i <= 19; i++)
                        {
                            gotoxy(16, i);
                            printf("                       ");
                        }
                        return;
                    }
                }
            }
            fill_number_to_screen();
            show_score();
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
            flag[y][x] = 0; //พอขยับและบวกเลขหมดแล้วก็รีเซทflag
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
                if (numberonscreen[y][x] != 0 && numberonscreen[y + 1][x] == 0) //ถ้าหากข้างหน้าเป็นช่องว่าง (0) ก็จะขยับลงไปได้เลย
                {
                    numberonscreen[y + 1][x] = numberonscreen[y][x];
                    numberonscreen[y][x] = 0;
                    Sleep(75);
                    newnum = true;
                }
                else if (numberonscreen[y][x] == numberonscreen[y + 1][x] && numberonscreen[y][x] != 0 && flag[y + 1][x] == 0 && flag[y][x] == 0)
                { //ถ้าหากว่าเป็นตัวที่เหมือนกันและไม่ติดflag(ไม่เคยบวกมาก่อน)
                    numberonscreen[y + 1][x] *= 2;
                    numberonscreen[y][x] = 0;
                    if (doublescore > 0)
                    {
                        score = score + (numberonscreen[y + 1][x] * 2);
                        fakescore = fakescore + (numberonscreen[y + 1][x] * 2);
                    }
                    else
                    {
                        score = score + numberonscreen[y + 1][x];
                        fakescore = fakescore + numberonscreen[y + 1][x];
                    }
                    Sleep(75);
                    newnum = true;
                    flag[y + 1][x] = 1;
                    if (numberonscreen[y + 1][x] == 2048) //ถ้าวกแล้วได้2048
                    {
                        score = score + numberonscreen[y - 1][x];
                        fill_number_to_screen();
                        gameover = true;
                        show_score();
                        Sleep(2000);
                        for (int y = 0; y < 4; y++)
                        {
                            for (int x = 0; x < 4; x++)
                            {
                                numberonscreen[y][x] = 0;
                            }
                        }
                        fill_number_to_screen();
                        setcolor(7, 4);
                        for (int i = 13; i <= 15; i++)
                        {
                            gotoxy(16, i);
                            printf("                       ");
                        }
                        gotoxy(16, 16);
                        printf("        YOU WIN        ");
                        for (int i = 17; i <= 19; i++)
                        {
                            gotoxy(16, i);
                            printf("                       ");
                        }
                        return;
                    }
                }
            }
            fill_number_to_screen();
            show_score();
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
    for (x = 0; x < 4; x++) //พอขยับและบวกเลขหมดแล้วก็รีเซทflag
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
                if (numberonscreen[y][x] != 0 && numberonscreen[y][x - 1] == 0) //ถ้าหากข้างหน้าเป็นช่องว่าง (0) ก็จะขยับซ้ายไปได้เลย
                {
                    numberonscreen[y][x - 1] = numberonscreen[y][x];
                    numberonscreen[y][x] = 0;
                    Sleep(75);
                    newnum = true;
                }
                else if (numberonscreen[y][x] == numberonscreen[y][x - 1] && numberonscreen[y][x] != 0 && flag[y][x - 1] == 0 && flag[y][x] == 0)
                { //ถ้าหากว่าเป็นตัวที่เหมือนกันและไม่ติดflag(ไม่เคยบวกมาก่อน)
                    numberonscreen[y][x - 1] *= 2;
                    numberonscreen[y][x] = 0;
                    newnum = true;
                    flag[y][x - 1] = 1;
                    if (doublescore > 0)
                    {
                        score = score + (numberonscreen[y][x - 1] * 2);
                        fakescore = fakescore + (numberonscreen[y][x - 1] * 2);
                    }
                    else
                    {
                        score = score + numberonscreen[y][x - 1];
                        fakescore = fakescore + numberonscreen[y][x - 1];
                    }
                    Sleep(75);
                    if (numberonscreen[y][x - 1] == 2048)
                    {
                        score = score + numberonscreen[y][x - 1]; //ถ้าวกแล้วได้2048
                        fill_number_to_screen();
                        gameover = true;
                        show_score();
                        Sleep(2000);
                        for (int y = 0; y < 4; y++)
                        {
                            for (int x = 0; x < 4; x++)
                            {
                                numberonscreen[y][x] = 0;
                            }
                        }
                        fill_number_to_screen();
                        setcolor(7, 4);
                        for (int i = 13; i <= 15; i++)
                        {
                            gotoxy(16, i);
                            printf("                       ");
                        }
                        gotoxy(16, 16);
                        printf("        YOU WIN        ");
                        for (int i = 17; i <= 19; i++)
                        {
                            gotoxy(16, i);
                            printf("                       ");
                        }
                        return;
                    }
                }
            }
            fill_number_to_screen();
            show_score();
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
            flag[y][x] = 0; //พอขยับและบวกเลขหมดแล้วก็รีเซทflag
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
                if (numberonscreen[y][x] != 0 && numberonscreen[y][x + 1] == 0) //ถ้าหากข้างหน้าเป็นช่องว่าง (0) ก็จะขยับขวาไปได้เลย
                {
                    numberonscreen[y][x + 1] = numberonscreen[y][x];
                    numberonscreen[y][x] = 0;
                    Sleep(75);
                    newnum = true;
                }
                else if ((numberonscreen[y][x] == numberonscreen[y][x + 1]) && numberonscreen[y][x] != 0 && flag[y][x + 1] == 0 && flag[y][x] == 0)
                { //ถ้าหากว่าเป็นตัวที่เหมือนกันและไม่ติดflag(ไม่เคยบวกมาก่อน)
                    numberonscreen[y][x + 1] *= 2;
                    numberonscreen[y][x] = 0;
                    newnum = true;
                    flag[y][x + 1] = 1;
                    if (doublescore > 0)
                    {
                        score = score + (numberonscreen[y][x + 1] * 2);
                        fakescore = fakescore + (numberonscreen[y][x + 1] * 2);
                    }
                    else
                    {
                        score = score + numberonscreen[y][x + 1];
                        fakescore = fakescore + numberonscreen[y][x + 1];
                    }
                    Sleep(75);
                    if (numberonscreen[y][x + 1] == 2048) //ถ้าวกแล้วได้2048
                    {
                        score = score + numberonscreen[y][x + 1];
                        fill_number_to_screen();
                        gameover = true;
                        show_score();
                        Sleep(2000);
                        for (int y = 0; y < 4; y++)
                        {
                            for (int x = 0; x < 4; x++)
                            {
                                numberonscreen[y][x] = 0;
                            }
                        }
                        fill_number_to_screen();
                        setcolor(7, 4);
                        for (int i = 13; i <= 15; i++)
                        {
                            gotoxy(16, i);
                            printf("                       ");
                        }
                        gotoxy(16, 16);
                        printf("        YOU WIN        ");
                        for (int i = 17; i <= 19; i++)
                        {
                            gotoxy(16, i);
                            printf("                       ");
                        }
                        return;
                    }
                }
            }
            fill_number_to_screen();
            show_score();
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
            flag[y][x] = 0; //พอขยับและบวกเลขหมดแล้วก็รีเซทflag
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
void show_score()
{
    gotoxy(4, 3);
    setcolor(0, 7);
    printf("Score : %d", score);
    setcolor(7, 0);
    gotoxy(4, 5);
    printf("You can draw %d item(s)", fakescore / 500);
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
                if ((numberonscreen[y][x] != 0 && numberonscreen[y - 1][x] == 0) || (numberonscreen[y][x] == numberonscreen[y - 1][x] && numberonscreen[y][x] != 0 && flag[y][x] == 0 && flag[y - 1][x] == 0))
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
                if ((numberonscreen[y][x] != 0 && numberonscreen[y + 1][x] == 0 || (numberonscreen[y][x] == numberonscreen[y + 1][x] && numberonscreen[y][x] != 0 && flag[y][x] == 0 && flag[y + 1][x] == 0)))
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
                if ((numberonscreen[y][x] != 0 && numberonscreen[y][x - 1] == 0) || (numberonscreen[y][x] == numberonscreen[y][x - 1] && numberonscreen[y][x] != 0 && flag[y][x] == 0 && flag[y][x - 1] == 0))
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
                if ((numberonscreen[y][x] != 0 && numberonscreen[y][x + 1] == 0 || (numberonscreen[y][x] == numberonscreen[y][x + 1] && numberonscreen[y][x] != 0 && flag[y][x] == 0 && flag[y][x + 1] == 0)))
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}
void readfile()
{
    fp = fopen("2048_ScoreBoard.txt", "r");
    do
    {
        fscanf(fp, "%d %s", &player[readcount].playerscore, player[readcount].playername);
        readcount++;
        printf("%s\n", player[readcount].playername);
    } while (player[readcount - 1].playername[0] != 'x' && readcount <= 4);
}
void writefile()
{
    int i, j, a;
    char name[20];
    fp = fopen("2048_ScoreBoard.txt", "w");
    for (i = 0; i <= readcount; i++)
    {
        for (j = i + 1; j <= readcount; j++)
        {
            if (player[i].playerscore < player[j].playerscore)
            {
                a = player[j].playerscore;
                player[j].playerscore = player[i].playerscore;
                player[i].playerscore = a;
                strcpy(name, player[j].playername);
                strcpy(player[j].playername, player[i].playername);
                strcpy(player[i].playername, name);
            }
        }
    }
    for (i = 0; i < readcount; i++)
    {
        fprintf(fp, "%d %s\n", player[i].playerscore, player[i].playername);
    }
    if (readcount < 5)
    {
        for (i = 0; i <= 5 - readcount; i++)
        {
            fprintf(fp, "0 x\n");
        }
    }
    fclose(fp);
}
void scoreboard()
{
    // SCORE BOARD
    gotoxy(10, 2);
    setcolor(7, 0);
    printf("___________________________________");
    gotoxy(9, 3);
    printf("|");
    setcolor(7, 3);
    printf("                                   ");
    setcolor(7, 0);
    printf("|");
    gotoxy(9, 4);
    printf("|");
    setcolor(7, 3);
    printf("            SCORE BOARD            ");
    setcolor(7, 0);
    printf("|");
    gotoxy(9, 5);
    printf("|");
    setcolor(7, 3);
    printf("___________________________________");
    setcolor(7, 0);
    printf("|");
    // Number
    int i;
    gotoxy(11, 8);
    printf("Name-Score");
    for (i = 0; i < 5; i++)
    {
        gotoxy(8, 10 + 3 * i);
        setcolor(rand() % 9 + 1, 0);
        printf("%d. %s %d\n\n", i + 1, player[i].playername, player[i].playerscore);
    }
    gotoxy(11, 25);
    setcolor(7, 0);
    printf("Press ESC to exit to menu");
}
void enteryourname()
{
    setcolor(7, 0);
    gotoxy(18, 5);
    printf("__________________");
    gotoxy(17, 6);
    printf("|                  |");
    gotoxy(17, 7);
    printf("| Enter Your Name  |");
    gotoxy(17, 8);
    printf("|__________________|");
    gotoxy(17, 10);
    printf("-->");
    setcursor(1);
    scanf("%[^\n]s", player[readcount].playername);
    setcursor(0);
}
int islose()
{
    int x = 0, y = 0, count = 0;
    for (y = 0; y <= 3; y++)
    {
        for (x = 0; x < 3;)
        {
            if ((numberonscreen[y][x] != numberonscreen[y][x + 1]) && (numberonscreen[y][x] != 0) && (numberonscreen[y][x + 1] != 0))
            {
                count++;
            }
            x += 2;
        }
    }
    for (y = 0; y < 3;)
    {
        for (x = 0; x <= 3; x++)
        {
            if ((numberonscreen[y][x] != numberonscreen[y + 1][x]) && (numberonscreen[y][x] != 0) && (numberonscreen[y + 1][x] != 0))
            {
                count++;
            }
        }
        y += 2;
    }
    if (count == 16)
    {
        return 1; //แพ้
    }
    else
    {
        return 0; //ไม่แพ้
    }
}
void randomitem()
{
    int count = 0, item;
    if (fakescore >= 500)
    {
        int a = 50;
        srand(time(NULL));
        fakescore -= 500;
        while (count <= 15)
        {
            item = rand() % 4;
            Beep(500, 200);
            displayitem(item);
            count++;
            Sleep(a);
            a += 50;
        }
        Beep(2000, 200);
    }
    switch (item)
    {
    case cherry:
    {
        gotoxy(66, 22);
        setcolor(7, 0);
        printf("You Got 1000 Points");
        score += 1000;
        show_score();
        Sleep(2000);
        clearitem();
        break;
    }
    case seven:
    {
        show_score();
        gotoxy(59, 22);
        setcolor(7, 0);
        printf("You can remove 1 block from board");
        gotoxy(59, 23);
        printf("Press Space bar to select");
        choseblock(1, 0);
        clearitem();
        break;
    }
    case bell:
    {
        show_score();
        gotoxy(59, 22);
        setcolor(7, 0);
        printf("You can change 1 number(No 1024,2048)");
        gotoxy(59, 23);
        printf("Press Space bar to select");
        choseblock(1, 1);
        clearitem();
        break;
    }
    case dollar:
    {
        show_score();
        gotoxy(59, 22);
        setcolor(7, 0);
        printf("You have 2x points 10 times");
        doublescore += 11;
        show_score();
        Sleep(2500);
        clearitem();
        break;
    }
    }
}
int choseblock(int chosing, int item)
{
    char ch;
    int x = 0, y = 0, count = 0;
    highlightnum(x, y);
    while (chosing > 0)
    {
        if (item == 0)
        {
            for (int a = 0; a <= 3; a++)
            {
                for (int b = 0; b <= 3; b++)
                {
                    if (numberonscreen[b][a] == 0)
                    {
                        count++;
                    }
                }
            }
            if (count == 15)
            {
                gotoxy(54, 23);
                setcolor(3, 0);
                printf("Looks like you can't remove any more block");
                gotoxy(54, 24);
                printf("You got 1000 score instate");
                score += 1000;
                show_score();
                Sleep(3000);
                setcolor(0, 0);
                gotoxy(54, 23);
                printf("                                          ");
                gotoxy(54, 24);
                printf("                           ");
                clearitem();
                fill_number_to_screen();

                return 0;
            }
        }
        if (kbhit())
        {
            ch = getch();
            if (ch == keyup && y > 0)
            {
                y--;
            }
            else if (ch == keydown && y < 3)
            {
                y++;
            }
            else if (ch == keyleft && x > 0)
            {
                x--;
            }
            else if (ch == keyright && x < 3)
            {
                x++;
            }
            else if (ch == ' ' && numberonscreen[y][x] != 0)
            {
                chosing--;
                if (item == 0)
                {
                    numberonscreen[y][x] = 0;
                    fill_number_to_screen();
                }
                else if (item == 1)
                {
                    bool loop = true;
                    do
                    {
                        setcolor(7, 0);
                        gotoxy(numberposition_x[x] - 2, numberposition_y[y]);
                        printf("     ");
                        gotoxy(numberposition_x[x] - 2, numberposition_y[y]);
                        setcursor(1);
                        scanf("%d", &numberonscreen[y][x]);
                        for (int i = 1; i <= 9; i++)
                        {
                            if (numberonscreen[y][x] == pow(2, i))
                            {
                                loop = false;
                                break;
                            }
                        }
                    } while (loop);
                    setcursor(0);
                    fill_number_to_screen();
                }
                break;
            }
            fflush(stdin);
            fill_number_to_screen();
            highlightnum(x, y);
        }
    }
}
void highlightnum(int x, int y)
{
    setcolor(0, 7);
    gotoxy(numberposition_x[x] - 3, numberposition_y[y] - 1);
    printf("       ");
    gotoxy(numberposition_x[x] - 3, numberposition_y[y]);
    printf(" ");
    gotoxy(numberposition_x[x] + 3, numberposition_y[y]);
    printf(" ");
    gotoxy(numberposition_x[x] - 3, numberposition_y[y] + 1);
    printf("       ");
}
void displayitem(int item)
{
    switch (item)
    {
    case cherry:
    {
        setcolor(0, 7);
        for (int i = 8; i <= 18; i++)
        {
            gotoxy(61, i);
            printf("                            ");
        }
        gotoxy(80, 8);
        setcolor(0, 2);
        printf("  ");
        gotoxy(78, 9);
        printf("  ");
        gotoxy(76, 10);
        printf("  ");
        gotoxy(74, 11);
        printf("  ");
        gotoxy(70, 12);
        setcolor(0, 4);
        printf("          ");
        gotoxy(68, 13);
        printf("              ");
        gotoxy(66, 14);
        printf("                  ");
        gotoxy(66, 15);
        printf("                  ");
        gotoxy(66, 16);
        printf("                  ");
        gotoxy(68, 17);
        printf("              ");
        gotoxy(70, 18);
        setcolor(0, 4);
        printf("          ");
        setcolor(0, 7);
        gotoxy(76, 14);
        printf("  ");
        gotoxy(74, 15);
        printf("  ");
        gotoxy(74, 16);
        printf("  ");
        break;
    }
    case seven:
    {
        setcolor(0, 1);
        int a = 80;
        for (int i = 8; i <= 18; i++)
        {
            gotoxy(61, i);
            printf("                            ");
        }
        setcolor(0, 7);
        for (int i = 8; i <= 9; i++)
        {
            gotoxy(64, i);
            printf("                      ");
        }
        for (int i = 10; i < 15; i++)
        {
            gotoxy(a, i);
            printf("      ");
            a -= 2;
        }
        for (int i = 15; i <= 18; i++)
        {
            gotoxy(a + 2, i);
            printf("      ");
        }
        break;
    }
    case bell:
    {
        setcolor(0, 5);
        for (int i = 8; i <= 18; i++)
        {
            gotoxy(61, i);
            printf("                            ");
        }
        setcolor(0, 0);
        gotoxy(73, 9);
        printf("    ");
        gotoxy(71, 10);
        printf("        ");
        for (int i = 11; i <= 14; i++)
        {
            gotoxy(69, i);
            printf("            ");
        }
        gotoxy(67, 15);
        printf("                ");
        gotoxy(72, 16);
        printf("      ");
        gotoxy(73, 17);
        printf("    ");
        setcolor(0, 6);
        gotoxy(73, 10);
        printf("    ");
        for (int i = 11; i <= 14; i++)
        {
            gotoxy(71, i);
            printf("        ");
        }
        gotoxy(73, 16);
        printf("    ");
        gotoxy(71, 12);
        setcolor(0, 14);
        printf("        ");
        break;
    }
    case dollar:
    {
        setcolor(0, 6);
        for (int i = 8; i <= 18; i++)
        {
            gotoxy(61, i);
            printf("                            ");
        }
        setcolor(0, 2);
        for (int i = 9; i <= 18; i++)
        {
            gotoxy(73, i);
            printf(" ");
            gotoxy(77, i);
            printf(" ");
        }
        gotoxy(70, 10);
        printf("           ");
        gotoxy(68, 11);
        printf("               ");
        gotoxy(68, 12);
        printf("    ");
        gotoxy(81, 12);
        printf("  ");
        gotoxy(70, 13);
        printf("          ");
        gotoxy(72, 14);
        printf("          ");
        gotoxy(81, 15);
        printf("  ");
        gotoxy(68, 15);
        printf("    ");
        gotoxy(70, 16);
        printf("            ");
        gotoxy(72, 17);
        printf("        ");
        break;
    }
    }
}
void clearitem()
{
    setcolor(0, 0);
    for (int i = 8; i <= 18; i++)
    {
        gotoxy(61, i);
        printf("                            ");
    }
    gotoxy(58, 22);
    printf("                                      ");
    gotoxy(58, 23);
    printf("                                      ");
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
void setfont()
{
    int newWidth = 8, newHeight = 18;
    CONSOLE_FONT_INFOEX fontStructure = {0};
    fontStructure.cbSize = sizeof(fontStructure);
    fontStructure.dwFontSize.X = newWidth;
    fontStructure.dwFontSize.Y = newHeight;
    wcscpy(fontStructure.FaceName, L"PSL Kittithada Pro");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetCurrentConsoleFontEx(hConsole, true, &fontStructure);
}