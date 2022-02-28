/*
Ändra sedan så att på något sätt vet den att den har nått slutet av oled:en.

*/

//användad uptadeOled() (lab 2 ass 5) för att kunna uppdatera skärmen för att se var ormen är osv.

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h> // for the time function
//#include <pic32mx.h>

//constants 
#define HEIGHT 20 // rows
#define WIDTH 40 // columns

int Field[HEIGHT][WIDTH], x, y, gy, head, tail, game, apples;

// holds the random positions of the apple
int a,b;

//holds the user input from the keyboard
int var;

//holds the direction of the snake
int dir;

void SnakeInit(){
    for(int i=0; i < HEIGHT; i++){
        for(int j=0; j < WIDTH; j++){
            Field[i][j] = 0;
        }
    }

    // setting the snake in the center of the board. will track the position of the snake
    x = HEIGHT/2;
    y = WIDTH/2;
    
    //gy is used to track the position of the field where
    //the snake is meant to be printed. gy is intially set to 20.
    gy = y;

    // assigns the value of the head to 5 and to tail 1. (range - the body should (at the start))
    // not be longer than 5
    head = 5;
    tail = 1;

    //zero means that it's running/starting. if it's 1 it means that the game ends.
    game = 0;

    // if there is no apple on the screen the value will be 0,
    //otherwise 1 and no apple will be addded to the screen.
    apples = 0;

    for(int i = 0; i < head; i++){
        // updating the value of gy +1
        gy++;
        //printing the snake body in (x, 21 - head), (x, 22 - head) etc. 
        Field[x][gy-head] = i+1;
    }
}


void GameBody(){
    for(int i =0;i <= WIDTH + 1; i++){ // top row
        printf("%c", 43);    //character for +
    }
    printf("\n");

    for(int i =0; i < HEIGHT;i++){ // middle of the play field
        printf("%c", 43);
        for(int j = 0; j < WIDTH; j++){
            if(Field[i][j]==0) // if empty
               printf(" "); // a little space
            
            // printing the body/tail of the snake. If Field[i][j] == head then we will
            // accidentally replace the head?
            if(Field[i][j] > 0  &&  Field[i][j] != head)
                printf("O"); // body character

            //printing the head
            if(Field[i][j] == head)
                printf("X"); // head character

            if(Field[i][j]==-1)
                printf("A");

            if(j == WIDTH - 1)
                printf("%c\n",43); // +
        }
    }   

   for(int i=0; i <= WIDTH + 1; i++){ //bottom row, runs until width + 1
        printf("%c", 43);
    }
    printf("\n");
}

void ResetScreenPos(){

    HANDLE hOut; // a void pointer.
    COORD  Pos;  // coordinate for position. keeps the post coordinate of the console screen.
    
    // gives a mechanism for retrieving the standard output.
    hOut = GetStdHandle(STD_OUTPUT_HANDLE); // The standard output device. Initially, this is the active console screen buffer

    // coordinates 0,0 (top left corner)
    Pos.X = 0;
    Pos.Y = 0;
    SetConsoleCursorPosition(hOut, Pos); // settomg the console position to that position (hOut, Pos)

}

void RandomApple(){
    
    //srand() gives the random function a starting point. time(0) gives the time in secondds since the Unix epoch
    /* The srand() function sets the starting point 
    for producing a series of pseudo-random integers. */
    srand(time(0));

    // modulo 18 och 38 eftersom height är 20, width 40 och äpplet får inte vara gömd.
    //rand() generates random numbers and if not srand() is called first 
    // the program will create the same sequence of numbers each time it runs
    a = 1 + rand() % 18;

    b = 1 + rand() % 38;

    // if field is empty then we can put an apple on the screen.
    if(apples == 0 && Field[a][b] == 0){
        Field[a][b] = -1;
        apples = 1; // an apple is now on the screen
    }

}

// get input from the user (keyboard)
int Getch_noblock(){
    
    //if a key on the keyboard is hit
    if(_kbhit())
    //return the character of that key
        return _getch();
    else
        return -1; //nothing
}

void GameOver(){
    
    system("Cls"); // clear the screen
    printf("\n GAME OVER :(");
    game = 1;
}

void Movement(){

    var = Getch_noblock();
    var = tolower(var); //get into lower case. not have to worry about if its uppercase or not.

    /*why the expression abs(dir-var) should be greater than 5 has to do with the difference
    in ascii values of A and D is less than 5 and that of S and W too is less than 5. 
    it's to limit the snake to not move in absurd directions, like from going in D key 
    direction to a key direction it has to first do a w key or s key move */
    if(((var == 'd' || var == 'a') || (var == 'w' || var == 's')) && (abs(dir-var)>5))
        dir = var;
    
    //if the input is equal to the ascii value of D then we want the snake snake to move 
    // in the right direction. 
    if(dir == 'd'){
        y++;
        if((Field[x][y] != 0  && Field[x][y] !=-1)){
            GameOver();
        }
        if(y == WIDTH + 1){
            GameOver();
        }
    }
       
    
    if(dir == 'a'){
        y--; // decrease because it's like a step back
        if(Field[x][y] != 0  && Field[x][y] !=-1){
            GameOver();
        }
        if(y == -2){
            GameOver();
        }
    }

    if(dir == 'w'){
        x--;
        if(Field[x][y] != 0  && Field[x][y] !=-1){
            GameOver();
        }
        if(x == -1){
            GameOver();
        }
    }

    if(dir == 's'){
        x++;
        if(Field[x][y] != 0  && Field[x][y] !=-1){
            GameOver();
        }
        if(x == HEIGHT + 1){
            GameOver();
        }
      // holds the new coordinates of the head.
    }

    //if the head is on the apple then "delete" the apple.
    if(Field[x][y]==-1){
        apples = 0;
        tail--; // gives a longer range - tail increases
    }
    head++; 
    Field[x][y] = head; 
}

void RemoveTail(){
    for(int i=0;i<HEIGHT;i++){
        for(int j =0;j<WIDTH;j++){
            if(Field[i][j]==tail)
                Field[i][j]=0;
        }
    }
    tail++;
}

int main(){
    
    SnakeInit();

    // each time we have finished printing the gamebody, reset the screen position 
    // so that it will look like it's just updating the body of the snake.
    while(game == 0){
        GameBody();
        ResetScreenPos();
        RandomApple();
        Movement();
        RemoveTail();
        Sleep(300); // so that the speed slows down.
    }

    return 0;
}