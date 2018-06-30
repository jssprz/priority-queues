#pragma once

#include "leftist-heap-node.h"
#include "../priority-queue.h"

using namespace std;

namespace priority_queue {
	template<typename T>
	class LeftistHeapPQueue :public IPriorityQueue<T> {
		typedef LeftistHeapPQueue<T> LH;
		typedef LeftistHeapNode<T> Node;
	public:
		LeftistHeapPQueue() :root(NULL) {}

		virtual void create(const vector<T> &data) {
			this->n = data.size();
			root = create_recursive(data, 0, this->n - 1);
		}

		virtual void insert(const T &value) {
            if(!root)
                root = new Node(value);
            else if (root->key < value) {
                auto new_node = new Node(value);
                new_node->left = root;
                root = new_node;
            }
            else if (!root->left)
                root->left = new Node(value);
            else {
                root->right = merge_trees(root->right, new Node(value));
                if (root->left->path_length < root->right->path_length)
                    root->swap_children();
                root->path_length = root->right->path_length + 1;
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

		static LH* meld(LH *c1, LH *c2) {
			auto result = new LH();
			result->n = c1->n + c2->n;
			result->root = merge_trees(c1->root, c2->root);
			return result;
		}

	private:
        /// Create a Skew Heap in O(n) with Divide and Conquer strategy
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
			if (n1->key < n2->key){
				if (!n2->left)
					n2->left = n1; //put the less as the left child of the greater
				else {
					n2->right = merge_trees(n2->right, n1); //merge right and the less node
					if (n2->left->path_length < n2->right->path_length) //the shortest path to a leaf throw the right is longest
						n2->swap_children(); //swap children to fix the leftist heap property
					n2->path_length = n2->right->path_length + 1; //update shortest path to leaf from the greater
				}
				return n2; //the root of the merge is the greater
			}
			else if (!n1->left)
				n1->left = n2; //put the less as the left child of the greater
			else {
				n1->right = merge_trees(n1->right, n2); //merge right and the less node
				if (n1->left->path_length < n1->right->path_length) //the shortest path to a leaf throw the right is longest
					n1->swap_children(); //swap children to fix the leftist heap property
				n1->path_length = n1->right->path_length + 1; //update shortest path to leaf from the greater
			}
			return n1; //the root of the merge is the greater
		}

		Node *root; //the root of the leftist heap
	};
}