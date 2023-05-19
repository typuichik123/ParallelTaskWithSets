
#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include<thread>
using namespace std;
void sort_for_one_thread(std::vector<int>& unsortedVector) {
    sort(unsortedVector.begin(), unsortedVector.end());
}


std::vector<int> intersectionMPI(std::vector<int> va, std::vector<int> vb, std::vector<int> vc)
{
    std::thread thread1(sort_for_one_thread, std::ref(va));
    std::thread thread2(sort_for_one_thread, std::ref(vb));
    std::thread thread3(sort_for_one_thread, std::ref(vc));
    thread1.join();
    thread2.join();
    thread3.join();
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

int main(int argc, char** argv)
{
   /* std::vector<int> vec1(30000000);
    std::vector<int> vec2(30000000);
    std::vector<int> vec3(30000000);
    generate(vec1.begin(), vec1.end(), rand);
    generate(vec2.begin(), vec2.end(), rand);
    generate(vec3.begin(), vec3.end(), rand);*/
    std::vector<int> vec1(200000000);
    std::vector<int> vec2(200000000);
    std::vector<int> vec3(200000000);
    for (int i = 200000000 - 1; i > 0; i--) {
        vec1[i] = i;
    }
    for (int i = 300000000 - 1; i > 100000000; i--) {
        vec2[i - 100000000] = i;
    }
    for (int i = 400000000 - 1; i > 200000000; i--) {
        vec3[i - 200000000] = i;
    }
    MPI_Init(&argc, &argv);
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    clock_t start;
    clock_t end;
    for (int i = 0; i < 1; i++) {
        start = clock();
        vector<int> result = intersectionMPI(vec1, vec2, vec3);
        end = clock();
        if (rank == 0) {
            /*cout << "Intersection: ";
            for (int i = 0; i < result.size(); i++) {
                cout << result[i] << " ";
            }
            cout << endl;*/
            std::cout << "\nTotal time in seconds: " << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
        }
    }
    MPI_Finalize();
    return 0;
}
