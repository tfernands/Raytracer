#include <iostream>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

using namespace std::placeholders;

class Application{

	std::mutex m_mutex;
	std::condition_variable m_condVar;
	bool m_bDataLoaded;

	public:
	Application(){
		m_bDataLoaded = false;
	}

	void loadData(){
		// Make This Thread sleep for 1 Second
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout<<"Loading Data from XML"<<std::endl;
		// Lock The Data structure
		std::lock_guard<std::mutex> guard(m_mutex);
		// Set the flag to true, means data is loaded
		m_bDataLoaded = true;
		// Notify the condition variable
		m_condVar.notify_one();
	}

	bool isDataLoaded(){
		return m_bDataLoaded;
	}

	void mainTask(){
		std::cout<<"Do Some Handshaking"<<std::endl;
		// Acquire the lock
		std::unique_lock<std::mutex> mlock(m_mutex);
		// Start waiting for the Condition Variable to get signaled
		// Wait() will internally release the lock and make the thread to block
		// As soon as condition variable get signaled, resume the thread and
		// again acquire the lock. Then check if condition is met or not
		// If condition is met then continue else again go in wait.
		m_condVar.wait(mlock, std::bind(&Application::isDataLoaded, this));
		std::cout<<"Do Processing On loaded Data"<<std::endl;
	}
};

void fillData(int data[], int start, int end, int d){
	for (int i = start; i < end; i++) data[i] = d;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000*d));
}

void printData(int data[], int size){
	for (int i = 0; i < size; i++) std::cout << data[i] << ',';
	std::cout << std::endl;
}

std::mutex m_mutex;

void count(int &a, int to){
	for (int i = 0; i < to; i++){
		m_mutex.lock();
		a = a+1;
		m_mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

int main(){

	int a = 0;
	std::thread t1(count, std::ref(a), 1000);
	std::thread t2(count, std::ref(a), 1000);
	std::thread t3(count, std::ref(a), 1000);
	t1.join();
	t2.join();
	t3.join();
	std::cout << a << std::endl;
	/*
	const int data_size = 1000;
	int *data = new int[data_size];

	const int thread_count = 4;
	std::thread* threads[thread_count];
	for (int i = 0; i < thread_count; i++){
		int start = (data_size/thread_count)*i;
		int end = (data_size/thread_count)*(i+1);
		threads[i] = new std::thread(fillData, data, start, end, i);
	}

	for (int i = 0; i < thread_count; i++){
		threads[i]->join();
		delete threads[i];
	}

	printData(data, data_size);
	*/
	//Application app;
	//std::thread thread_1(&Application::mainTask, &app);
	//std::thread thread_2(&Application::loadData, &app);
	//thread_2.join();
	//thread_1.join();

	return 0;
}