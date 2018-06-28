#include <fstream>
#include <iomanip>      // std::setprecision
#include "experiments.h"
#include "../fibonacci-pq/fibonacci-pq.h"

using namespace std;
using namespace priority_queue;

namespace experiments {
	class FibonacciHeapTest {
	public:
		typedef FibonacciPQueue<int> FQ;
		
		void FibonacciQueueSpecialSortTest() {
			cout << "FibonacciQueueSpecialSortTest started" << endl;
			auto report_file = ofstream("tests/FibonacciQueueSpecialSortTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			auto fq = new FQ();
			special_sort_test(report_file, fq);
			delete fq;
		}

		void FibonacciQueueSortInsertingTest() {
			cout << "FibonacciQueueSortInsertingTest started" << endl;
			auto report_file = ofstream("tests/FibonacciQueueSortInsertingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			sort_inserting_test<FQ>(report_file);
		}

		void FibonacciQueueInsertAndMeldingTest() {
			cout << "FibonacciQueueInsertAndMeldingTest started" << endl;
			auto report_file = ofstream("tests/FibonacciQueueInsertAndMeldingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			insert_and_meld_test<FQ>(report_file);
		}

		void FibonacciQueueMeldTest() {
			cout << "FibonacciQueueMeldTest started" << endl;
			auto report_file = ofstream("tests/FibonacciQueueMeldTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			meld_test<FQ>(report_file);
		}
	};

	
}