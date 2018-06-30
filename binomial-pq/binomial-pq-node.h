#pragma once

namespace priority_queue {
	template<typename T>
	struct BinomialHeapNode {
		typedef BinomialHeapNode<T> Node;

		BinomialHeapNode(T key) :key(key) {	child = sibling = NULL; }

		T key;
		Node *child, *sibling;
	};
}