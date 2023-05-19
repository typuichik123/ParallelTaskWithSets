#pragma once
#include <vector>
#include <algorithm>
#include <thread> 
#include <mutex>
#include <mpi.h>
#include <set>
#include <random>
using namespace std;
std::vector<int> find_intersection(std::vector<int>& va, std::vector<int>& vb, std::vector<int>& vc);
std::vector<int> simpleIntersection(std::vector<int>& a, std::vector<int>& b, std::vector<int>& c);
std::vector<int> intersectionMPI(std::vector<int>& va, std::vector<int>& vb, std::vector<int>& vc);
std::vector<int> intersection_worker(const std::vector<int>& v1, const std::vector<int>& v2, const std::vector<int>& v3);
void find_interseptionForParallelTask(std::vector<int>& result, std::vector<int>& vb, std::vector<int>& vc, int value);
void find_interseption_pool(std::vector<int>& va, std::vector<int>& vb, std::vector<int>& vc, std::vector<int>& result, int threadCount);
std::vector<int> generate_unique_sorted_values(int n, int lower_bound, int upper_bound);