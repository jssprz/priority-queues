#include <fstream>
#include <iomanip>      // std::setprecision
#include "experiments.h"
#include "../skew-heap-pq/skew-heap-pq.h"

using namespace std;
using namespace priority_queue;

namespace experiments {		
	class SkewHeapTests	{
		typedef SkewHeapPQueue<int> SH;
	public:
		void SkewHeapSpecialSortTest() { 
			cout << "\rSkewHeapSpecialSortTest started     " << endl;
			auto report_file = ofstream("tests_results/SkewHeapSpecialSortTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			auto fq = new SH();
			special_sort_test(report_file, fq);
			delete fq;
		}

		void SkewHeapSortInsertingTest() { 
			cout << "\rSkewHeapSortInsertingTest started     " << endl;
			auto report_file = ofstream("tests_results/SkewHeapSortInsertingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			sort_inserting_test<SH>(report_file);
		}

		void SkewHeapInsertAndMeldingTest() { 
			cout << "\rSkewHeapInsertAndMeldingTest started     " << endl;
			auto report_file = ofstream("tests_results/SkewHeapInsertAndMeldingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			insert_and_meld_test<SH>(report_file);
		}

		void SkewHeapMeldTest() { 
			cout << "\rSkewHeapMeldTest started     " << endl;
			auto report_file = ofstream("tests_results/SkewHeapMeldTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			meld_test<SH>(report_file);
		}
	};
}