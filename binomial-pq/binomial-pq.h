#pragma once

#include "binomial-pq-node.h"
#include "../priority-queue.h"

using namespace std;

namespace priority_queue {
	template<typename T>
	class BinomialPQueue : public IPriorityQueue<T> {
		typedef BinomialPQueue<T> BQ;
		typedef BinomialHeapNode<T> Node;

	public:
		BinomialPQueue() {
			forest = new Node*[64];
			forest[0] = NULL;
			forest_size = 1;
		}

		~BinomialPQueue() {
			delete[] forest;
		}

		virtual void create(const vector<T> &data) {
			this->n = data.size();
			for (size_t i = 0; i < this->n; i++) {
				// create new tree withe the value
				auto carry = new Node(data[i]);

				// increment the forest
				int k = 0;
				while (k < forest_size) {
					if (forest[k]) {
						carry = merge_trees(carry, forest[k]);
						forest[k++] = NULL;
					}
					else {
						forest[k] = carry;
						carry = NULL;
						break;
					}
				}

				if (carry)
					forest[forest_size++] = carry;
			}
		}

		virtual void insert(const T &value) {
			// create new tree withe the value
			auto carry = new Node(value);

			// increment the forest
			int k = 0;
			while (k < forest_size) {
				if (forest[k]) {
					carry = merge_trees(carry, forest[k]);
					forest[k++] = NULL;
				}
				else {
					forest[k] = carry;
					carry = NULL;
					break;
				}
			}

			if (carry)
				forest[forest_size++] = carry;

			this->n++;
		}

		virtual T extract_next() {
			// find the tree with the maximum
			auto max = forest[0]; int max_id = 0;
			for (size_t i = 1; i < forest_size; i++) {
				if (forest[i] && (!max || forest[i]->key > max->key)) {
					max = forest[i];
					max_id = i;
				}
			}
			auto result = max->key;

			// remove the root of the max
			forest[max_id] = NULL;
			if (max_id == forest_size - 1)
				forest_size--;

			// get children of max
			int children_size = max_id--;
			auto children = new Node*[children_size];
			auto x = max->child;
			while (max_id >= 0) {
				children[max_id] = x;
				x = x->sibling;
				children[max_id--]->sibling = NULL;
			}

			// add forest of children of max
			sum_forest(children, children_size);

			delete[] children;
			delete max;
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

		static BQ* meld(BQ *c1, BQ *c2) {
			auto result = new BQ();
			result->forest = sum_forests(c1->forest, c1->forest_size, c2->forest, c2->forest_size, result->forest_size);
			result->n = c1->n + c2->n;
			return result;
		}

		static Node* merge_trees(Node *t1, Node *t2) {
			if (t1->key < t2->key) {
				t1->sibling = t2->child;
				t2->child = t1;
				return t2;
			}
			else {
				t2->sibling = t1->child;
				t1->child = t2;
				return t1;
			}
		}

		static Node** sum_forests(Node **f1, int f1_size, Node **f2, int f2_size, int &result_size) {
			auto result = new Node*[64];
			Node *carry[3];
			int k = 0, c = 0;
			while (k < f1_size && k < f2_size) {
				if (f1[k])
					carry[c++] = f1[k];
				if (f2[k])
					carry[c++] = f2[k];

				if (!c)
					// result has not a binomial tree in this position
					result[k] = NULL;
				else if (c == 1) {
					// move carry to result
					result[k] = carry[0];
					c = 0;
				}
				else if (c == 2) {
					// result has not a binomial tree in this position
					result[k] = NULL;

					// merge binomial trees in the carry
					auto merged = merge_trees(carry[0], carry[1]);

					// the merged tree is the carry now
					carry[0] = merged;
					c = 1;
				}
				else { // c == 3
					// move a tree of the carry to result
					result[k] = carry[2];

					// merge the other two binomials trees
					auto merged = merge_trees(carry[0], carry[1]);

					// the merged tree is the carry now
					carry[k] = merged;
					c = 1;
				}

				k++;
			}

			while (k < f1_size) {
				if (c) {
					if (f1[k]) {
						// result has not a binomial tree in this position
						result[k] = NULL;

						// merge binomial tree with the carry, the merged tree is the carry now
						carry[0] = merge_trees(carry[0], f1[k]);
						c = 1;
					}
					else {
						// move carry to result
						result[k] = carry[0];
						c = 0;
					}
				}
				else
					result[k] = f1[k];

				k++;
			}

			while (k < f2_size) {
				if (c) {
					if (f2[k]) {
						// result has not a binomial tree in this position
						result[k] = NULL;

						// merge binomial tree with the carry, the merged tree is the carry now
						carry[0] = merge_trees(carry[0], f2[k]);
						c = 1;
					}
					else {
						// move carry to result
						result[k] = carry[0];
						c = 0;
					}
				}
				else
					result[k] = f2[k];

				k++;
			}

			result_size = k;

			// move carry to result
			if (c)
				result[result_size++] = carry[0];

			return result;
		}

	private:
		void sum_forest(Node **f, int f_size) {
			Node *carry[3];
			int k = 0, c = 0;
			while (k < f_size) {
				if (forest[k])
					carry[c++] = forest[k];
				if (f[k])
					carry[c++] = f[k];

				if (c == 1) {
					// move carry to result
					forest[k] = carry[0];
					c = 0;
				}
				else if (c == 2) {
					// result has not a binomial tree in this position
					forest[k] = NULL;

					// merge binomial trees in the carry, the merged tree is the carry now
					carry[0] = merge_trees(carry[0], carry[1]);
					c = 1;
				}
				else { // c == 3
					// move a tree of the carry to result
					forest[k] = carry[2];

					// merge the other two binomials trees, the merged tree is the carry now
					carry[0] = merge_trees(carry[0], carry[1]);
					c = 1;
				}

				k++;
			}

			while (k < forest_size) {
				if (c) {
					if (forest[k]) {
						// result has not a binomial tree in this position
						forest[k] = NULL;

						// merge binomial tree with the carry, the merged tree is the carry now
						carry[0] = merge_trees(carry[0], forest[k]);
						c = 1;
					}
					else {
						// move carry to result
						forest[k] = carry[0];
						c = 0;
					}
				}

				k++;
			}

			// move carry to result
			if (c)
				forest[forest_size++] = carry[0];
		}

		Node **forest;
		int forest_size;
	};
}