#include <iostream>
#include <cstring>
#include "OrderedSet.h"
using namespace std;

int cmp_func(char* a1, char* a2) {
    if (strlen(a1) < strlen(a2)) return -1;
    if (strlen(a1) == strlen(a2)) return 0;
    if (strlen(a1) > strlen(a2)) return 1;
}

int main(int argc, const char* argv[]) {
    int size_data;
    cout << 'Enter number of elements in ordered set.' << endl;
    cin >> size_data;

    OrderedSet *MyOrderedSet = new OrderedSet(size_data, *cmp_func);

    /*int i=0;
    for (int i=0; i<size_data; i++) {*/
        srand(time(NULL));
        int n = rand()%10;
        char array[26] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm'};
        char* str;
        str = (char*)calloc(sizeof(char), n);
        for(int j=0; j < n; j++)
            str[j] = array[rand()%26];
        add(str);

}
