#include "pch.h"
#include "CppUnitTest.h"
#include "..\find_intersection.h"
#include <mpi.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{

	TEST_CLASS(UnitTests)
	{
	public:
		TEST_METHOD(TestMethodSync1)
		{
			std::vector<int> vec1 = { 1, 2, 3 };
			std::vector<int> vec2 = { 3, 4, 5 };
			std::vector<int> vec3 = { 3, 4, 5 };
			std::vector<int> result = simpleIntersection(vec1, vec2, vec3);
			Assert::IsTrue(result.size() == 1);
			std::vector<int> true_result = {3};
			for (int i = 0; i < true_result.size(); i++) {
			Assert::AreEqual(true_result[i], result[i]);
			}
		}
		TEST_METHOD(TestMethodSync3)
		{
			std::vector<int> vec1 = {};
			std::vector<int> vec2 = {};
			std::vector<int> vec3 = { };
			std::vector<int> result = simpleIntersection(vec1, vec2, vec3);
			Assert::IsTrue(result.size() == 0);
			std::vector<int> true_result = {};
			for (int i = 0; i < true_result.size(); i++) {
				Assert::AreEqual(true_result[i], result[i]);
			}
		}

		TEST_METHOD(TestMethodWithThreads1)
		{
			std::vector<int> result(0);
			std::vector<int> vec1 = { 1, 2, 3 };
			std::vector<int> vec2 =  { 3, 4, 5 };
			std::vector<int> vec3 = { 3, 4, 5 };
				find_interseption_pool(vec1, vec2, vec3, result, 1);
			Assert::IsTrue(result.size() == 1);
			std::vector<int> true_result = { 3 };
			for (int i = 0; i < true_result.size(); i++) {
				Assert::AreEqual(true_result[i], result[i]);
			}
		}
		TEST_METHOD(TestMethodWithThreads3)
		{
			std::vector<int> result(0);
			std::vector<int> vec1 = {};
			std::vector<int> vec2 = {};
			std::vector<int> vec3 = { };
			find_interseption_pool(vec1, vec2, vec3, result, 3);
			Assert::IsTrue(result.size() == 0);
			std::vector<int> true_result = {};
			for (int i = 0; i < true_result.size(); i++) {
				Assert::AreEqual(true_result[i], result[i]);
			}
		}
		TEST_METHOD(TestMethodWithThreads4)
		{
			std::vector<int> result(0);
			std::vector<int> vec1 = { 1, 2, 3, 7, 18 };
			std::vector<int> vec2 = { 1, 3, 4, 5, 8, 9 };
			std::vector<int> vec3 = { 1, 6, 12, 90 };
			find_interseption_pool(vec1, vec2, vec3, result, 4);
			Assert::IsTrue(result.size() == 1);
			std::vector<int> true_result = {1};
			for (int i = 0; i < true_result.size(); i++) {
				Assert::AreEqual(true_result[i], result[i]);
			}
		}
		
		TEST_METHOD(TestMethodSyncAndThread)
		{
			std::vector<int> vec1(20000);
			std::vector<int> vec2(20000);
			std::vector<int> vec3(20000);
			generate(vec1.begin(), vec1.end(), rand);
			generate(vec2.begin(), vec2.end(), rand);
			generate(vec3.begin(), vec3.end(), rand);
			sort(vec1.begin(), vec1.end());
			sort(vec2.begin(), vec2.end());
			sort(vec3.begin(), vec3.end());
			vec1.erase(std::unique(vec1.begin(), vec1.end()), vec1.end());
			vec2.erase(std::unique(vec2.begin(), vec2.end()), vec2.end());
			vec3.erase(std::unique(vec3.begin(), vec3.end()), vec3.end());
			std::vector<int> result = simpleIntersection(vec1, vec2, vec3);
			std::vector<int> result1(0);
			find_interseption_pool(vec1, vec2, vec3, result1, 4);
			Assert::AreEqual(result.size(), result1.size());
			for (int i = 0; i < result.size(); i++) {
				Assert::AreEqual(result[i], result1[i]);
			}
		}
		TEST_METHOD(TestMethodWithThreadsRandomAndMPI)
		{
			std::vector<int> vec1(20000);
			std::vector<int> vec2(20000);
			std::vector<int> vec3(20000);
			generate(vec1.begin(), vec1.end(), rand);
			generate(vec2.begin(), vec2.end(), rand);
			generate(vec3.begin(), vec3.end(), rand);
			sort(vec1.begin(), vec1.end());
			sort(vec2.begin(), vec2.end());
			sort(vec3.begin(), vec3.end());
			vec1.erase(std::unique(vec1.begin(), vec1.end()), vec1.end());
			vec2.erase(std::unique(vec2.begin(), vec2.end()), vec2.end());
			vec3.erase(std::unique(vec3.begin(), vec3.end()), vec3.end());
			MPI_Init(nullptr, nullptr);
			int rank, num_procs;
			std::vector<int> res1;
			MPI_Comm_rank(MPI_COMM_WORLD, &rank);
			MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
			if (rank == 0) {
				std::vector<int> res = intersectionMPI(vec1, vec2, vec3);
			for (int i = 0; i < res.size(); i++) {
				res1.push_back(res[i]);
			}
			}
			MPI_Finalize();
			std::vector<int> result(0);
			find_interseption_pool(vec1, vec2, vec3, result, 8);
			Assert::IsTrue(result.size() == res1.size());
			for (int i = 0; i < result.size(); i++) {
				Assert::AreEqual(result[i], res1[i]);
			}
		}
		TEST_METHOD(TestMethodMPI)
		{
			std::vector<int> vec1(20000);
			std::vector<int> vec2(20000);
			std::vector<int> vec3(20000);
			generate(vec1.begin(), vec1.end(), rand);
			generate(vec2.begin(), vec2.end(), rand);
			generate(vec3.begin(), vec3.end(), rand);
			sort(vec1.begin(), vec1.end());
			sort(vec2.begin(), vec2.end());
			sort(vec3.begin(), vec3.end());
			vec1.erase(std::unique(vec1.begin(), vec1.end()), vec1.end());
			vec2.erase(std::unique(vec2.begin(), vec2.end()), vec2.end());
			vec3.erase(std::unique(vec3.begin(), vec3.end()), vec3.end());
			MPI_Init(nullptr, nullptr);
			int rank, num_procs;
			std::vector<int> res1;
			MPI_Comm_rank(MPI_COMM_WORLD, &rank);
			MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
			if (rank == 0) {
				std::vector<int> res = intersectionMPI(vec1, vec2, vec3);
				for (int i = 0; i < res.size(); i++) {
					res1.push_back(res[i]);
				}
			}
			MPI_Finalize();
			Assert::IsTrue(res1.size() != 0);
		}
	};
}
