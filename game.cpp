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
int check(char moving);       //เอาไว้ตรวจสอบว่าขยับได้อยู่ไหม
void menu();                  //สร้างหน้าเมนู
void initnumber();            //สุ่มตำแหน่งบล็อกเลขเริ่มต้น 2 ตัว
void drawframe();             //วาดเฟรม
void movenum_up();            //ขยับบล็อกขึ้น
void movenum_down();          //ขยับบล็อกลง
void movenum_left();          //ขยับบล็อกซ้าย
void movenum_right();         //ขยับบล็อกขวา
void newnumaftermove();       //สุ่มตัวเลขใหม่หลังจากขยับเสร็จ
void fill_number_to_screen(); //นำตัวเลขหลังการขยับเอาเอามาขึ้นจอ
void show_score();            //แสดงคะแนนที่ผู้เล่นทำได้
void arange_score();          //เรียงคะแนน
void setcursor(bool visible); //เปิดปิดcursor
void readfile();              //อ้่านไฟล์ScoreBoard
void writefile();             //เขียนคะแนน
void enteryourname();         //เอาไวั้รับชื่อผู้เล่น
void scoreboard();
void gotoxy(SHORT x, SHORT y);
void setcolor(int fg, int bg);
//////////////////////GLOBAL VARIABLE/////////////////////////////////
unsigned int score = 0;
unsigned int readcount = 0;
int numberposition_x[4] = {9, 21, 33, 45};
int numberposition_y[4] = {9, 14, 19, 24};
int numberonscreen[4][4] = {{0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}}; //mini 2048  [y][x]
int flag[4][4] = {{0, 0, 0, 0},
                  {0, 0, 0, 0},
                  {0, 0, 0, 0},
                  {0, 0, 0, 0}}; //สร้างarry เพิ่มบอกposition ห้ามบวกทับ
bool playing = false;
bool in_menu = true;
bool in_scoreboard = false;
bool runing = true;
struct score
{
    char playername[20];
    unsigned int playerscore;
} player[6];
FILE *fp;
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
    readfile();
    while (runing)
    {
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
                fflush(stdin);
            }
        }
        system("cls");
        drawframe();
        initnumber();
        fill_number_to_screen();
        show_score();
        while (playing)
        {
            if (kbhit()) //ถ้ามาการกดคตีย์บอร์ดระหว่างเล่น
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
                    setcolor(7, 0);
                    system("cls");
                    playing = false;
                    in_menu = true;
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
                    show_score();
                }
            }
            fflush(stdin);
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
    setcolor(5, 0);
    gotoxy(29, 2);
    printf("Use %c %c <- -> to navigate", 24, 25);
    gotoxy(29, 3);
    printf("Press R to restart");
    gotoxy(29, 4);
    printf("Press ESC to quit");
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
                    score = score + numberonscreen[y - 1][x];
                    newnum = true;
                    flag[y - 1][x] = 1;
                    Sleep(75);
                    if (numberonscreen[y - 1][x] == 2048) //ถ้าวกแล้วได้2048
                    {
                        score = score + numberonscreen[y - 1][x];
                        fill_number_to_screen();
                        playing = false;
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
                    score = score + numberonscreen[y + 1][x];
                    Sleep(75);
                    newnum = true;
                    flag[y + 1][x] = 1;
                    if (numberonscreen[y + 1][x] == 2048) //ถ้าวกแล้วได้2048
                    {
                        score = score + numberonscreen[y - 1][x];
                        fill_number_to_screen();
                        playing = false;
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
                    score = score + numberonscreen[y][x - 1];
                    Sleep(75);
                    if (numberonscreen[y][x - 1] == 2048)
                    {
                        score = score + numberonscreen[y][x - 1]; //ถ้าวกแล้วได้2048
                        fill_number_to_screen();
                        playing = false;
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
                    score = score + numberonscreen[y][x + 1];
                    Sleep(75);
                    if (numberonscreen[y][x + 1] == 2048) //ถ้าวกแล้วได้2048
                    {
                        score = score + numberonscreen[y][x + 1];
                        fill_number_to_screen();
                        playing = false;
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
    //SCORE BOARD
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
    //Number
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
    srand(time(NULL));
    setcolor(rand() % 9 + 1, 0);
    gotoxy(18, 5);
    printf("__________________");
    setcolor(rand() % 9 + 1, 0);
    gotoxy(17, 6);
    printf("|                  |");
    setcolor(rand() % 9 + 1, 0);
    gotoxy(17, 7);
    printf("| Enter Your Name  |");
    setcolor(rand() % 9 + 1, 0);
    gotoxy(17, 8);
    printf("|__________________|");
    gotoxy(17, 10);
    setcolor(rand() % 9 + 1, 0);
    printf("-->");
    setcursor(1);
    setcolor(rand() % 9 + 1, 0);
    scanf("%s", player[readcount].playername);
    setcursor(0);
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