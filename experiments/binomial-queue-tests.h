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
			cout << "\rBinomialQueueSpecialSortTest started     " << endl;
			auto report_file = ofstream("tests_results/BinomialQueueSpecialSortTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			auto bq = new BQ();
			special_sort_test(report_file, bq);
			delete bq;
		}

		void BinomialQueueSortInsertingTest() {
			cout << "\rBinomialQueueSortInsertingTest started     " << endl;
			auto report_file = ofstream("tests_results/BinomialQueueSortInsertingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			sort_inserting_test<BQ>(report_file);
		}

		void BinomialQueueInsertAndMeldingTest() {
			cout << "\rBinomialQueueInsertAndMeldingTest started     " << endl;
			auto report_file = ofstream("tests_results/BinomialQueueInsertAndMeldingTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			insert_and_meld_test<BQ>(report_file);
		}

		void BinomialQueueMeldTest() {
			cout << "\rBinomialQueueMeldTest started     " << endl;
			auto report_file = ofstream("tests_results/BinomialQueueMeldTest.report", ios::trunc);
			report_file << fixed << setprecision(5);
			meld_test<BQ>(report_file);
		}
	};
}