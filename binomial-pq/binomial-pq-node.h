#pragma once

namespace priority_queue {
	template<typename T>
	class BinomialHeapNode {
		typedef BinomialHeapNode<T> Node;

	public:
		BinomialHeapNode(T key) :key(key) {
			child = sibling = NULL;
		}

		T key;
		Node *child, *sibling;
	};
}