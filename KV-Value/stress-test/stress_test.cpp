#include <iostream>
#include <chrono>       //用于计时
#include <cstdlib>
#include <pthread.h>    //用于POSIX线程
#include <time.h>
#include <fstream>
#include <cstdint>
#include "../skiplist.h"
#define STORE_FILE_STRESS_TEST "test_result2"

#define NUM_THREADS 1      //线程数量
#define TEST_COUNT 1000    //测试次数
SkipList<int, std::string> skipList(18);

void *insertElement(void* threadid) {//插入元素的线程函数
    uintptr_t tid; 
    tid = reinterpret_cast<uintptr_t>(threadid);
    long tid_long = static_cast<long>(tid);  //线程id

    int tmp = TEST_COUNT / NUM_THREADS; //每个线程处理的操作数量
    for (int i = tid_long * tmp, count = 0; count < tmp; i++, count++) {//tid_long 是线程的ID，将其乘以每个线程要处理的操作数量，得到了该线程要处理的操作的起始索引
        skipList.insert_element(rand() % TEST_COUNT, "a"); 
    }
    pthread_exit(NULL);// 终止当前线程的执行，并返回 NULL
}

void *getElement(void* threadid) {//查找元素的线程函数
    uintptr_t tid; 
    tid = reinterpret_cast<uintptr_t>(threadid);
    long tid_long = static_cast<long>(tid);

    int tmp = TEST_COUNT / NUM_THREADS; 
    for (int i = tid_long * tmp, count = 0; count < tmp; i++, count++) {
        skipList.search_element(rand() % TEST_COUNT); 
    }
    pthread_exit(NULL);
}

void *deleteElement(void* threadid) {  //删除元素的线程函数
    uintptr_t tid; 
    tid = reinterpret_cast<uintptr_t>(threadid);
    long tid_long = static_cast<long>(tid);

    int tmp = TEST_COUNT / NUM_THREADS; 
    for (int i = tid_long * tmp, count = 0; count < tmp; i++, count++) {
        skipList.delete_element(rand() % TEST_COUNT); 
    }
    pthread_exit(NULL);
}

void *fixElement(void* threadid) { //更新元素的线程函数
    uintptr_t tid; 
    tid = reinterpret_cast<uintptr_t>(threadid);
    long tid_long = static_cast<long>(tid);

    int tmp = TEST_COUNT / NUM_THREADS; 
    for (int i = tid_long * tmp, count = 0; count < tmp; i++, count++) {
        skipList.update_element(rand() % TEST_COUNT, "b", true); 
    }
    pthread_exit(NULL);
}


int main() {
    srand(time(NULL));  
    std::ofstream _file_writer;

    _file_writer.open(STORE_FILE_STRESS_TEST, std::ios::app);
    _file_writer << "TEST_COUNT : " << TEST_COUNT << " NUM_THREADS: " << NUM_THREADS << std::endl;
    _file_writer.close();
    
    // ~ insert 测试
    {
        pthread_t threads[NUM_THREADS];  //定义一个线程数组
        int rc;  //用于存储线程创建函数 pthread_create 的返回值。
        long i;

        auto start = std::chrono::high_resolution_clock::now();  //·获取当前时间

        for( i = 0; i < NUM_THREADS; i++ ) {
            rc = pthread_create(&threads[i], NULL, insertElement, (void *)(uintptr_t)i);

            if (rc) {
                exit(-1);
            }
        }

        void *ret;
        for( i = 0; i < NUM_THREADS; i++ ) {
            if (pthread_join(threads[i], &ret) !=0 )  {//pthread_join 函数用于等待指定线程终止。
                exit(3);
            }
        }
        auto finish = std::chrono::high_resolution_clock::now(); 
        std::chrono::duration<double> elapsed = finish - start;
        double qps = TEST_COUNT / elapsed.count();

        _file_writer.open(STORE_FILE_STRESS_TEST, std::ios::app);
        _file_writer << "insert elapsed: " << elapsed.count() << " seconds" << std::endl;
        _file_writer << "insert QPS: " << qps << std::endl;
        _file_writer.close();
    }

    // ~ update 测试
    {
        pthread_t threads[NUM_THREADS];
        int rc;
        long i;
        auto start = std::chrono::high_resolution_clock::now();

        for( i = 0; i < NUM_THREADS; i++ ) {
            rc = pthread_create(&threads[i], NULL, fixElement, (void *)(uintptr_t)i);

            if (rc) {
                exit(-1);
            }
        }

        void *ret;
        for( i = 0; i < NUM_THREADS; i++ ) {
            if (pthread_join(threads[i], &ret) !=0 )  {
                exit(3);
            }
        }

        auto finish = std::chrono::high_resolution_clock::now(); 
        std::chrono::duration<double> elapsed = finish - start;
        double qps = TEST_COUNT / elapsed.count();

        _file_writer.open(STORE_FILE_STRESS_TEST, std::ios::app);
        _file_writer << "update elapsed: " << elapsed.count() << " seconds" << std::endl;
        _file_writer << "update QPS: " << qps << std::endl;
        _file_writer.close();
    }

    // ~ search 测试
    {
        pthread_t threads[NUM_THREADS];
        int rc;
        long i;
        auto start = std::chrono::high_resolution_clock::now();

        for( i = 0; i < NUM_THREADS; i++ ) {
            rc = pthread_create(&threads[i], NULL, getElement, (void *)(uintptr_t)i);

            if (rc) {
                exit(-1);
            }
        }

        void *ret;
        for( i = 0; i < NUM_THREADS; i++ ) {
            if (pthread_join(threads[i], &ret) !=0 )  {
                exit(3);
            }
        }

        auto finish = std::chrono::high_resolution_clock::now(); 
        std::chrono::duration<double> elapsed = finish - start;
        double qps = TEST_COUNT / elapsed.count();

        _file_writer.open(STORE_FILE_STRESS_TEST, std::ios::app);
        _file_writer << "get elapsed: " << elapsed.count() << " seconds" << std::endl;
        _file_writer << "get QPS: " << qps << std::endl;
        _file_writer.close();
    }

    // ~ delete 测试
    {
        pthread_t threads[NUM_THREADS];
        int rc;
        long i;
        auto start = std::chrono::high_resolution_clock::now();

        for( i = 0; i < NUM_THREADS; i++ ) {
            rc = pthread_create(&threads[i], NULL, deleteElement, (void *)(uintptr_t)i);

            if (rc) {
                exit(-1);
            }
        }

        void *ret;
        for( i = 0; i < NUM_THREADS; i++ ) {
            if (pthread_join(threads[i], &ret) !=0 )  {
                exit(3);
            }
        }

        auto finish = std::chrono::high_resolution_clock::now(); 
        std::chrono::duration<double> elapsed = finish - start;
        double qps = TEST_COUNT / elapsed.count();

        _file_writer.open(STORE_FILE_STRESS_TEST, std::ios::app);
        _file_writer << "delete elapsed: " << elapsed.count() << " seconds" << std::endl;
        _file_writer << "delete QPS: " << qps << std::endl;
        _file_writer << std::endl;
        _file_writer.close();
    }

    pthread_exit(NULL);
    std::cout << "success!" << std::endl;
    return 0;
}

// #include <iostream>
// #include <chrono>
// #include <cstdlib>
// #include <pthread.h>
// #include <ctime>
// #include <fstream>
// #include <cstdint>
// #include "../skiplist.h"
// #define STORE_FILE_STRESS_TEST "test_result2"

// #define NUM_THREADS 1
// #define TEST_COUNT 1000
// SkipList<int, std::string> skipList(18);

// void *insertElement(void* threadid) {
//     uintptr_t tid = reinterpret_cast<uintptr_t>(threadid);
//     long tid_long = static_cast<long>(tid);
//     int tmp = TEST_COUNT / NUM_THREADS; 
//     for (int i = tid_long * tmp, count = 0; count < tmp; i++) {
//         count++;
//         skipList.insert_element(rand() % TEST_COUNT, "a"); 
//     }
//     pthread_exit(NULL);
// }

// void *getElement(void* threadid) {
//     uintptr_t tid = reinterpret_cast<uintptr_t>(threadid);
//     long tid_long = static_cast<long>(tid);
//     int tmp = TEST_COUNT / NUM_THREADS; 
//     for (int i = tid_long * tmp, count = 0; count < tmp; i++) {
//         count++;
//         skipList.search_element(rand() % TEST_COUNT); 
//     }
//     pthread_exit(NULL);
// }

// void *deleteElement(void* threadid) {
//     uintptr_t tid = reinterpret_cast<uintptr_t>(threadid);
//     long tid_long = static_cast<long>(tid);
//     int tmp = TEST_COUNT / NUM_THREADS; 
//     for (int i = tid_long * tmp, count = 0; count < tmp; i++) {
//         count++;
//         skipList.delete_element(rand() % TEST_COUNT); 
//     }
//     pthread_exit(NULL);
// }

// void *fixElement(void* threadid) {
//     uintptr_t tid = reinterpret_cast<uintptr_t>(threadid);
//     long tid_long = static_cast<long>(tid);
//     int tmp = TEST_COUNT / NUM_THREADS; 
//     for (int i = tid_long * tmp, count = 0; count < tmp; i++) {
//         count++;
//         skipList.update_element(rand() % TEST_COUNT, "b", true); 
//     }
//     pthread_exit(NULL);
// }

// void logTime(const std::string& operation, std::chrono::duration<double> elapsed) {
//     std::ofstream _file_writer;
//     _file_writer.open(STORE_FILE_STRESS_TEST, std::ios::app);
//     _file_writer << operation << " elapsed: " << elapsed.count() << " seconds" << std::endl;
//     _file_writer.close();
// }

// void testOperation(const std::string& operation, void *(*func)(void *)) {
//     pthread_t threads[NUM_THREADS];
//     int rc;
//     long i;

//     auto start = std::chrono::high_resolution_clock::now();
//     for (i = 0; i < NUM_THREADS; i++) {
//         rc = pthread_create(&threads[i], NULL, func, (void *)i);
//         if (rc) {
//             std::cerr << "Error: unable to create thread, " << rc << std::endl;
//             exit(-1);
//         }
//     }

//     void *ret;
//     for (i = 0; i < NUM_THREADS; i++) {
//         if (pthread_join(threads[i], &ret) != 0) {
//             std::cerr << "Error: unable to join thread" << std::endl;
//             exit(3);
//         }
//     }

//     auto finish = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> elapsed = finish - start;
//     logTime(operation, elapsed);
// }

// int main() {
//     srand(time(NULL));

//     std::ofstream _file_writer;
//     _file_writer.open(STORE_FILE_STRESS_TEST, std::ios::app);
//     _file_writer << "TEST_COUNT: " << TEST_COUNT << " NUM_THREADS: " << NUM_THREADS << std::endl;
//     _file_writer.close();

//     testOperation("insert", insertElement);
//     testOperation("update", fixElement);
//     testOperation("search", getElement);
//     testOperation("delete", deleteElement);

//     pthread_exit(NULL);
//     std::cout << "Success!" << std::endl;
//     return 0;
// }
