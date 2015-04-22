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
            if ((data[i] != NULL) && (data[i+1] != NULL)) {
                if ((cmp_func(new_element, data[i]) == 0) && (cmp_func(new_element, data[i+1]) == 1)) n = i + 1;
            }
        }

		for (int j = size_data - 1; j >= n; j--) {
            data[j+1] = data[j];
        }
		data[n] = new_element;

		size_data++;
    }

    int remove( char* element ) {
        int k = 0;

		for (int i = 0; i < size_data; i++) {
            if(data[i] == element) { k = i;
                break;
            }
            else k = -1;
        }

        if (k == -1) cout << "Невозможно удалить элемент " << element << endl;
        else { cout << "Элемент " << element << " успешно удалён" << endl;

        for (int j = k; j < size_data - 1; j++)
            data[j] = data[j+1];
        }
        size_data--;
    }

    int search( char* element ) {
        int k = 0;

		for (int i = 0; i < size_data; i++) {
            if(data[i] == element) { k = i;
                cout << "Номер элемента " << element << " : " << k << endl;
                break;
            }
            else k = -1;
        }

        if (k == -1) cout << "Элемент " << element << " не найден" << endl;
    }

	void print() {
        for (int i = 0; i < size_data; i++) {
            if (data[i] != NULL)
                cout << data[i] << endl; }
    }
};
