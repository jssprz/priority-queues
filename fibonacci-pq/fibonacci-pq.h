#pragma once

#include "fibonacci-pq-node.h"
#include "../priority-queue.h"

using namespace std;

namespace priority_queue {
	template<typename T>
	class FibonacciPQueue : public IPriorityQueue<T> {
		typedef FibonacciPQueue<T> FQ;
		typedef FibonacciHeapNode<T> Node;

	public:
		FibonacciPQueue() :forest(NULL) {}

		virtual void create(const vector<T> &data) {
			forest = new Node(data[0]);
			forest->next = forest->prev = forest;
			this->n = data.size();
			auto end_it = data.end();
			for (typename vector<T>::const_iterator it = data.begin() + 1; it != end_it; ++it) {
				auto new_node = new Node(*it);
				forest->prev->next = new_node;
				new_node->prev = forest->prev;
				forest->prev = new_node;
				new_node->next = forest;
				if (forest->key < *it)
					forest = new_node;
			}
		}

		virtual void insert(const T &value) {
			if (!forest) {
				forest = new Node(value);
				forest->next = forest->prev = forest;
				this->n = 1;
				return;
			}

			auto new_node = new Node(value);
			forest->prev->next = new_node;
			new_node->prev = forest->prev;
			forest->prev = new_node;
			new_node->next = forest;
			if (forest->key < value)
				forest = new_node;
			this->n++;
		}

		virtual T extract_next() {
			auto current_max = forest;
			auto result = current_max->key;

			//1- Delete root of the tree that contains the max
			if (forest->next == forest) { //if forest has only one node
				forest = forest->child;
				if (!forest) {
					this->n = 0;
					return result;
				}
			}
			else {
				forest->next->prev = forest->prev;
				forest->prev->next = forest->next;
				forest = merge_forests(forest->next, forest->child); //add children to the forest

				//2- Convert the forest of binomial trees in a binomial forest
				Node *binomial_forest[64] = { NULL }; // only one tree for each degree, it assume 64 > log(n) 
				while (true) {
					if (binomial_forest[forest->degree]) {
						// there is a binomial tree with the same degree as the current node (forest)
						auto bin_tree = binomial_forest[forest->degree];
						if (bin_tree == forest)
							break;

						// remove bin_tree of the current binomial forest because it must be merged
						binomial_forest[forest->degree] = NULL;
						bin_tree->prev->next = bin_tree->next;
						bin_tree->next->prev = bin_tree->prev;

						// merge the current node (forest) and bin_tree
						if (forest->key > bin_tree->key) {
							// add bin_tree as child of the current node (forest)
							bin_tree->prev = bin_tree->next = bin_tree;
							forest->degree++;
							forest->child = merge_forests(forest->child, bin_tree);
						}
						else {
							if (forest->next == forest)
								bin_tree->next = bin_tree->prev = bin_tree;
							else {
								forest->prev->next = bin_tree;
								forest->next->prev = bin_tree;
								bin_tree->next = forest->next;
								bin_tree->prev = forest->prev;
							}
							// add the current node (forest) as child of bin_tree
							forest->prev = forest->next = forest;
							bin_tree->degree++;
							bin_tree->child = merge_forests(bin_tree->child, forest);
							forest = bin_tree;
						}
					}
					else {
						binomial_forest[forest->degree] = forest;
						forest = forest->next;
					}
				}
			}

			//3- Calculate the new max, and update forest pointer
			auto current = forest, first = forest;
			current = current->next;
			while (current != first) {
				if (current->key > forest->key)
					forest = current;
				current = current->next;
			}

			delete current_max;
			this->n--;
			return result;
		}

		virtual vector<T> extract_all() {
			vector<T> result;
			result.reserve(this->n);
			while (this->n)
				result.push_back(extract_next());
			return result;
		}

		static FQ* meld(FQ *c1, FQ *c2) {
			auto result = new FQ();
			result->n = c1->n + c2->n;
			result->forest = merge_forests(c1->forest, c2->forest);
			return result;
		}

	private:
		static Node* merge_forests(Node *n1, Node *n2) {
			if (!n1)return n2;
			if (!n2)return n1;
			if (n1->key < n2->key) {
				auto n2_next = n2->next;
				auto n1_prev = n1->prev;
				n2->next = n1;
				n1->prev = n2;
				n2_next->prev = n1_prev;
				n1_prev->next = n2_next;
				return n2;
			}
			else {
				auto n1_next = n1->next;
				auto n2_prev = n2->prev;
				n1->next = n2;
				n2->prev = n1;
				n1_next->prev = n2_prev;
				n2_prev->next = n1_next;
				return n1;
			}
		}

		// circular double linked list bigining by the tree with the max element
		Node *forest;
	};
}