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
void Game::Buycoins(){
    //PAY
    int count = 0;
    gotoxy(10,6);
    cout << "How many coins do you want ?";
    gotoxy(10,6);
    cin >> count;
    users[name] += count;
}
void Game::updateScore(){
	gotoxy(WIN_WIDTH + 7, 5);cout<<"Tries: "<<tries<<endl;
}
void Game::instructions(){
	gotoxy(10,6);cout << "Read Carefully!!!!!" << endl;
	system("pause");
	
	system("cls");
	cout<<"Instructions";
	cout<<"\n----------------";
	cout<<"\n Shift Disks from Tower 1 to Tower 3. ";
	cout<<"\n You can not place large disk on small disk";
	cout<<"\n Towers are Numbered as 1,2 and 3" << endl;
}
void Game::drawTile(int tower, int tileNo, int y){
	int x;
	if( tower == 1 ) x = T1POS;
	else if( tower == 2 ) x = T2POS;
	else if( tower == 3 ) x = T3POS;
	
	x -= tileNo;
	
	for(int j=0; j<((tileNo)*2)-1; j++){
		gotoxy(x,y);
		cout<<"±";
		x++;
	}
}
void Game::drawTower(int tower){
	//int x;
	int y = 9;
	
	gotoxy(10, 10); cout<<"==========";
	gotoxy(25, 10); cout<<"==========";
	gotoxy(40, 10); cout<<"==========";
	
	gotoxy(15, 11); cout<<"1";
	gotoxy(30, 11); cout<<"2";
	gotoxy(45, 11); cout<<"3";
	
	for(int i=0; i<5; i++){
		drawTile(tower, towers[tower-1][i], y);
		y--;
	}
}
int Game::isEmpty(int towerNo){
	for(int i=0; i<DISKS; i++)
		if( towers[towerNo][i] != 0 )
			return 0;
	return 1;
}
int Game::validate(int from, int to){
	if( !isEmpty(to) ){
		  
		if( towers[from][towerTop[from]] < towers[to][towerTop[to]] )
			return 1;
		else 
			return 0;
	}
	return 1;
}
int Game::move(int from, int to){
	if( isEmpty(from) ) return 0; 
	if( validate(from, to) ){ 
		if( towers[from][towerTop[from]] != 0 ){
			towerTop[to]++;
			towers[to][towerTop[to]] = towers[from][towerTop[from]];
			towers[from][towerTop[from]] = 0;
			towerTop[from]--;
			return 1;
		}
	}
	return 0;
}
int Game::win(){ 
	for(int i=0; i<DISKS; i++)
		if( towers[2][i] != DISKS-i )
			return 0;
	return 1;
}

void Game::play(){
	
	gotoxy(10,6);cout << "GEAR UP YOUR SEAT BELT" << endl;
	Sleep(900);
	int from, to;
	for(int i=0; i<DISKS; i++)
		towers[0][i] = DISKS-i;
	for(int i=0; i<DISKS; i++)
		towers[1][i] = 0;
	for(int i=0; i<DISKS; i++)
		towers[2][i] = 0;
	 
	do{
	
		system("cls");  
		
		cout<<"============================================================"<<endl;
		cout<<"                       TOWER OF HANOI                       "<<endl;
		cout<<"============================================================"<<endl<<endl;
		
		drawTower(1);
		drawTower(2);
		drawTower(3);
		
		if( win() ){
			system("cls");
			cout<<"============================================================"<<endl;
			cout<<"                           YOU WIN                          "<<endl;
			cout<<"============================================================"<<endl;
			cout<<endl<<endl<<endl;
			cout<<"Press any key to go back to menu...";
			getch();
			break;
		}
		
		gotoxy(10,15);
		cout<<"From (Values: 1,2,3): ";
		cin>>from;
		gotoxy(10,16);
		cout<<"To (Values: 1,2,3): ";
		cin>>to;
		
		if( to < 1 || to > 3 ) continue;
		if( from < 1 || from > 3 ) continue;
		if( from == to ) continue;
		
		from--;
		to--;
		
		move(from, to);
		 
		if(kbhit()){
			char ch = getch();
			if( ch=='a' || ch=='A' ){ 
			}
			if( ch=='d' || ch=='D' ){ 
			} 
			if(ch==27){
				break;
			}
		} 
		  
	}while(1);
}
int main(){
    system("cls");
    Game T;
    string name;
    string password;
    bool check;
    char ch;
    while(1){
     label_f:
            int t;
           gotoxy(10,6); cout << "1.Create an account"<<endl;
            cout << "2.Login"<<endl;
            cout << "3.exit"<<endl;
            cout << "Enter your choice: ";
            cin >> t;
            if(t==1) {
                label:
                    cout << "LOADING";
                    for(int i = 0;i<6;i++){
                        cout << ".";
                        Sleep(850);
                    }
                    cout << endl;
                    system("cls");
                    cout << "Enter Your username: ";
                    cin >> name;
                    cout << "Enter Password: ";
                    /*ch = _getch();
                    for(int i=0;i<4;i++){//character 13 is enter
                            password.push_back(ch);
                            cout << '*';
                            ch = _getch();
                    }*/
                    cin >> password;
                    try{
                        T.Register_account(name,password);
                    }
                    catch(int &ex){
                        goto label;
                    }
                    cout << endl;
                    goto label_f;
            }
            else if(t==2){
                    label_1:
                    cout << "LOADING";
                    for(int i = 0;i<6;i++){
                        cout << ".";
                        Sleep(850);
                    }
                    cout << endl;
                    system("cls");
                            cout << "Login Page!!!!!!!" << endl;
                            cout << "Enter Your username: ";
                            cin >> name;
                            cout << "Enter Password: ";
                            ch = _getch();
                            for(int i = 0;i<4;i++){//character 13 is enter
                                    password.push_back(ch);
                                    cout << '*';
                                    ch = _getch();
                            }
                            try{
                                check = T.Login(name,password);
                            }
                            catch(int &ex){
                                goto label_1;
                            }
                            catch(string &ex){
                                goto label_1;
                            }
                            if(check){
                                        setcursor(0,0); 
                                        srand( (unsigned)time(NULL)); 
                                        /*The srand() function is used to seed the random number generator in C++ based on the current time, 
                                        so that the sequence of random numbers generated by rand() function will be different every time the program is run.
                                        By seeding the random number generator with srand((unsigned)time(NULL)), 
                                        you will get different random numbers each time you run your program, as the seed value will be based on the current time. 
                                        This can help in generating different game scenarios or random events in your program.*/
                                        //system("color A1");
                                        
                                        do{
                                            cout << "LOADING";
                                            for(int i = 0;i<6;i++){
                                                cout << ".";
                                                Sleep(850);
                                            }
                                            cout << endl;
                        
                                            system("cls"); 	
                                            cout<<"============================================================"<<endl;
                                            cout<<"                    WELCOME TO THE HANOI WORLD!!!!!      "<<endl;
                                            cout<<"============================================================"<<endl<<endl;
                                            Sleep(1200);
                                            system("cls");
                                            gotoxy(10,5); cout<<" -------------------------- "; 
                                            gotoxy(10,6); cout<<" |     Tower of Hanoi     | "; 
                                            gotoxy(10,7); cout<<" --------------------------";
                                            gotoxy(10,9); cout<<"1. Start Game";
                                            gotoxy(10,10); cout<<"2. Instructions";	 
                                            gotoxy(10,11); cout<<"3. Quit";
                                            gotoxy(10,13); cout<<"Select option: ";
                                            char op = getche();
                                            system("cls");
                                            gotoxy(10,6);cout << "Loading";
                                            int r = 17;
                                            for(int i = 0;i<6;i++){
                                                gotoxy(r,6);cout <<".";
                                                r++;
                                                Sleep(850);
                                            }
                                        
                                            gotoxy(17,13);cout << endl;
                                            
                                            if( op=='1') T.play();
                                            else if( op=='2'){ 
                                                T.instructions(); 
                                                system("pause");
                                            }
                                            else if( op=='3') {
                                                system("cls");
                                                cout <<"Thank You For Playing" << endl;
                                                system("pause");
                                                system("cls");
                                                exit(0);
                                            }
                                            
                                        }while(1);
        
                            }
                }
                else if(t==3){
                    system("cls");
                    cout <<"Thank You For Playing" << endl;
                    system("pause");
                    system("cls");
                    exit(0);
                }

        }
        return 0;
}
