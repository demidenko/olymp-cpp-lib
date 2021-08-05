template<class T>
struct compressor {
	compressor(){}
	compressor(const vector<T> &values): v(values) { make(); }
	void make() {
		sort(begin(v),end(v));
		v.resize(unique(begin(v),end(v))-begin(v));
	}
	void add(const T &x) { v.push_back(x); }
	size_t lb(auto &&x) const { return lower_bound(begin(v),end(v),x) - begin(v); }
	size_t ub(auto &&x) const { return upper_bound(begin(v),end(v),x) - begin(v); }
	size_t index(auto &&x) const {
		size_t i = lb(x);
		assert(v.at(i) == x);
		return i;
	}
	auto range(auto &&__begin, auto &&__end) const { return pair(lb(__begin),lb(__end)); }
	size_t size() const { return v.size(); }
	private: vector<T> v;
};
