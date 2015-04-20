#include <iostream>
#include <cstring>
#include "OrderedSet.h"
using namespace std;

int cmp(char* a1, char* a2) {
    if (strlen(a1) <= strlen(a2)) return 0;
    else return 1;
}

char* makestring() {
    int n = rand()%10;
    char array[26] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm'};
    char* str = (char*)calloc(sizeof(char), n);
    for(int i=0; i < n; i++)
        str[i] = array[rand()%26];
    return str;
    }

int main(int argc, const char* argv[]) {
    srand(time(NULL));

    int size_data;
    cout << 'Enter number of elements in ordered set.' << endl;
    cin >> size_data;

    OrderedSet *MyOrderedSet = new OrderedSet(size_data, &cmp);

    for (int i=0; i<size_data; i++) {
        char* tmp;
        tmp = makestring();
        cout << tmp << endl;
        MyOrderedSet -> add(tmp);
    }

    /*MyOrderedSet -> add("str");*/
}
