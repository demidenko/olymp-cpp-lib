template<class T>
struct rsq {
	explicit rsq(size_t n = 0): f(n) {}
	rsq(const std::ranges::range auto &vals): f(begin(vals), end(vals)) {
		for(size_t i=0, j, n=size(f); i<n; ++i) if(j=i|(i+1); j<n) f[j] += f[i];
	}
	void add(size_t i, const T &val) {
		for(; i < size(f); i|=i+1) f[i] += val;
	}
	T sum_until(size_t i) const {
		T s{}; for(; i--; i&=i+1) s += f[i];
		return s;
	}
	T operator()(size_t l, size_t r) const {
		return l < r ? sum_until(r) - sum_until(l) : T{};
	}
	T operator[](size_t i) const { return operator()(i, i+1); }
	private: vector<T> f;
};
