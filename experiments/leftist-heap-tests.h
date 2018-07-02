#include <fstream>
#include <iomanip>      // std::setprecision
#include "experiments.h"
#include "../leftist-heap-pq/leftist-heap-pq.h"

using namespace std;
using namespace priority_queue;

namespace experiments {		
	class LeftistHeapTest {
		typedef LeftistHeapPQueue<int> LH;
	public:
		void LeftistHeapSpecialSortTest() { 
			cout << "\rLeftistHeapSpecialSortTest started     " << endl;
			auto report_file = ofstream("tests_results/LeftistHeapSpecialSortTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			auto fq = new LH();
			special_sort_test(report_file, fq);
			delete fq;
		}

		void LeftistHeapSortInsertingTest() { 
			cout << "\rLeftistHeapSortInsertingTest started     " << endl;
			auto report_file = ofstream("tests_results/LeftistHeapSortInsertingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			sort_inserting_test<LH>(report_file);
		}

		void LeftistHeapInsertAndMeldingTest() { 
			cout << "\rLeftistHeapInsertAndMeldingTest started     " << endl;
			auto report_file = ofstream("tests_results/LeftistHeapInsertAndMeldingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			insert_and_meld_test<LH>(report_file);
		}

		void LeftistHeapMeldTest() { 
			cout << "\rLeftistHeapMeldTest started     " << endl;
			auto report_file = ofstream("tests_results/LeftistHeapMeldTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			meld_test<LH>(report_file);
		}
	};

	
}