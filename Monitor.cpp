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
	m.lock();//���������� ��������
	ready = false;
	//���������� ��������, ����������� ��� ����������� �������
	for (int i = 0; i < 3; i++) 
		std::cout << "count\n";
	cv.notify_one();//����������� ������-����������� � ����������� �������
	ready = true;
	m.unlock();//������������ ��������

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int main() {
	std::thread first(count);//�����-���������
	std::unique_lock<std::mutex> lock1(m);//���������� �������� ��� ��������

	//�������� ����������� �������
	while (!ready) 
		cv.wait(lock1);//�������� � ��������� ������������ ��������
	//���������
	for (int i = 0; i < 300000; i++) {}
	std::cout << "main\n";

	lock1.unlock();//������������� ������������ ��������
	
	first.join();

	return 0;
}