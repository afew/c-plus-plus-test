#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
using namespace std::chrono;

//std::mutex					*((std::mutex*)syn_m);
//std::condition_variable		syn_d;
void* 					syn_m;
void*					syn_d;

std::string					g_data;
bool						ready		= false;
bool						processed	= false;

void worker_thread()
{
	// Wait until main() sends data
	std::unique_lock<std::mutex> lk(*((std::mutex*)syn_m));
	((std::condition_variable*)syn_d)->wait(lk, []{return true;});

	// after the wait, we own the lock.
	std::cout << "Worker thread is processing data\n";
	g_data += " after processing";

	// Send data back to main()
	processed = true;
	std::cout << "Worker thread signals data processing completed\n";

	// Manual unlocking is done before notifying, to avoid waking up
	// the waiting thread only to block again (see notify_one for details)
	lk.unlock();
	((std::condition_variable*)syn_d)->notify_one();
}

int main()
{
	auto waitTime = std::chrono::milliseconds::max();
	long long count = waitTime.count();

	int wait_int=10;
	waitTime = 5ms;
	waitTime = std::chrono::milliseconds{wait_int};

	//std::mutex					*((std::mutex*)syn_m);
	//std::condition_variable		syn_d;
	syn_m = new std::mutex;
	syn_d = new std::condition_variable;


	std::thread worker(worker_thread);
	std::this_thread::sleep_for(std::chrono::seconds(5));

	g_data = "Example data";
	// send data to the worker thread
	{
		std::lock_guard<std::mutex> lk(*((std::mutex*)syn_m));
		//ready = true;
		std::cout << "main() signals data ready for processing\n";
	}
	((std::condition_variable*)syn_d)->notify_one();

	// wait for the worker
	{
		std::unique_lock<std::mutex> lk(*((std::mutex*)syn_m));
		((std::condition_variable*)syn_d)->wait_for(lk, 0ms);
	}
	std::cout << "Back in main(), data = " << g_data << '\n';


	// wait for the worker
	{
		std::unique_lock<std::mutex> lk(*((std::mutex*)syn_m));
		((std::condition_variable*)syn_d)->wait_for(lk, 0ms);
	}
	std::cout << "Back in main(), data = " << g_data << '\n';

	worker.join();


	delete ((std::mutex*)syn_m);
	delete ((std::condition_variable*)syn_d);
}