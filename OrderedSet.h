#include <iostream>
#include <cstdlib>
#include "string.h"
using namespace std;

class OrderedSet
{
public:

    char** data;
    int size_data;
    int (*cmp_func)(char* a1, char* a2);

    OrderedSet( int size_data, int (*cmp_func)(char* a1, char* a2)) {
        this -> size_data = size_data;
        this -> cmp_func = cmp_func;
        data = (char**)calloc(sizeof(char*), size_data);
    }


    ~OrderedSet() {
        free(data);
    }

	int add( char* new_element, int (*cmp_func)(char* el1, char* el2) ) {
		int n = 0;
		this -> size_data = size_data;

		for (int i = 0; i < size_data; i++)
			while (cmp_func(new_element, data[i]) > 0)
				n = i + 1;

 		char** new_data = (char**)calloc(sizeof(char*), (size_data + 1));

		for (int j = 0; j < n; j++)
			new_data[j] = data[j];

		new_data[n] = new_element;

		for (int j = n; j < (size_data + 1); j++)
			new_data[j] = data[j-1];
    }

    int remove( char* element, int (*cmp_func)(char* el1, char* el2) ) {
		this -> size_data = size_data;
        int k=0;
		for (int i = 0; i < size_data; i++) {
            if(data[i] == element) k = i;
            else k = -1;
        }
        if (k == -1) cout << 'Error';
        else    for (int j = k; j < size_data; j++)
                    data[k] = data[k+1];
    }

    int search( char* element) {
		this -> size_data = size_data;
        int k=0;
		for (int i = 0; i < size_data; i++) {
            if(data[i] == element) {
                k = i;
                cout << k;
            }
            else cout << 'Error';
        }
	}

};
