#include <iostream>
#include <thread>
#include <time.h>
#include <mutex>
#include <stdlib.h>
#include <condition_variable>
using namespace std;
int num;
mutex mtx;
bool ready=false;
bool readyCons = false;
bool readyProd = false;
condition_variable cv;
void consumer()
{
	unique_lock <mutex> ul(mtx);
	int c = 0;
	while (c < 10)
	{
		cv.wait(ul, [] {return (ready == true) ? true : false; });
		cout << "awoke..." << endl;
		cout << "consumed: " << num << endl<<endl;
		ready = false;
		cv.notify_one();
		c++;
	}
}

void provider()
{
	srand(time(NULL));
	int c = 0;
	unique_lock <mutex> ul(mtx);
	while (c < 10)
	{
		num = rand();
		cout << "provide: " << num << endl;
		ready = true;
		cv.notify_one();
		cv.wait(ul, [] {return (ready == false) ? true : false; });
		c++;
	}
}

int main()
{
	thread prov(provider);
	thread cons(consumer);

	prov.join();
	cons.join();
	return 0;
}