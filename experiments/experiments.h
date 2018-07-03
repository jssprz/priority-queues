#pragma once

#include "timer.h"
#include <algorithm>    // std::make_heap, std::pop_heap, std::push_heap, std::sort_heap
#include <numeric>		// std::accumulate
#include <deque>
#include <vector>
#include <string>

using namespace std;

namespace experiments {
	class Assert{
	public:
		static void IsTrue(bool cond, string msg){
			if(!cond)
				throw msg;
		}
	};

	template<typename PQ>
	void special_sort_test(ofstream &report_file, PQ *queue) {
		DECLARE_TIMING(creation_timer);
		DECLARE_TIMING(extract_all_timer);

		vector<int> data(33554432/*2097152*/);
		std::iota(std::begin(data), std::end(data), 0);

		srand(TRIALS); //to generate the same trials in all experiments

		auto create_times = vector<vector<double>>(TRIALS, vector<double>()),
			extract_all_times = vector<vector<double>>(TRIALS, vector<double>()),
			subtotal_times = vector<vector<double>>(TRIALS, vector<double>());

		for (int trial = 0; trial < TRIALS; trial++) {
			cout << "\r\tExecuting trial: " << (trial+1) << " of " << TRIALS << endl;
			random_shuffle(data.begin(), data.end());

			int n = 32768;
			for (int i = 15; i <= 25; i++) {
				cout << "\r\t\tN: " << n;

				auto data_subset = vector<int>(data.begin(), data.begin() + n);
				START_TIMING(creation_timer);
				queue->create(data_subset);
				STOP_TIMING(creation_timer);

				create_times[trial].push_back(GET_TIMING(creation_timer));

				START_TIMING(extract_all_timer);
				auto sorted = queue->extract_all();
				STOP_TIMING(extract_all_timer);

				extract_all_times[trial].push_back(GET_TIMING(extract_all_timer));
				subtotal_times[trial].push_back(GET_TIMING(creation_timer) + GET_TIMING(extract_all_timer));

				//check data is correct
				Assert::IsTrue(sorted.size() == n, "Data is not complete");
				for (vector<int>::iterator it = sorted.begin() + 1; it != sorted.end(); ++it)
					Assert::IsTrue(*it < *(it - 1), "Data is not sorted");

				n = n << 1;
			}
		}

		string latex_creation_plot = "", latex_extract_all_plot = "", latex_total_plot = "";
		int n = 32768;
		for (int i = 15; i <= 25; i++) {
			report_file << "***SPECIAL SORT (n=" << n << ")***" << endl;
			double avg_create_time = 0, avg_extract_all_time = 0, avg_subtotal_time = 0;
			for (int j = 0; j < TRIALS; j++) {
				avg_create_time += create_times[j][i-15];
				avg_extract_all_time += extract_all_times[j][i-15];
				avg_subtotal_time += create_times[j][i-15] + extract_all_times[j][i-15];
			}
			avg_create_time /= TRIALS;
			avg_extract_all_time /= TRIALS;
			avg_subtotal_time /= TRIALS;

			double sd_create_time = 0, sd_extract_all_time = 0, sd_subtotal_time = 0;
			for (int j = 0; j < TRIALS; j++) {
				sd_create_time += (create_times[j][i - 15] - avg_create_time) * (create_times[j][i - 15] - avg_create_time);
				sd_extract_all_time += (extract_all_times[j][i - 15] - avg_extract_all_time) * (extract_all_times[j][i - 15] - avg_extract_all_time);
				sd_subtotal_time += (subtotal_times[j][i - 15] - avg_subtotal_time) * (subtotal_times[j][i - 15] - avg_subtotal_time);
			}
			sd_create_time = sqrt(sd_create_time / TRIALS);
			sd_extract_all_time = sqrt(sd_extract_all_time / TRIALS);
			sd_subtotal_time = sqrt(sd_subtotal_time / TRIALS);

			report_file << "\tcreate heap:\t\t" << avg_create_time * 1000 << "(+-" << sd_create_time * 1000 << ")ms" << endl;
			report_file << "\textract all:\t\t" << avg_extract_all_time * 1000 << "(+-" << sd_extract_all_time * 1000 << ")ms" << endl;
			report_file << "\ttotal (n=" << n << "):\t\t" << (avg_subtotal_time) * 1000 << "(+-" << sd_subtotal_time * 1000 << ")ms" << endl << endl;

			latex_creation_plot += "(" + to_string(i) + ", " + to_string(avg_create_time * 1000) + ")";
            latex_extract_all_plot += "(" + to_string(i) + ", " + to_string(avg_extract_all_time * 1000) + ")";
            latex_total_plot += "(" + to_string(i) + ", " + to_string(avg_subtotal_time * 1000) + ")";

			n = n << 1;
		}

        report_file << "\n\nlatex points to plot creation times: \n" << latex_creation_plot;
        report_file << "\n\nlatex points to plot extract_all times: \n" << latex_extract_all_plot;
        report_file << "\n\nlatex points to plot totals times: \n" << latex_total_plot;
    }

	template<typename PQ>
	void sort_inserting_test(ofstream &report_file) {
		DECLARE_TIMING(insertion_timer);
		DECLARE_TIMING(extraction_timer);

		vector<int> data(33554432/*2097152*/);
		std::iota(std::begin(data), std::end(data), 0);

		srand(TRIALS); //to generate the same trials in all experiments

		auto insert_all_times = vector<vector<double>>(TRIALS, vector<double>()),
			insert_each_times = vector<vector<double>>(TRIALS, vector<double>()),
			extract_all_times = vector<vector<double>>(TRIALS, vector<double>()),
			extract_each_times = vector<vector<double>>(TRIALS, vector<double>()),
			subtotal_times = vector<vector<double>>(TRIALS, vector<double>());

		for (int trial = 0; trial < TRIALS; trial++) {
			cout << "\r\tExecuting trial: " << (trial+1) << " of " << TRIALS << endl;
			random_shuffle(data.begin(), data.end());

			int n = 32768;
			for (int i = 15; i <= 25; i++) {
				cout << "\r\t\tN: " << n;

				auto queue = new PQ();
				int j = 0;
                START_TIMING(insertion_timer);
				for (j = 0; j < n; j++)
					queue->insert(data[j]);
                STOP_TIMING(insertion_timer);

                auto insert_time = GET_TIMING(insertion_timer);
                insert_all_times[trial].push_back(insert_time);
				insert_each_times[trial].push_back(insert_time / n);

				vector<int> sorted(n);
                START_TIMING(extraction_timer);
                for (j = 0; j < n; j++)
					sorted[j] = queue->extract_next();
                STOP_TIMING(extraction_timer);

                auto extract_time = GET_TIMING(extraction_timer);
				extract_all_times[trial].push_back(extract_time);
				extract_each_times[trial].push_back(extract_time / n);

				//check data is sorted
				Assert::IsTrue(queue->count() == 0, "The queue is not empty");
				Assert::IsTrue(sorted.size() == n, "The extracted data is not complete");
				for (vector<int>::iterator it = sorted.begin() + 1; it != sorted.end(); ++it)
					Assert::IsTrue(*it < *(it - 1), "Data is not sorted");

				delete queue;

				subtotal_times[trial].push_back(insert_time + extract_time);

				n = n << 1;
			}
		}

        string latex_insert_all_plot = "", latex_insert_each_plot = "", latex_extract_all_plot = "", latex_extract_each_plot = "", latex_total_plot = "";
        int n = 32768;
		for (int i = 15; i <= 25; i++) {
			report_file << "***SORT INSERTING (n=" << n << ")***" << endl;
			double avg_insert_all_time = 0, avg_insert_each_time = 0, avg_extract_all_time = 0, avg_extract_each_time = 0, avg_subtotal_time = 0;
			for (int j = 0; j < TRIALS; j++) {
				avg_insert_all_time += insert_all_times[j][i-15];
				avg_insert_each_time += insert_each_times[j][i-15];
				avg_extract_all_time += extract_all_times[j][i-15];
				avg_extract_each_time += extract_each_times[j][i-15];
				avg_subtotal_time += subtotal_times[j][i-15];
			}
			avg_insert_all_time /= TRIALS;
			avg_insert_each_time /= TRIALS;
			avg_extract_all_time /= TRIALS;
			avg_extract_each_time /= TRIALS;
			avg_subtotal_time /= TRIALS;

			double sd_insert_all_time = 0, sd_insert_each_time = 0, sd_extract_all_time = 0, sd_extract_each_time = 0, sd_subtotal_time = 0;
			for (int j = 0; j < TRIALS; j++) {
				sd_insert_all_time += (insert_all_times[j][i - 15] - avg_insert_all_time) * (insert_all_times[j][i - 15] - avg_insert_all_time);
				sd_insert_each_time += (insert_each_times[j][i - 15] - avg_insert_each_time) * (insert_each_times[j][i - 15] - avg_insert_each_time);
				sd_extract_all_time += (extract_all_times[j][i - 15] - avg_extract_all_time) * (extract_all_times[j][i - 15] - avg_extract_all_time);
				sd_extract_each_time += (extract_each_times[j][i - 15] - avg_extract_each_time) * (extract_each_times[j][i - 15] - avg_extract_each_time);
				sd_subtotal_time += (subtotal_times[j][i - 15] - avg_subtotal_time) * (subtotal_times[j][i - 15] - avg_subtotal_time);
			}
			sd_insert_all_time = sqrt(sd_insert_all_time / TRIALS);
			sd_insert_each_time = sqrt(sd_insert_each_time / TRIALS);
			sd_extract_all_time = sqrt(sd_extract_all_time / TRIALS);
			sd_extract_each_time = sqrt(sd_extract_each_time / TRIALS);
			sd_subtotal_time = sqrt(sd_subtotal_time / TRIALS);

			report_file << "\tinsert all elements:\t\t" << avg_insert_all_time * 1000 << "(+-" << sd_insert_all_time * 1000 << ")ms" << endl;
			report_file << "\tinsert each element:\t\t" << avg_insert_each_time * 1000 << "(+-" << sd_insert_each_time * 1000 << ")ms" << endl;
			report_file << "\textract all elements:\t\t" << avg_extract_all_time * 1000 << "(+-" << sd_extract_all_time * 1000 << ")ms" << endl;
			report_file << "\textract each element:\t\t" << avg_extract_each_time * 1000 << "(+-" << sd_extract_each_time * 1000 << ")ms" << endl;
			report_file << "\ttotal (n=" << n << "):\t\t" << avg_subtotal_time * 1000 << "(+-" << sd_subtotal_time * 1000 << ")ms" << endl << endl;

            latex_insert_all_plot += "(" + to_string(i) + ", " + to_string(avg_insert_all_time * 1000) + ")";
            latex_insert_each_plot += "(" + to_string(i) + ", " + to_string(avg_insert_each_time * 1000) + ")";
            latex_extract_all_plot += "(" + to_string(i) + ", " + to_string(avg_extract_all_time * 1000) + ")";
            latex_extract_each_plot += "(" + to_string(i) + ", " + to_string(avg_extract_each_time * 1000) + ")";
            latex_total_plot += "(" + to_string(i) + ", " + to_string(avg_subtotal_time * 1000) + ")";

			n = n << 1;
		}

        report_file << "\n\nlatex points to plot insert all times: \n" << latex_insert_all_plot;
        report_file << "\n\nlatex points to plot insert each times: \n" << latex_insert_each_plot;
        report_file << "\n\nlatex points to plot extract all times: \n" << latex_extract_all_plot;
        report_file << "\n\nlatex points to plot extract each times: \n" << latex_extract_each_plot;
        report_file << "\n\nlatex points to plot total times: \n" << latex_total_plot;
	}

	template<typename PQ>
	PQ* insert_meld(ofstream &report_file, vector<int> data, int k) {
		DECLARE_TIMING(creation_timer);
		DECLARE_TIMING(meld_timer);
		report_file << "***INSERT AND MELD (k=" << k << ")***" << endl;

		deque<PQ*> queues;

		//insertion
		int subset_size = data.size() / k;
		for (size_t i = 0; i < k; i++) {
			auto pq = new PQ();
			START_TIMING(creation_timer);
			pq->create(vector<int>(data.begin() + i * subset_size, data.begin() + (i + 1) * subset_size));
			STOP_TIMING(creation_timer);

			queues.push_back(pq);
		}

		report_file << "\ttotal time for creation:\t" << GET_TOTAL_TIME(creation_timer) * 1000 << "ms" << endl;
		report_file << "\taverage time for subset creation:\t" << GET_AVERAGE_TIMING(creation_timer) * 1000 << "ms" << endl;

		//melding
		while (queues.size() != 1) {
			auto c1 = queues.front(); queues.pop_front();
			auto c2 = queues.front(); queues.pop_front();

			START_TIMING(meld_timer);
			auto meld = PQ::meld(c1, c2);
			STOP_TIMING(meld_timer);

			queues.push_back(meld);
			delete c1;
			delete c2;
		}

		report_file << "\ttotal time for meld:\t" << GET_TOTAL_TIME(meld_timer) * 1000 << "ms" << endl;
		report_file << "\taverage time for meld pairs:\t" << GET_AVERAGE_TIMING(meld_timer) * 1000 << "ms" << endl;

		return queues.front();
	}

	template<typename PQ>
	void insert_and_meld_test(ofstream &report_file) {
		DECLARE_TIMING(creation_timer);
		DECLARE_TIMING(meld_timer);

		vector<int> data(1048576); /*2^20*/
		std::iota(std::begin(data), std::end(data), 0);

		srand(TRIALS); //to generate the same trials in all experiments

		random_shuffle(data.begin(), data.end());

		auto std_heap = data;
		make_heap(std_heap.begin(), std_heap.end());
		sort_heap(std_heap.begin(), std_heap.end());

		auto create_times = vector<vector<double>>(TRIALS, vector<double>()),
			create_each_times = vector<vector<double>>(TRIALS, vector<double>()),
			meld_times = vector<vector<double>>(TRIALS, vector<double>()),
			meld_each_times = vector<vector<double>>(TRIALS, vector<double>());

		for (int trial = 0; trial < TRIALS; trial++) {
			cout << "\r\tExecuting trial: " << (trial+1) << " of " << TRIALS << endl;

			int k = 1;
			for (int i = 0; i <= 15; i++) {
				cout << "\r\t\tK: " << k;
				deque<PQ*> queues;

				//insertion
				int subset_size = data.size() / k;
				for (size_t i = 0; i < k; i++) {
					auto pq = new PQ();

					START_TIMING(creation_timer);
					pq->create(vector<int>(data.begin() + i * subset_size, data.begin() + (i + 1) * subset_size));
					STOP_TIMING(creation_timer);

					queues.push_back(pq);
				}

				create_times[trial].push_back(GET_TOTAL_TIME(creation_timer));
				create_each_times[trial].push_back(GET_AVERAGE_TIMING(creation_timer));

				//melding
				while (queues.size() != 1) {
					auto c1 = queues.front(); queues.pop_front();
					auto c2 = queues.front(); queues.pop_front();

					START_TIMING(meld_timer);
					auto meld = PQ::meld(c1, c2);
					STOP_TIMING(meld_timer);

					queues.push_back(meld);
					delete c1;
					delete c2;
				}

				meld_times[trial].push_back(GET_TOTAL_TIME(meld_timer));
				meld_each_times[trial].push_back(GET_AVERAGE_TIMING(meld_timer));

				auto pq = queues.front();
				queues.pop_front();

				//check data is correct
				for (vector<int>::reverse_iterator it = std_heap.rbegin(); it != std_heap.rend(); ++it)
					Assert::IsTrue(*it == pq->extract_next(), "The heap constructed for k=" + to_string(k) + " is not sorted");
				delete pq;

				CLEAR_TIMING(creation_timer);
				CLEAR_TIMING(meld_timer);

				k = k << 1;
			}
		}

        string latex_creation_plot = "", latex_create_each_plot = "", latex_meld_plot = "", latex_meld_each_plot = "", latex_total_plot = "";
        int k = 1;
		for (int i = 0; i <= 15; i++) {
			report_file << "***INSERT AND MELD (k=" << k << ")***" << endl;
			double avg_create_time = 0, avg_create_each_time = 0, avg_meld_time = 0, avg_meld_each_time = 0;
			for (int j = 0; j < TRIALS; j++) {
				avg_create_time += create_times[j][i];
				avg_create_each_time += create_each_times[j][i];
				avg_meld_time += meld_times[j][i];
				avg_meld_each_time += meld_each_times[j][i];
			}
			avg_create_time /= TRIALS;
			avg_create_each_time /= TRIALS;
			avg_meld_time /= TRIALS;
			avg_meld_each_time /= TRIALS;

			double sd_create_time = 0, sd_create_each_time = 0, sd_meld_time = 0, sd_meld_each_time = 0;
			for (int j = 0; j < TRIALS; j++) {
				sd_create_time += (create_times[j][i] - avg_create_time) * (create_times[j][i] - avg_create_time);
				sd_meld_time += (meld_times[j][i] - avg_meld_time) * (meld_times[j][i] - avg_meld_time);
				sd_create_each_time += (create_each_times[j][i] - avg_create_each_time) * (create_each_times[j][i] - avg_create_each_time);
				sd_meld_each_time += (meld_each_times[j][i] - avg_meld_each_time) * (meld_each_times[j][i] - avg_meld_each_time);
			}
			sd_create_time = sqrt(sd_create_time / TRIALS);
			sd_meld_time = sqrt(sd_meld_time / TRIALS);
			sd_create_each_time = sqrt(sd_create_each_time / TRIALS);
			sd_meld_each_time = sqrt(sd_meld_each_time / TRIALS);

			report_file << "\tcreate all heaps:\t\t" << avg_create_time * 1000 << "(+-" << sd_create_time * 1000 << ")ms" << endl;
			report_file << "\tcreate each heap:\t\t" << avg_create_each_time * 1000 << "(+-" << sd_create_each_time * 1000 << ")ms" << endl;
			report_file << "\tmelding process:\t\t" << avg_meld_time * 1000 << "(+-" << sd_meld_time * 1000 << ")ms" << endl;
			report_file << "\teach meld:\t\t" << avg_meld_each_time * 1000 << "(+-" << sd_meld_each_time * 1000 << ")ms" << endl;
			report_file << "\ttotal (k=" << k << "):\t\t" << (avg_create_time + avg_meld_time) * 1000 << "(+-" << (sd_create_time + sd_meld_time) * 1000 << ")ms" << endl << endl;

            latex_creation_plot += "(" + to_string(i) + ", " + to_string(avg_create_time * 1000) + ")";
            latex_create_each_plot += "(" + to_string(i) + ", " + to_string(avg_create_each_time * 1000) + ")";
            latex_meld_plot += "(" + to_string(i) + ", " + to_string(avg_meld_time * 1000) + ")";
            latex_meld_each_plot += "(" + to_string(i) + ", " + to_string(avg_meld_each_time * 1000) + ")";
            latex_total_plot += "(" + to_string(i) + ", " + to_string((avg_create_time + avg_meld_time) * 1000) + ")";

			k = k << 1;
		}

        report_file << "\n\nlatex points to plot create all times: \n" << latex_creation_plot;
        report_file << "\n\nlatex points to plot create each times: \n" << latex_create_each_plot;
        report_file << "\n\nlatex points to plot meld all times: \n" << latex_meld_plot;
        report_file << "\n\nlatex points to plot meld each times: \n" << latex_meld_each_plot;
        report_file << "\n\nlatex points to plot total times: \n" << latex_total_plot;
	}

	template<typename PQ>
	void meld_test(ofstream &report_file) {
        DECLARE_TIMING(meld_timer);

        srand(TRIALS); //to generate the same trials in all experiments

        auto meld_times = vector<vector<double>>(TRIALS, vector<double>());
        for (size_t trial = 0; trial < TRIALS; trial++) {
            cout << "\r\tExecuting trial: " << (trial+1) << " of " << TRIALS << endl;

            int n = 32768;
            for (size_t i = 15; i <= 25; i++) {
                cout << "\r\t\tN: " << n;
                vector<int> data(n);
                std::iota(std::begin(data), std::end(data), 0);
                random_shuffle(data.begin(), data.end());

                vector<int> data2;
                data2.insert(data2.end(), data.begin() + n / 2, data.end());
                data.resize(n / 2);

                auto q1 = new PQ(), q2 = new PQ();
                q1->create(data); vector<int>().swap(data);
                q2->create(data2); vector<int>().swap(data2);

                START_TIMING(meld_timer);
                auto meld = PQ::meld(q1, q2);
                STOP_TIMING(meld_timer);

                meld_times[trial].push_back(GET_TIMING(meld_timer));

                //check data is correct
                auto sorted = meld->extract_all();
                Assert::IsTrue(sorted.size() == n, "Data is not complete");
                for (vector<int>::iterator it = sorted.begin() + 1; it != sorted.end(); ++it)
                    Assert::IsTrue(*it < *(it - 1), "Data is not sorted");

                delete meld;
                delete q1;
                delete q2;

                n = n << 1;
            }
        }

        string latex_meld_plot = "";
        int n = 32768;
        for (size_t i = 0; i <= 10; i++) {
            report_file << "***MELD (n=" << n << ")***" << endl;
            double avg_meld_time = 0;
            for (int j = 0; j < TRIALS; j++)
                avg_meld_time += meld_times[j][i];
            avg_meld_time /= TRIALS;

            double sd_meld_time = 0;
            for (int j = 0; j < TRIALS; j++)
                sd_meld_time += (meld_times[j][i] - avg_meld_time) * (meld_times[j][i] - avg_meld_time);

            sd_meld_time = sqrt(sd_meld_time / TRIALS);

            report_file << "\tmelding process:\t\t" << avg_meld_time * 1000 << "(+-" << sd_meld_time * 1000 << ")ms" << endl;

            latex_meld_plot += "(" + to_string(i) + ", " + to_string(avg_meld_time * 1000) + ")";

            n = n << 1;
        }

        report_file << "\n\nlatex points to plot meld times: \n" << latex_meld_plot;
    }
}