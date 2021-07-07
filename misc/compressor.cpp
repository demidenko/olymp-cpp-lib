template<class T>
struct compressor {
	compressor(){}
	compressor(const vector<T> &values): v(values) { make(); }
	void make() {
		sort(begin(v),end(v));
		v.resize(unique(begin(v),end(v))-begin(v));
	}
	void add(const T &x) { v.push_back(x); }
	size_t lb(const T &x) const { return lower_bound(begin(v),end(v),x) - begin(v); }
	size_t ub(const T &x) const { return upper_bound(begin(v),end(v),x) - begin(v); }
	size_t index(const T &x) const {
		size_t i = lb(x);
		assert(v.at(i) == x);
		return i;
	}
	auto range(const T &from, const T &to) const { return pair(lb(from),lb(to)); }
	size_t size() const { return v.size(); }
	private: vector<T> v;
};
