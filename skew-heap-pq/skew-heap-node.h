#pragma once

namespace priority_queue {
	template<typename T>
	struct SkewHeapNode {
		typedef SkewHeapNode<T> Node;

		SkewHeapNode(T key) :key(key) {	left = right = NULL; }

		void swap_children() {
			auto aux = left;
			left = right;
			right = aux;
		}

		T key;
		Node *left, *right;
	};
}