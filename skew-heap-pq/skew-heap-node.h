#pragma once

namespace priority_queue {
	template<typename T>
	struct SkewHeapNode {
		typedef SkewHeapNode<T> Node;

		SkewHeapNode(T key) :key(key) {	left = right = NULL; }

		T key;
		Node *left, *right;
	};
}