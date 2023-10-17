#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

//#include <chrono>

//using namespace std;
std::mutex m;
std::condition_variable cv;
bool ready = false;


void count() {
	m.lock();//блокировка мьютекса
	ready = false;
	//выполнение дейсьвий, необходимых для наступления события
	for (int i = 0; i < 3; i++) 
		std::cout << "count\n";
	cv.notify_one();//уведомление потока-потребителя о наступившем событии
	ready = true;
	m.unlock();//освобождение мбютекса

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int main() {
	std::thread first(count);//поток-поставщик
	std::unique_lock<std::mutex> lock1(m);//блокировка мьютекса при создании

	//проверка наступления события
	while (!ready) 
		cv.wait(lock1);//ожидание и временное освобождение мьютекса
	//обработка
	for (int i = 0; i < 300000; i++) {}
	std::cout << "main\n";

	lock1.unlock();//окончательное освобождение мьютекса
	
	first.join();

	return 0;
}