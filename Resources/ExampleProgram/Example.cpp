//Note: This program is just an example. It does not have any practical use and is only for demonstration purposes.
#include <iostream>
#include <vector>

using namespace std;

vector<int> sortList;
void parseIn() {
    for (int i = 0; i < 10; i++) {
        int x = 0;
        cin >> x;

        sortList.push_back(x);
    }
}

bool compare(int &a, int &b) {
    return a > b;
}

void core() {
    sort(sortList.begin(), sortList.end(), compare);
}

void writeOut() {
    for (int i = 0; i < 10; i++) {
        cout << sortList.at(i);
    }
    cout << endl;
}

int main () {
    parseIn();
    core();
    writeOut();

    return 0;
}