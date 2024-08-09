#include <iostream>
#include <chrono> //用于计时
#include <cstdlib> //用于随机数
#include <time.h> //用于随机数
#include <pthread.h> //用于多线程
#include <fstream> //用于文件操作
#include <cstdint> //用于int64_t
#include "../skiplist.h"
#define STORE_FILE_STRESS_TEST "test_result"

#define NUM_THREADS 1    //线程数
#define TEST_COUNT 1000  //测试次数
SkipList<int , std::string> skipList(18);

void *insertElement(void* threadid){//插入元素的线程函数
    uintptr_t tid;
    tid = reinterpret_cast<uintptr_t>(threadid);
    long tid_long = static_cast<long>(tid);//线程id

    int tmp = TEST_COUNT / NUM_THREADS;//每个线程处理的操作数量
    for(int i = tid_long * tmp , count = 0; count < tmp ; i++ , count++){
        skipList.insert_element(rand() % TEST_COUNT , "a");
    }
    pthread_exit(NULL);//终止当前线程的执行，并返回NULL
}

void *searchElement(void* threadid){//查找元素的线程函数
    uintptr_t tid;
    tid = reinterpret_cast<uintptr_t>(threadid);
    long tid_long = static_cast<long>(tid);//线程id

    int tmp = TEST_COUNT / NUM_THREADS;//每个线程处理的操作数量
    for(int i = tid_long*tmp , count = 0 ; count < tmp ; i++ , count++)
        skipList.search_element(rand()%TEST_COUNT);
    pthread_exit(NULL);//终止当前线程的执行，并返回NULL
}

void *deleteElement(void* threadid){
    uintptr_t tid;
    tid = reinterpret_cast<uintptr_t>(threadid);
    long tid_long = static_cast<long>(tid);//线程id

    int tmp = TEST_COUNT / NUM_THREADS;//每个线程处理的操作数量
    for(int i = tid_long*tmp , count = 0 ; count < tmp ; i++ , count++)
        skipList.delete_element(rand()%TEST_COUNT);
    pthread_exit(NULL);//终止当前线程的执行，并返回NULL
}

void *upElement(void* threadid){
    uintptr_t tid;
    tid = reinterpret_cast<uintptr_t>(threadid);
    long tid_long = static_cast<long>(tid);

    int tmp = TEST_COUNT / NUM_THREADS;
    for(int i = tid_long*tmp , count = 0 ; count < tmp ; i++ , count++)
        skipList.update_element(rand()%TEST_COUNT , "b" , true);
    pthread_exit(NULL);
}

int main(){
    srand(time(NULL));
    std::ofstream _file_writer;
    _file_writer.open(STORE_FILE_STRESS_TEST,std::ios::app);
    _file_writer << "TEST_COUNT : " << TEST_COUNT << " NUM_THREADS: " << NUM_THREADS << std::endl;
    _file_writer.close();

    //insert测试
    {
        pthread_t threads[NUM_THREADS];//定义一个线程数组
        int rc; //用于存储线程创建函数 pthread_create 的返回值
        long i;

        auto start = std::chrono::high_resolution_clock::now(); //获取当前时间

        for(i = 0 ; i < NUM_THREADS ; i++){
            rc = pthread_create(&threads[i],NULL,insertElement,(void *)(uintptr_t)i);
            if(rc)
                exit(-1);
        }
        void *ret;
        for(i = 0 ; i < NUM_THREADS ; i++){
            if(pthread_join(threads[i],&ret) != 0)//等待指定线程终止
                exit(3);
        }
        auto finish = std::chrono::high_resolution_clock::now(); //获取当前时间
        std::chrono::duration<double> elapsed = finish - start;
        double qps = TEST_COUNT / elapsed.count();

        _file_writer.open(STORE_FILE_STRESS_TEST , std::ios::app);
        _file_writer << "insert elapsed: " << elapsed.count() << " seconds" << std::endl;
        _file_writer << "insert QPS: " << qps << std::endl;
        _file_writer.close();
    }

    //update测试
    {
        pthread_t threads[NUM_THREADS];
        int rc;
        long i;
        auto start = std::chrono::high_resolution_clock::now();

        for(i = 0 ; i < NUM_THREADS ; i++){
            rc = pthread_create(&threads[i],NULL,upElement,(void *)(uintptr_t)i);
            if(rc)
                exit(-1);
        }

        void *ret;
        for(i = 0 ; i < NUM_THREADS ; i++){
            if(pthread_join(threads[i],&ret) != 0)//等待线程终止
                exit(3);
        }
        
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        double qps = TEST_COUNT / elapsed.count();

        _file_writer.open(STORE_FILE_STRESS_TEST,std::ios::app);
        _file_writer << "update elapsed: " << elapsed.count() << " seconds" << std::endl;
        _file_writer << "update QPS: " << qps << std::endl;
        _file_writer.close();
    }

    //search测试
    {
        pthread_t threads[NUM_THREADS];
        long i;
        int rc;
        auto start = std::chrono::high_resolution_clock::now();

        for(i = 0 ; i < NUM_THREADS ; i++){
            rc = pthread_create(&threads[i],NULL,searchElement,(void *)(uintptr_t)i);
            if(rc)
                exit(-1);
        }

        void *ret;
        for(i = 0 ; i < NUM_THREADS ; i++)
            if(pthread_join(threads[i],&ret) != 0)
                exit(3);
        
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        double qps = TEST_COUNT / elapsed.count();

        _file_writer.open(STORE_FILE_STRESS_TEST , std::ios::app);
        _file_writer << "search elapsed: " << elapsed.count() << " seconds" << std::endl;
        _file_writer << "search QPS: " << qps << std::endl;
        _file_writer.close();
    }
    {
        pthread_t threads[NUM_THREADS];
        int rc;
        long i;
        auto start = std::chrono::high_resolution_clock::now();
        for(i = 0 ; i < NUM_THREADS ; i++){
            rc = pthread_create(&threads[i],NULL,deleteElement,(void *)(uintptr_t)i);
            if(rc)
                exit(-1);
        }

        void *ret;
        for(i = 0 ; i < NUM_THREADS ; i++){
            if(pthread_join(threads[i],&ret) != 0)
                exit(3);
        }

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        double qps = TEST_COUNT / elapsed.count();

        _file_writer.open(STORE_FILE_STRESS_TEST , std::ios::app);
        _file_writer << "delete elapsed: " << elapsed.count() << " seconds" << std::endl;
        _file_writer << "delete QPS: " << qps << std::endl;
        _file_writer << std::endl;
        _file_writer.close();
    }
    pthread_exit(NULL);
    std::cout << "stress test finished" << std::endl;\
    return 0;
}