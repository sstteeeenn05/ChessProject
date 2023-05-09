#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

vector<string> board = {

        "rnbqkbnr",
        "pppppppp",
        "        ",
        "        ",
        "        ",
        "        ",
        "PPPPPPPP",
        "RNBQKBNR"
};

void move(int x1, int y1, int x2, int y2) {
    random_device dev;
    mt19937 gen(dev());
    int rand = gen() % 3;
    switch (rand) {
        case 0:
            cout << "failed" << '\n';
            break;
        case 1:
            swap(board[y1][x1], board[y2][x2]);
            cout << "success" << '\n';
            break;
        case 2:
            cout << "promotion" << '\n';
            int option;
            cin >> option;
            cout << "success" << '\n';
            break;
    }
}

void printBoard() {
    cout << '{' << '\n';
    for (auto &item: board) {
        cout << item << '\n';
    }
    cout << '}' << '\n';
}

void preview(int x1, int y1) {
    random_device dev;
    mt19937 gen(dev());
    cout << '{' << '\n';
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            cout << gen()%2;
        }
        cout<<'\n';
    }
    cout << '}' << '\n';
}

void printGameState() {
    random_device dev;
    mt19937 gen(dev());
    int rand = gen() % 5;
    switch (rand) {
        case 0:
            cout << "white win" << '\n';
            break;
        case 1:
            cout << "black win" << '\n';
            break;
        case 2:
            cout << "white turn" << '\n';
            break;
        case 3:
            cout << "black turn" << '\n';
            break;
        case 4:
            cout << "tie" << '\n';
            break;
    }
}



int main() {
    string command;
    while(cin >> command) {

        if(command == "move") {
            int x1,y1,x2,y2;
            cin >> x1 >> y1 >> x2 >> y2 ;
            move(x1,y1,x2,y2);
        }
        else if(command == "preview") {
            int x1,y1;
            cin >> x1 >> y1 ;
            preview(x1,y1);
        }
        else if(command == "print") {
            string type;
            cin >> type;
            cout << "command:" << command << type << '\n';
            if(type == "board") {
                printBoard();
            }
            else if (type == "gameState") printGameState();
            else cout << "invalid print type" << '\n';
        }
        else {
            cout << "invalid" << '\n';
        }
    }
    cout << "ended" << '\n';
}
