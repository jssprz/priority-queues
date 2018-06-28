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
			root = merge_trees(root, new Node(value));
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
			if (n1->key < n2->key)
				swap(n1, n2);
			n1->swap_children();
			n1->left = merge_trees(n2, n1->left);
			return n1;
		}

		static void swap(Node *&n1, Node *&n2) {
			auto aux = n1;
			n1 = n2;
			n2 = aux;
		}

		Node *root;
	};
}