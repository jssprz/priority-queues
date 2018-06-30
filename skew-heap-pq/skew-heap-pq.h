#pragma once

#include "skew-heap-node.h"
#include "../priority-queue.h"

using namespace std;

namespace priority_queue {
	template<typename T>
	class SkewHeapPQueue :public IPriorityQueue<T>
	{
		typedef SkewHeapPQueue<T> SH;
		typedef SkewHeapNode<T> Node;
	public:
		SkewHeapPQueue() :root(NULL) {}

		virtual void create(const vector<T> &data) {
			this->n = data.size();
			root = create_recursive(data, 0, this->n - 1);
		}

		virtual void insert(const T &value) {
		    if (!root)
		        root = new Node(value);
		    else if (value < root->key) {
                root->swap_children();
                root->left = merge_trees(new Node(value), root->left);
            }
            else {
		        auto new_node = new Node(value);
                new_node->left = root;
                root = new_node;
            }
			this->n++;
		}

		virtual T extract_next() {
			T result = root->key;
			auto old = root;
			root = merge_trees(root->left, root->right);
			delete old;
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

		static SH* meld(SH *c1, SH *c2) {
			auto result = new SH();
			result->n = c1->n + c2->n;
			result->root = merge_trees(c1->root, c2->root);
			return result;
		}

	private:
		Node* create_recursive(const vector<T> &data, int min, int max) {
			if (min > max)return NULL;
			else if (min == max)
				return new Node(data[min]);
			else {
				int mid = (min + max) / 2;
				return merge_trees(create_recursive(data, min, mid), create_recursive(data, mid + 1, max));
			}
		}

		static Node* merge_trees(Node *n1, Node *n2) {
			if (!n1)return n2;
			if (!n2)return n1;
			if (n1->key < n2->key) {
				n2->swap_children();
				n2->left = merge_trees(n1, n2->left);
				return n2;
			}
			else {
				n1->swap_children();
				n1->left = merge_trees(n2, n1->left);
				return n1;
			}
		}

		Node *root;
	};
}