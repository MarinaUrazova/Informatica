#include <iostream>
#include <cstdlib>
#include <cstring>
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

	int add( char* new_element )  {
		int n = 0;

		for (int i = 0; i < size_data; i++) {
            if (data[i] != NULL)
                while (cmp_func(new_element, data[i]) < 1)
                    n = i + 1;
            else n = i;
        }

		for (int j = size_data - 1; j > n; j--) {
            data[j+1] = data[j];
        }
		data[n] = new_element;

    }

    int remove( char* element ) {
        int k=0;
		for (int i = 0; i < size_data; i++) {
            if(data[i] == element) k = i;
            else k = -1;
        }
        if (k == -1) cout << 'Error';
        else    for (int j = k; j < size_data; j++)
                    data[k] = data[k+1];
    }

    int search( char* element ) {
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
