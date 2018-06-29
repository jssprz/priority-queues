#include <iostream>
#include <string>
#include "config.h"

#ifdef USE_EXPERIMENTS
#ifdef _MSC_VER
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#ifdef USE_BINARY_HEAP_PQ
#include "experiments/binary-heap-tests.h"
#endif
#ifdef USE_LEFTIST_HEAP_PQ
#include "experiments/leftist-heap-tests.h"
#endif
#ifdef USE_SKEW_HEAP_PQ
#include "experiments/skew-heap-tests.h"
#endif
#ifdef USE_BINOMIAL_PQ
#include "experiments/binomial-queue-tests.h"
#endif
#ifdef USE_FIBONACCI_PQ
#include "experiments/fibonacci-queue-tests.h"
#endif
#endif

using namespace std;
#ifdef USE_EXPERIMENTS
using namespace experiments;
#endif

int main() {
#ifdef USE_EXPERIMENTS
    try {
        string strPath = "tests_results";
#ifdef _WIN32
        const int dir_err = _mkdir(strPath.c_str());
#else
        const int dir_err = mkdir(strPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

#endif
        cout << dir_err << endl;
        if (dir_err != 0 && dir_err != -1 && dir_err != EEXIST)
            throw string("error creating 'tests_results' folder");

#ifdef USE_BINARY_HEAP_PQ
        //binary-heap tests group
        auto bh_tests = BinaryHeapPQueueTest();
        bh_tests.BHeapSpecialSortTest();
        bh_tests.BHeapSortInsertingTest();
        bh_tests.BHeapInsertAndMeldingTest();
        bh_tests.BHeapMeldTest();
#endif
#ifdef USE_LEFTIST_HEAP_PQ
        //leftist-heap tests group
        auto lh_tests = LeftistHeapTest();
        lh_tests.LeftistHeapSpecialSortTest();
        lh_tests.LeftistHeapSortInsertingTest();
        lh_tests.LeftistHeapInsertAndMeldingTest();
        lh_tests.LeftistHeapMeldTest();
#endif
#ifdef USE_SKEW_HEAP_PQ
        //skew-heap tests group
        auto sh_tests = SkewHeapTests();
        sh_tests.SkewHeapSpecialSortTest();
        sh_tests.SkewHeapSortInsertingTest();
        sh_tests.SkewHeapInsertAndMeldingTest();
        sh_tests.SkewHeapMeldTest();
#endif
#ifdef USE_BINOMIAL_PQ
        //binomial-queue tests group
        auto bq_tests = BinomialQueueTest();
        bq_tests.BinomialQueueSpecialSortTest();
        bq_tests.BinomialQueueSortInsertingTest();
        bq_tests.BinomialQueueInsertAndMeldingTest();
        bq_tests.BinomialQueueMeldTest();
#endif
#ifdef USE_FIBONACCI_PQ
        //fibonacci-queue tests group
        auto fq_tests = FibonacciHeapTest();
        fq_tests.FibonacciQueueSpecialSortTest();
        fq_tests.FibonacciQueueSortInsertingTest();
        fq_tests.FibonacciQueueInsertAndMeldingTest();
        fq_tests.FibonacciQueueMeldTest();
#endif
    }
    catch (string e){
        cout << e << endl;
    }
#else
    cout << "the experiments files was not included" << endl;
#endif
    return 0;
}