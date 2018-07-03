#pragma once

#include <vector>
#include <list>

using namespace std;

namespace priority_queue {
	template<typename T>
	class IPriorityQueue {
	public:
		
		virtual void create(const vector<T> &data) = 0;

		virtual void insert(const T &value) = 0;
		
		virtual T extract_next() = 0;
		
		virtual vector<T> extract_all() = 0;

		virtual int count() {
			return n;
		}
		
	protected:
		int n = 0;
	};
}