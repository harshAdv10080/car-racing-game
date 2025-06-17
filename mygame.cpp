#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <locale>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70 

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];

char car[4][5] = {
    " || ",
    "|==|",
    " || ",
    "|__|"
};

int carPos = WIN_WIDTH / 2;
int score = 0;

void gotoxy(int x, int y){
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size){
    if (size == 0) size = 20;
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

void setColor(int color){
    SetConsoleTextAttribute(console, color);
}

void drawBorder(){
    setColor(7); // White
    for (int i = 0; i < SCREEN_HEIGHT; i++){
        gotoxy(0, i); cout << char(219);
        gotoxy(16, i); cout << char(219);
        gotoxy(WIN_WIDTH, i); cout << char(219);
        gotoxy(SCREEN_WIDTH, i); cout << char(219);
    }
}

void genEnemy(int ind){
    enemyX[ind] = 17 + rand() % (33);
}

void drawEnemy(int ind){
    if (enemyFlag[ind]){
        setColor(12); // Red
        gotoxy(enemyX[ind], enemyY[ind]);     cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 1); cout << " ** ";
        gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 3); cout << " ** ";
    }
}

void eraseEnemy(int ind){
    if (enemyFlag[ind]){
        for (int i = 0; i < 4; i++){
            gotoxy(enemyX[ind], enemyY[ind] + i); cout << "    ";
        }
    }
}

void resetEnemy(int ind){
    eraseEnemy(ind);
    enemyY[ind] = 1;
    genEnemy(ind);
}

void drawCar(){
    setColor(11); // Cyan
    for(int i = 0; i < 4; i++){
        gotoxy(carPos, i + 22); cout << car[i];
    }
}

void eraseCar(){
    for(int i = 0; i < 4; i++){
        gotoxy(carPos, i + 22); cout << "    ";
    }
}

int collision(){
    for (int i = 0; i < 2; i++){
        if (enemyFlag[i] && enemyY[i] + 3 >= 22){
            if (enemyX[i] < carPos + 4 && enemyX[i] + 4 > carPos){
                return 1;
            }
        }
    }
    return 0;
}

void gameover(){
    system("cls");
    setColor(4); // Red
    cout << "\n\n\n";
    cout << "\t\t=====================================\n";
    cout << "\t\t|            GAME OVER              |\n";
    cout << "\t\t=====================================\n\n";
    setColor(7);
    cout << "\t\t    Your Score: " << score << "\n\n";
    cout << "\t\tPress any key to return to menu...";
    getch();
}

void updateScore(){
    setColor(14); // Yellow
    gotoxy(WIN_WIDTH + 7, 5); cout << "Score: " << score;
}

void instructions(){
    system("cls");
    setColor(10); // Green
    cout << "Instructions\n";
    cout << "--------------------------\n";
    cout << "-> Avoid incoming enemy cars.\n";
    cout << "-> Use LEFT ARROW to move Left\n";
    cout << "-> Use RIGHT ARROW to move Right\n";
    cout << "-> Press 'Esc' to quit mid-game\n";
    cout << "\nPress any key to return to menu...";
    getch();
}

void play(){
    carPos = WIN_WIDTH / 2;
    score = 0;
    enemyFlag[0] = 1;
    enemyFlag[1] = 0;
    enemyY[0] = enemyY[1] = 1;

    system("cls");
    drawBorder();
    updateScore();
    genEnemy(0);
    genEnemy(1);

    setColor(14); // Yellow
    gotoxy(WIN_WIDTH + 7, 2);  cout << "CAR GAME";
    gotoxy(WIN_WIDTH + 6, 4);  cout << "-----------";
    gotoxy(WIN_WIDTH + 7, 12); cout << "Controls:";
    gotoxy(WIN_WIDTH + 7, 13); cout << "----------";
    gotoxy(WIN_WIDTH + 2, 14); cout << "← Arrow - Left";
    gotoxy(WIN_WIDTH + 2, 15); cout << "→ Arrow - Right";

    setColor(10); // Green
    gotoxy(18, 5); cout << "Press any key to start";
    getch();
    gotoxy(18, 5); cout << "                        ";

    while (1){
        if (kbhit()) {
            int ch = getch();
            if (ch == 224) {
                int arrow = getch();
                if (arrow == 75) { // Left
                    if (carPos > 18)
                        carPos -= 4;
                }
                else if (arrow == 77) { // Right
                    if (carPos < 50)
                        carPos += 4;
                }
            }
            else if (ch == 27) { // Escape
                break;
            }
        }

        drawCar();
        drawEnemy(0);
        drawEnemy(1);

        if (collision()){
            gameover();
            return;
        }

        Sleep(50);

        eraseCar();
        eraseEnemy(0);
        eraseEnemy(1);

        if (enemyY[0] == 10 && !enemyFlag[1]){
            enemyFlag[1] = 1;
        }

        if (enemyFlag[0]) enemyY[0]++;
        if (enemyFlag[1]) enemyY[1]++;

        if (enemyY[0] > SCREEN_HEIGHT - 4){
            resetEnemy(0);
            score++;
            updateScore();
        }

        if (enemyY[1] > SCREEN_HEIGHT - 4){
            resetEnemy(1);
            score++;
            updateScore();
        }
    }
}

int main(){
    setlocale(LC_ALL, "");
    setcursor(0, 0);
    srand((unsigned)time(NULL));

    while (1){
        system("cls");
        setColor(13); // Magenta
        gotoxy(10, 5); cout << "===============================";
        gotoxy(10, 6); cout << "         CAR RACING GAME       ";
        gotoxy(10, 7); cout << "===============================";
        gotoxy(10, 9); cout << "1. Start Game";
        gotoxy(10,10); cout << "2. Instructions";
        gotoxy(10,11); cout << "3. Quit";
        gotoxy(10,13); cout << "Select option: ";

        char op = getche();
        if (op == '1') play();
        else if (op == '2') instructions();
        else if (op == '3') exit(0);
    }

    return 0;
}
