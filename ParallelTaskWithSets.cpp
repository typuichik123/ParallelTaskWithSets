#include "find_intersection.h"
#include <iostream>
#include <time.h> 
#include <fstream>



int main(int argc, char** argv)
{	
	
	setlocale(LC_ALL, "rus");
	std::cout << "ввод множеств с консоли: 0\nмножества, заданные в программе: 1 " << std::endl;
	int mod = 0;
	std::cin>> mod;
	std::vector<int> va;
	std::vector<int> vb;
	std::vector<int> vc;
	if (mod == 0) {
		int numer;
		std::cout << "input the numers of vecros in line, -1 is flag of end the vector" << std::endl;
		while (std::cin >> numer)
		{
			if (numer == -1) break;
			va.push_back(numer);
		}
		while (std::cin >> numer)
		{
			if (numer == -1) break;
			vb.push_back(numer);
		}
		while (std::cin >> numer)
		{
			if (numer == -1) break;
			vc.push_back(numer);
		}
	}
	else {
		va.resize(20000);
		vb.resize(20000);
		vc.resize(20000);
		generate(va.begin(), va.end(), rand);
		generate(vb.begin(), vb.end(), rand);
		generate(vc.begin(), vc.end(), rand);
		sort(va.begin(), va.end());
		sort(vb.begin(), vb.end());
		sort(vc.begin(), vc.end());
		va.erase(std::unique(va.begin(), va.end()), va.end());
		vb.erase(std::unique(vb.begin(), vb.end()), vb.end());
		vc.erase(std::unique(vc.begin(), vc.end()), vc.end());
	}
	double sumSimple = 0;
	double sumPAr = 0;
	 int repeat = 5 ;
	 int numThreads = 8;
	 clock_t start;
	 clock_t end;
	 std::ofstream out("statisticlast20k.txt", std::ios::app);
	 for (int tr = 1; tr <= numThreads; tr ++) {
		 cout << "\nКоличество потоков: " << tr << std::endl;
		 out << "\nКоличество потоков: " << tr << std::endl;
		 for (int j = 0; j < repeat; j++) {
			 if (tr == numThreads) {
				  start = clock();
				 std::vector<int> vec1 = simpleIntersection(va, vb, vc);
				  end = clock();
				 //std::cout << "Последовательное решение: ";
				 /*for (int q : vec1) {
					 std::cout << q << " ";
				 }*/
				 std::cout << "\n Последовательное - время в секундах: " << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
				 sumSimple += (double)(end - start);
				 
			 }
			start = clock();
			std::vector<int> vec2(0);
				find_interseption_pool(va, vb, vc, vec2, tr);
			
			 end = clock();
			 double currentParallelTime = (double)(end - start);
			 out << currentParallelTime / CLOCKS_PER_SEC << std::endl;
			 sumPAr += currentParallelTime;
			 std::cout << "Параллельное решение: ";
			 /*for (int q : vec2) {
				 std::cout << q << " ";
			 }*/
			 std::cout << "\nВремя в секундах: " << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
		 } 
		 out << "Параллельное решение: ";
		 out << "\nСреднее время в секундах: " << (sumPAr / CLOCKS_PER_SEC) / repeat << std::endl;
		 if (tr == numThreads) {
			 std::cout << "Последовательное решение: ";
			 std::cout << "\nСреднее время в секундах: " << (sumSimple / CLOCKS_PER_SEC) / repeat << std::endl;
			 out << "Последовательное решение: ";
			 out << "\nСреднее время в секундах: " << (sumSimple / CLOCKS_PER_SEC) / repeat << std::endl;
		 }
		 std::cout << "Параллельное решение : ";
		 std::cout << "\nСреднее время в секундах: " << (sumPAr / CLOCKS_PER_SEC) / repeat << std::endl;
		 sumSimple = 0;
		 sumPAr = 0;
	 }
	 out.close();

}


