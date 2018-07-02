#include <fstream>
#include <iomanip>      // std::setprecision
#include "experiments.h"
#include "../binary-heap-pq/binary-heap-pq.h"

using namespace std;
using namespace priority_queue;

namespace experiments {
	class BinaryHeapPQueueTest {
	public:
		typedef BinaryHeapPQueue<int> BH;
		
		void BHeapSpecialSortTest() {
			cout << "\rBHeapSpecialSortTest started     " << endl;
			auto report_file = ofstream("tests_results/BHeapSpecialSortTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			auto bh = new BH();
			special_sort_test(report_file, bh);
			delete bh;
		}

		void BHeapSortInsertingTest() {
			cout << "\rBHeapSortInsertingTest started     " << endl;
			auto report_file = ofstream("tests_results/BHeapSortInsertingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			sort_inserting_test<BH>(report_file);
		}

		void BHeapInsertAndMeldingTest() {
			cout << "\rBHeapInsertAndMeldingTest started     " << endl;
			auto report_file = ofstream("tests_results/BHeapInsertAndMeldingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			insert_and_meld_test<BH>(report_file);
		}

		void BHeapMeldTest() {
			cout << "\rBHeapMeldTest started     " << endl;
			auto report_file = ofstream("tests_results/BHeapMeldTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			meld_test<BH>(report_file);
		}
	};
}