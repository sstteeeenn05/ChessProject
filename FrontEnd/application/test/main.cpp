#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>

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

string printGameState() {
    random_device dev;
    mt19937 gen(dev());
    int rand = gen() % 5;
    string output;
    switch (rand) {
    case 0:
        output = "win;white;";
        break;
    case 1:
        output = "win;black;";
        break;
    case 2:
        output = "turn;white;";
        break;
    case 3:
        output = "turn;black;";
        break;
    case 4:
        output = "tie;;";
        break;
    }
    return output + to_string(gen() % 2) + ";" + to_string(gen() % 2) + ";";
}

void move(int x1, int y1, int x2, int y2) {
    string output(printGameState());
    random_device dev;
    mt19937 gen(dev());
    int rand = gen() % 3;
    switch (rand) {
        case 0:
            cout << printGameState() + "failed" << '\n';
            break;
        case 1:
            swap(board[y1][x1], board[y2][x2]);
            cout << printGameState() + "success" << '\n';
            break;
        case 2:
            cout << printGameState() + "promotion" << '\n';
            int option;
            cin >> option;
            cout << printGameState() + "success" << '\n';
            break;
    }
}

void printBoard() {
    string output(printGameState());
    for (auto &item: board) {
        output += item;
    }
    cout << output << endl;
}

void preview(int x1, int y1) {
    string output(printGameState());
    random_device dev;
    mt19937 gen(dev());
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            output+=gen()%2?'1':'0';
        }
    }
    cout << output << endl;
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
            printBoard();
        }
        else {
            cout << "invalid" << '\n';
        }
    }
    cout << "ended" << '\n';
}
