#pragma once

namespace priority_queue {
	template<typename T>
	class FibonacciHeapNode {
		typedef FibonacciHeapNode<T> Node;

	public:
		FibonacciHeapNode(T key) :key(key) {}

		T key;
		int degree = 0;
		Node *child = NULL, *prev, *next;
	};
}