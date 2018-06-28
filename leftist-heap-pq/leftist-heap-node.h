#pragma once

namespace priority_queue {
	template<typename T>
	class LeftistHeapNode {
		typedef LeftistHeapNode<T> Node;
	public:
		LeftistHeapNode(T key) :key(key), path_length(0) {
			left = right = NULL;
		}

		void swap_children() {
			auto aux = left;
			left = right;
			right = aux;
		}

		T key;
		int path_length;
		Node *left, *right;
	};
}