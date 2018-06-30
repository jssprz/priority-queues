#pragma once

namespace priority_queue {
	template<typename T>
	struct FibonacciHeapNode {
		typedef FibonacciHeapNode<T> Node;

		FibonacciHeapNode(T key) :key(key) {}

		T key;
		int degree = 0;
		Node *child = NULL, *prev, *next;
	};
}