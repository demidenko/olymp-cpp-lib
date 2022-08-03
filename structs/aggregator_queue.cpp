template<class T>
struct aggregator_queue {
	aggregator_queue(): rs{} {}
	void push(const T &x) {
		rs = T(rs, r.emplace_back(x));
	}
	void pop() {
		if(empty(l)) {
			for(T m{}; !empty(r); ) {
				l.emplace_back(r.back(), m);
				r.pop_back();
			}
			rs = {};
		}
		l.pop_back();
	}
	T operator()() {
		return empty(l) ? rs : T(l.back(), rs);
	}
	size_t size() const { return l.size() + r.size(); }
	private:
	vector<T> l, r;
	T rs;
};
