#include "find_intersection.h"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <algorithm>
#include <iterator>
#include <boost/thread.hpp>
#include <boost/asio/defer.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/range/algorithm/set_algorithm.hpp>
mutex mtx;
std::chrono::nanoseconds timespan(1);
void find_interseptionForParallelTask( std::vector<int> &result, std::vector<int> &vb, std::vector<int> &vc,int value) {
    
    if (std::binary_search(vb.begin(), vb.end(), value) &&
        std::binary_search(vc.begin(), vc.end(), value)) {
        mtx.lock();
        result.push_back(value);
        mtx.unlock();
        this_thread::sleep_for(timespan);
    }

}

void find_interseption_pool(std::vector<int>& va, std::vector<int>& vb, std::vector<int>& vc, std::vector<int> &result, int threadCount ) {
    if (threadCount > boost::thread::hardware_concurrency()) {
        std::printf("Max thread count is %d , your choice is %d", boost::thread::hardware_concurrency(), threadCount);
        exit(-1);
    }
    boost::asio::thread_pool pool(threadCount);
 

    size_t size = va.size();
    for (size_t i = 0; i < size; i++)
    {   
        boost::asio::post(pool, boost::bind(find_interseptionForParallelTask, boost::ref(result), boost::ref(vb), boost::ref(vc), va[i]));
    }

    pool.join();
    sort(result.begin(), result.end());
}



std::vector<int> find_intersection(std::vector<int>& va, std::vector<int>& vb, std::vector<int>& vc) {
	std::vector<int> intersecAB;
	std::set_intersection(va.begin(), va.end(), vb.begin(), vb.end(), std::back_inserter(intersecAB));
	std::vector<int> intersecABC;
	std::set_intersection(intersecAB.begin(), intersecAB.end(), vc.begin(), vc.end(), std::back_inserter(intersecABC));
    return intersecABC;
}

std::vector<int> simpleIntersection(std::vector<int> & a, std::vector<int> & b, std::vector<int> & c) {
    std::vector<int> intersecABC;
    for (int i = 0; i < a.size(); i++) {
        int x = a[i];
        this_thread::sleep_for(timespan);
        if (std::binary_search(b.begin(), b.end(), x) &&
            std::binary_search(c.begin(), c.end(), x)) {
            intersecABC.push_back(x);
        }
    }
    sort(intersecABC.begin(), intersecABC.end());
    return intersecABC;
}

std::vector<int> intersectionMPI(std::vector<int> &va, std::vector<int> & vb, std::vector<int>  & vc)
{
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Получаем количество процессов
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Получаем номер текущего процесса

    std::vector<int> res; // Итоговое пересечение

    // Рекурсивно делим данные между процессами
    int n = va.size();
    int offset = rank;
    while (offset < n) {
        int val = va[offset];

        int idxb = std::lower_bound(vb.begin(), vb.end(), val) - vb.begin();
        int idxc = std::lower_bound(vc.begin(), vc.end(), val) - vc.begin();
        this_thread::sleep_for(timespan);
        if ((idxb < vb.size() && vb[idxb] == val) && (idxc < vc.size() && vc[idxc] == val)) {
            res.push_back(val);
        }
        offset += size;
    }

    // Сбор результатов
    std::vector<int> tmpres;
    int* counts = new int[size];
    int* offsets = new int[size];
    int ressize = res.size();
    MPI_Allgather(&ressize, 1, MPI_INT, counts, 1, MPI_INT, MPI_COMM_WORLD);
    int total = 0;
    for (int i = 0; i < size; i++) {
        offsets[i] = total;
        total += counts[i];
    }
    tmpres.resize(total);
    MPI_Allgatherv(&res[0], ressize, MPI_INT, &tmpres[0], counts, offsets, MPI_INT, MPI_COMM_WORLD);

    // Возвращаем результат
    return tmpres;
}

    std::vector<int> intersection_worker(const std::vector<int>& v1, const std::vector<int>& v2, const std::vector<int>& v3) {
        std::vector<int> temp_result;
        std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(temp_result));

        std::vector<int> result;
        std::set_intersection(temp_result.begin(), temp_result.end(), v3.begin(), v3.end(), std::back_inserter(result));

        return result;
    }

    std::vector<int> generate_unique_sorted_values(int n, int lower_bound, int upper_bound) {
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dist(lower_bound, upper_bound);
        std::set<int> unique_values;

        while (static_cast<int>(unique_values.size()) < n) {
            unique_values.insert(dist(gen));
        }

        std::vector<int> sorted_values(unique_values.begin(), unique_values.end());
        return sorted_values;
    }


    