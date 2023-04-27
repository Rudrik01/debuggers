#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <map>
#include<conio.h>
#include<dos.h> 
#include <windows.h>
#include <time.h>
using namespace std;
fstream file;
map<string,int> users;
int coins = 200;
#define WIN_WIDTH 70 

#define T1POS 15
#define T2POS 30
#define T3POS 45
#define DISKS 5

using namespace std;
 
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

 
int towers[3][DISKS];
int towerTop[3] = {DISKS-1,-1,-1};

int tries = 0; 
int score = 0; 

void gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
void setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;	
	
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}
class Game{
    public:
        string name;
        string password;
        string match;
        string pass;
        Game();
        Game(string &username,string &passw);
        void Register_account(string &username, string &password);
        bool Login(string &name,string &password);
        void Hint();
        void Buycoins();
        void updateScore();
        void instructions();
        void drawTile(int tower, int tileNo, int y);
        void drawTower(int tower);
        int isEmpty(int towerNo);
        int validate(int from, int to);
        int move(int from, int to);
        int win();
        void play();

};

Game::Game(){}
Game::Game(string &username,string &passw) :name{username},password{passw}{
    cout << "Construct called" << endl;
}
void Game::Register_account(string &username, string &password){
   
    string temp;
    
    file.open("record.txt");
    if(!file.is_open()){
        cerr << "Error in opening the file" << endl;
        exit(1);
    }

    while(file >> temp){
            if(temp == username){
                cout << "Usename already exist,Please choose different username!!!" << endl;
                throw 0;
                break;
            }


            while(file << username <<  " " << password){
                    file << endl;
                    users[username] = coins;
                    cout << "Account created successfully" << endl;
                    break;
                }
            }
            file.close();
    }
    /*if (!(file >> temp)) {    
        // ...do something with empty file... 
         while(file << username <<  " " << password){
                //users[username] = coins;
                cout << "Account created successfully" << endl;
                break;
            }

    }*/
bool Game::Login(string &name,string &password){
    file.open("record.txt");
    if(!file.is_open()){
        cerr << "Error in opening the file" << endl;
        exit(1);
    }
    while(file >> match){
        if(match == name){
            file >> pass;
            if(pass == password){
                cout << "Login successful" << endl;
                bool b = true;
                file.close();
                return b;
                break;
            }
            else{
                cout << "Invalid Password" << endl;
                throw 0;
                break;
            }
        }
        else{
            cout << "Username not found!!!!" << endl;
            throw "404";
            break;
        }
    file.close();
    return false;
    }
}
void Game::Hint(){
    srand(time(0));
    if(users[name] > 100){
            users[name] = coins - 50;
            int temp;

            temp =  1+ (rand() % 5);
            if(temp == 1)
                cout << "Start with the smallest disk: Move the smallest disk from the starting pole to the ending pole." << endl;
        
            else if(temp == 2){
                gotoxy(10,6);
                cout << "Work in pairs: Always move two disks at a time. Move the smaller disk onto the larger one, then move the pair to the destination pole." << endl;
            }
            else if(temp == 3){
                cout << "Keep the largest disk at the bottom: Never place a larger disk on top of a smaller one." << endl;
            }
            else if(temp == 4){
                cout << "Use the empty pole: Always use the third pole as an intermediary step, which can be empty at times." << endl;
            }
            else if(temp == 5){
                cout << "Repeat until done: Continue moving the disks in pairs until you have successfully transferred the entire stack to the destination pole." << endl;
            }
    else{
        cout << "You need to Buy coins" << endl;
        char ch;
        cout << "1.Do you want to buy ?(Y/N)";
        cin >> ch;
        if(ch == 'y' || ch == 'Y'){
            Buycoins();
        }
    }
    }
}