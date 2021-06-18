template<typename T>
struct compressor {
	compressor(const vector<T> &values): x(values) {
		sort(ALL(x));
		x.resize(unique(ALL(x))-begin(x));
	}
	size_t index(const T &value) {
		size_t i = lb(value);
		assert(x.at(i) == value);
		return i;
	}
	size_t greater(const T &value) { return ub(value); }
	size_t greater_equal(const T &value) { return lb(value); }
	size_t size() const { return x.size(); }
	private: vector<T> x;
	size_t lb(const T &value) { return lower_bound(ALL(x),value) - begin(x); }
	size_t ub(const T &value) { return upper_bound(ALL(x),value) - begin(x); }
};
