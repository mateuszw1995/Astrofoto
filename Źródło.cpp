#include <thread>
#include <mutex>
#include <iostream>
#include <future>
using namespace std;

mutex m;
future<void> f;

bool new_work = false;
function<void()> shared_task;
promise <void> shared_promise;

void work()
{
	try {
		for(;;) {
			unique_lock<mutex> ul(m);
			if(new_work) {
				new_work = false;

				auto copy_task = shared_task;
				auto copy_promise = move(shared_promise);

				ul.unlock();
				copy_task();
				copy_promise.set_value();
			}
		}
	}
	catch(int) {

	}
	return;
}

void zlec(const function<void()>& task)
{
	f.get();

	unique_lock<mutex> ul(m);
	shared_task = task;
	shared_promise = promise<void>();
	f = shared_promise.get_future();

	new_work = true;
}

int main(){
	thread worker(work);
	f = shared_promise.get_future();
	shared_promise.set_value();

	for(int i = 0; i < 10; ++i) {
		zlec([i](){ cout << 2*i << endl; });

		cout << 2*i+1 << endl;
	}

	zlec([](){throw 0;});

	worker.join();
	return 0;
}