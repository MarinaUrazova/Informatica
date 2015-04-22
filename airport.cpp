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
	size_t time;
	size_t type;
	event(size_t time_, size_t type_): time(time_), type(type_) {}
	bool operator<(const event&  other) const {
		return time < other.time;
	}
};
int pl = 0;
int handledpl = 0;
std::multiset<event> events;
pthread_mutex_t runway[2];
pthread_mutex_t print;
pthread_mutex_t eventsmtx;

void* plane(void *type_) {
        int type = *((int*) type_);
	bool started = false;
	int plane_num = pl++;
	int current_time;
	while (!started) {
 		pthread_mutex_lock(&eventsmtx);
		if (events.begin()->type < 2) {
			current_time = events.begin()->time;
			events.erase(events.begin());
			started = true;
		}
		pthread_mutex_unlock(&eventsmtx);
	}
	int done = 0;
	pthread_mutex_lock(&print);
	if (type)
		cout << plane_num << " is ready for arrival";
	else
		cout << plane_num << " is ready for departure";
	cout << " Time: " << current_time / 60 <<":" << current_time % 60 << endl;
	pthread_mutex_unlock(&print);
	while (!done) {
	pthread_mutex_lock(&eventsmtx);
	current_time = events.begin()->time;
		if (!pthread_mutex_trylock(&runway[0])) {
			events.insert(event(current_time+3,3));
			done = 1;
		}
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
	events.erase(events.begin());
	pthread_mutex_unlock(&eventsmtx);
	pthread_mutex_lock(&print);
	cout << plane_num;
	if (type)
		cout << " arrived. Time: " << current_time /60 << ":" << current_time % 60 << "railway #" << done << endl;
	else
        cout << " departured. Time: " << current_time /60 << ":" << current_time % 60 << "railway #" << done << endl;
	pthread_mutex_unlock(&runway[done - 1]);
	pthread_mutex_unlock(&print);
	handledpl++;
}

int main() {
	std::default_random_engine generator;
	std::uniform_int_distribution <size_t> distribution(0,59);
	size_t duration;
	std::vector <pthread_t> planes;
	std::cin >> duration;
	pthread_mutex_init(&runway[0], NULL);
	pthread_mutex_init(&runway[1], NULL);
	pthread_mutex_init(&print, NULL);
	pthread_mutex_init(&eventsmtx, NULL);
	for (size_t hour = 0; hour < duration; hour++) {
		for (size_t i = 0; i < 5; ++i) {
			events.insert(event(distribution(generator) + 60*hour, 0));
			events.insert(event(distribution(generator) + 60*hour, 1));
		}
	}

	int a[2];
	a[0] = 0;
	a[1] = 1;
	while (!events.empty()) {
		planes.push_back(pthread_t());
		pthread_mutex_lock(&eventsmtx);
		if (events.begin()->type < 2) {
			pthread_create(&planes.back(),NULL, plane, &(a[events.begin()->type]));
		}
		pthread_mutex_unlock(&eventsmtx);
	}
	while (handledpl < duration * 10) {}
	pthread_mutex_destroy(&runway[0]);
	pthread_mutex_destroy(&runway[1]);
	pthread_mutex_destroy(&print);
	pthread_mutex_destroy(&eventsmtx);
}


