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
			// insert each element
			for (typename vector<T>::const_iterator it = data.begin() + 1; it != end_it; ++it) {
				// create new tre with one only node with the value
				auto new_node = new Node(*it);

				// insert the new node in the list
				forest->prev->next = new_node;
				new_node->prev = forest->prev;
				forest->prev = new_node;
				new_node->next = forest;

				// update the node with the maximum value
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

            // create new tre with one only node with the value
            auto new_node = new Node(value);

			// insert the new node in the list
            forest->prev->next = new_node;
			new_node->prev = forest->prev;
			forest->prev = new_node;
			new_node->next = forest;

            // update the node with the maximum value
            if (forest->key < value)
				forest = new_node;

			this->n++;
		}

		virtual T extract_next() {
			auto old_max = forest;
			auto result = old_max->key;

			if (forest->next == forest) { //if forest has only one node
				forest = forest->child;
				if (!forest) {
					this->n = 0;
					return result;
				}
			}
			else {
                //1- Remove root of the tree that contains the max
				forest->next->prev = forest->prev;
				forest->prev->next = forest->next;

				//2- Add children to the forest
                forest = merge_forests(forest->next, forest->child);

				//3- Convert the forest of binomial trees in a binomial forest
				Node *binomial_forest[64] = { NULL }; // only one tree for each degree, it assumes 64 > log(n)
				while (true) {
					if (binomial_forest[forest->degree]) {
						// there is a binomial tree with the same degree as the current node (forest)
						auto bin_tree = binomial_forest[forest->degree];
						if (bin_tree == forest)
							break;

						//3.1- remove bin_tree of the current binomial forest because it must be merged
						binomial_forest[forest->degree] = NULL;
						bin_tree->prev->next = bin_tree->next;
						bin_tree->next->prev = bin_tree->prev;

						//3.2- merge the current node (forest) and bin_tree
						if (forest->key > bin_tree->key) {
							// add bin_tree as child of the current node (forest)
							forest->degree++;
							if(!forest->child) {
                                forest->child = bin_tree;
                                bin_tree->next = bin_tree->prev = bin_tree;
                            }
							else{
							    auto child_next = forest->child->next;
							    forest->child->next = bin_tree;
							    bin_tree->prev = forest->child;
							    bin_tree->next = child_next;
							    child_next->prev = bin_tree;
							}
						}
                        else {
						    // change the current node (forest) for bin_tree in the root list
                            forest->prev->next = bin_tree;
                            forest->next->prev = bin_tree;
                            bin_tree->next = forest->next;
                            bin_tree->prev = forest->prev;

							// add the current node (forest) as child of bin_tree
							bin_tree->degree++;
                            if(!bin_tree->child) {
                                bin_tree->child = forest;
                                forest->next = forest->prev = forest;
                            }
                            else{
                                auto child_next = bin_tree->child->next;
                                bin_tree->child->next = forest;
                                forest->prev = bin_tree->child;
                                forest->next = child_next;
                                child_next->prev = forest;
                            }
							forest = bin_tree;
						}
					}
					else {
					    // the current node (forest) is the only node with its degree in this moment
						binomial_forest[forest->degree] = forest;
						forest = forest->next;
					}
				}
            }

            //4- Calculate the new max, and update forest pointer
            auto current = forest->next, first = forest;
            while (current != first) {
                if (current->key > forest->key)
                    forest = current;
                current = current->next;
            }

			delete old_max;
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

            auto n1_next = n1->next;
            auto n2_prev = n2->prev;
            n1->next = n2;
            n2->prev = n1;
            n1_next->prev = n2_prev;
            n2_prev->next = n1_next;

            return (n1->key < n2->key) ? n2 : n1;
		}

		// circular double linked list bigining by the tree with the max element
		Node *forest;
	};
}