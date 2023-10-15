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
condition_variable cv;
void consumer()
{
	unique_lock <mutex> ul(mtx);
	int c = 0;
	while (c < 10)
	{
		while (ready == false)
		{
			cv.wait(ul, [] {return (ready == true) ? true : false; });
			cout << "awoke..." << endl;
		}
		ready = false;
		cout << "consumed: " << num << endl<<endl;
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
		if (ready == true)
		{
			cv.wait(ul, [] {return (ready == false) ? true : false; });
		}
		ready = true;
		num = rand();
		cout << "provide: " << num << endl;
		cv.notify_one();
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