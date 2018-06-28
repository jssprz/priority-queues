#include <fstream>
#include <iomanip>      // std::setprecision
#include "experiments.h"
#include "../binomial-pq/binomial-pq.h"

using namespace std;
using namespace priority_queue;

namespace experiments {
	class BinomialQueueTest {
	public:
		typedef BinomialPQueue<int> BQ;
		
		void BinomialQueueSpecialSortTest() {
			cout << "BinomialQueueSpecialSortTest started" << endl;
			auto report_file = ofstream("tests/BinomialQueueSpecialSortTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			auto bq = new BQ();
			special_sort_test(report_file, bq);
			delete bq;
		}

		void BinomialQueueSortInsertingTest() {
			cout << "BinomialQueueSortInsertingTest started" << endl;
			auto report_file = ofstream("tests/BinomialQueueSortInsertingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			sort_inserting_test<BQ>(report_file);
		}

		void BinomialQueueInsertAndMeldingTest() {
			cout << "BinomialQueueInsertAndMeldingTest started" << endl;
			auto report_file = ofstream("tests/BinomialQueueInsertAndMeldingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			insert_and_meld_test<BQ>(report_file);
		}

		void BinomialQueueMeldTest() {
			cout << "BinomialQueueMeldTest started" << endl;
			auto report_file = ofstream("tests/BinomialQueueMeldTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			meld_test<BQ>(report_file);
		}
	};
}