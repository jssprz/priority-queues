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
			cout << "SkewHeapSpecialSortTest started" << endl;
			auto report_file = ofstream("tests/SkewHeapSpecialSortTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			auto fq = new SH();
			special_sort_test(report_file, fq);
			delete fq;
		}

		void SkewHeapSortInsertingTest() { 
			cout << "SkewHeapSortInsertingTest started" << endl;
			auto report_file = ofstream("tests/SkewHeapSortInsertingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			sort_inserting_test<SH>(report_file);
		}

		void SkewHeapInsertAndMeldingTest() { 
			cout << "SkewHeapInsertAndMeldingTest started" << endl;
			auto report_file = ofstream("tests/SkewHeapInsertAndMeldingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			insert_and_meld_test<SH>(report_file);
		}

		void SkewHeapMeldTest() { 
			cout << "SkewHeapMeldTest started" << endl;
			auto report_file = ofstream("tests/SkewHeapMeldTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			meld_test<SH>(report_file);
		}
	};
}