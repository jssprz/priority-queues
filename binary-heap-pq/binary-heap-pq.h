#pragma once

#include <iostream>
#include <vector>
#include "../priority-queue.h"

using namespace std;

namespace priority_queue {
	template<typename T>
	class BinaryHeapPQueue : public IPriorityQueue<T> {
		typedef BinaryHeapPQueue<T> BH;

	public:
		BinaryHeapPQueue() {}

		virtual void create(const vector<T> &data) {
			heap = data;
			this->n = data.size();
			// Build heap (rearrange array)
			for (int i = this->n / 2 - 1; i >= 0; i--)
				top_down(i, this->n);
		}

		virtual void insert(const T &value) {
			heap.push_back(value);
			botom_up(heap.size() - 1);
			this->n++;
		}

		virtual T extract_next() {
			swap(heap.size() - 1, 0);
			T value = heap.back();
			heap.pop_back();
			top_down(0, --this->n);
			return value;
		}

		virtual vector<T> extract_all() {
			// performs heap-sort over heap vector
			while (--this->n) {
				// change current root (max element) with the current last
				swap(0, this->n);

				// max heapify on the reduced heap
				top_down(0, this->n);
			}

			vector<T> result(this->heap.rbegin(), this->heap.rend());

			// clear the heap
			heap = vector<T>();

			return result;
		}

		static BH* meld(BH *c1, BH *c2) {
			auto merged = new BH();
			merged->heap.reserve(c1->heap.size() + c2->heap.size()); // preallocate memory
			merged->heap.insert(merged->heap.end(), c1->heap.begin(), c1->heap.end());
			merged->heap.insert(merged->heap.end(), c2->heap.begin(), c2->heap.end());
			merged->n = merged->heap.size();

			// Build heap (rearrange array)
			for (int i = merged->n / 2 - 1; i >= 0; i--)
				merged->top_down(i, merged->n);

			return merged;
		}

	private:
		void swap(int idx1, int idx2) {
			T aux = heap[idx1];
			heap[idx1] = heap[idx2];
			heap[idx2] = aux;
		}

		int get_parent(int child) {
			return !(child % 2) ? (child >> 1) - 1 : child >> 1;
		}

		int get_left_child(int parent) {
			return (parent << 1) + 1;
		}

		int get_right_child(int parent) {
			return (parent << 1) + 2;
		}

		void botom_up(int idx) {
			int parent = get_parent(idx);
			while (idx >= 0 && parent >= 0 && heap[idx] > heap[parent]) {
				swap(idx, parent);
				idx = parent;
				parent = get_parent(idx);
			}
		}

		void top_down(int idx, int length) {
			while (true) {
				int left = get_left_child(idx);
				int right = get_right_child(idx);
				int largest = idx;

				if (left < length && heap[left] > heap[largest])
					largest = left;

				if (right < length && heap[right] > heap[largest])
					largest = right;

				if (largest != idx) {
					swap(largest, idx);
					idx = largest;
				}
				else
					break;
			}
		}

		// heap array representation
		vector<T> heap;
	};
}