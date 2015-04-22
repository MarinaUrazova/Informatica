#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
#include <set>
#include <random>
#include <vector>
using namespace std;

class event { 
public:
	size_t time; // время;
	size_t type; // статус самолёта (сначала принимает значения 0 или 1 - появление самолета на отправление/прибытие);
	event(size_t time_, size_t type_): time(time_), type(type_) {} // событие - функция от времени и статуса самолёта;
	bool operator<(const event&  other) const {
		return time < other.time;
	} // перегруженный оператор <;
};
int pl = 0; // количество самолётов;
int handledpl = 0; // количество самолётов, прошедших обработку;
std:: multiset<event> events; // очередь событий (события не обязательно должны быть уникальными);
pthread_mutex_t runway[2]; 
pthread_mutex_t print;
pthread_mutex_t eventsmtx; // Мьютексы. Использование мьютексов позволяет множеству потоков упорядочить доступ к данным.

void* plane(void *type_) {
    int type = *((int*) type_); // преобразование типа;
	int plane_num = pl++; // номер самолёта;
	int current_time; // время;

	bool started = false;
	while (!started) { // проверяем, сопоставлено ли уже событие потоку;
 		pthread_mutex_lock(&eventsmtx); // берём право на доступ к очереди событий;
		if (events.begin()->type < 2) {
			current_time = events.begin()->time;
			events.erase(events.begin());
			started = true;
		} // если ближайшее событие со статусом 0 или 1, то фиксируем текущее время и удаляем событие из очереди событий, далее функция будет обрабатывать этот самолет;
		pthread_mutex_unlock(&eventsmtx); // отдаём право на доступ к очереди событий;
	}
	
	pthread_mutex_lock(&print); // берём право на доступ к печати;
	if (type)
		cout << plane_num << " is ready for arrival";
	else
		cout << plane_num << " is ready for departure";
	cout << " Time: " << current_time / 60 <<":" << current_time % 60 << endl;
	pthread_mutex_unlock(&print); // отдаём право на доступ к печати;
	// Далее аналогично (захват и освобождение мьютексов).
	int done = 0;
	while (!done) {
	pthread_mutex_lock(&eventsmtx);
	current_time = events.begin()->time;
		if (!pthread_mutex_trylock(&runway[0])) { 
			events.insert(event(current_time+3,3)); 
			done = 1;
		} // если мьютекс &runway[0] (первая взлётно-посадочная полоса) уже захвачен добавляем новое событие со сдвигом времени, переменной done присваиваем номер взлётно-посадочной полосы;
		if (!done && !pthread_mutex_trylock(&runway[1])) {
			events.insert(event(current_time+3,4));
			done = 2;
		}
	pthread_mutex_unlock(&eventsmtx);
	}

	pthread_mutex_lock(&print);
	if (type)
		cout << plane_num <<" started arriving. Time: " << current_time /60 << ":" << current_time % 60 << "railway #" << done << endl;
	else
		cout << plane_num << " started departuring. Time: " << current_time /60 << ":" << current_time % 60 << "railway #" << done << endl;

	pthread_mutex_unlock(&print);

	bool ready = false;
	while (!ready)
	{
		pthread_mutex_lock(&eventsmtx);
		ready = (events.begin()->type == done + 2);
		if (!ready)
		pthread_mutex_unlock(&eventsmtx);
	}
	current_time = events.begin()->time;
	events.erase(events.begin()); // удаляем событие
	pthread_mutex_unlock(&eventsmtx);

	pthread_mutex_lock(&print);
	cout << plane_num;
	if (type)
		cout << " arrived. Time: " << current_time /60 << ":" << current_time % 60 << "railway #" << done << endl;
	else
        cout << " departured. Time: " << current_time /60 << ":" << current_time % 60 << "railway #" << done << endl;
	pthread_mutex_unlock(&runway[done - 1]);
	pthread_mutex_unlock(&print);

	handledpl++; // увеличиваем количество обработанных самолётов;
}

int main() {
	std::default_random_engine generator; // генератор случайных чисел по умолчанию;
	std::uniform_int_distribution <size_t> distribution(0,59); // необходимо для равномерного распределения времени вылета и прилёта самолётов;
	size_t duration; // количество часов;
	std::vector <pthread_t> planes; // последовательность самолётов;
	std::cin >> duration;
	pthread_mutex_init(&runway[0], NULL);
	pthread_mutex_init(&runway[1], NULL);
	pthread_mutex_init(&print, NULL);
	pthread_mutex_init(&eventsmtx, NULL); // инициализация мьютексов;
	for (size_t hour = 0; hour < duration; hour++) {
		for (size_t i = 0; i < 5; ++i) {
			events.insert(event(distribution(generator) + 60*hour, 0));
			events.insert(event(distribution(generator) + 60*hour, 1)); // вставка событий с случайным временем и статусом (0, 1) в очередь событий для пяти самолётов, готовых взлететь и для пяти самолётов, готовых приземлиться в течении каждого часа.
		}
	}

	int a[2];
	a[0] = 0;
	a[1] = 1; // инициализация массива;
	while (!events.empty()) {
		planes.push_back(pthread_t()); // добавить новый самолёт в последовательность самолётов (новая нить);
		pthread_mutex_lock(&eventsmtx); 
		if (events.begin()->type < 2) {
			pthread_create(&planes.back(),NULL, plane, &(a[events.begin()->type])); // создание новой нити исполнения (на идентификатор нити исполнения указывает параметр &planes.back() - последний элемент последовательности самолётов,
																					//	нить исполнения выполняет функцию plane, передавая ей в качестве аргумента параметр &(a[events.begin()->type]) - статус самолёта самого первого события)
		}
		pthread_mutex_unlock(&eventsmtx);
	}
	while (handledpl < duration * 10) {} // проверка того, что все самолёты прошли обработку;
	pthread_mutex_destroy(&runway[0]);
	pthread_mutex_destroy(&runway[1]);
	pthread_mutex_destroy(&print);
	pthread_mutex_destroy(&eventsmtx); // разрушение мьютексов;
}


