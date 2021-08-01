template<class T>
struct rsq {
	explicit rsq(size_t sz = 0): f(sz) {}
	rsq(const vector<auto> &vals): f(begin(vals),end(vals)) {
		for(auto i = size(vals); i--;) if(auto j = i&(i+1); j) f[j-1]+=f[i];
	}
	void add(size_t i, const T &val) {
		for(++i; i--; i&=i+1) f[i]+=val;
	}
	T operator()(size_t l, size_t r) const {
		if(l>=r) return T();
		return sum_suf(l) - sum_suf(r);
	}
	T operator[](size_t i) const { return sum_suf(i) - sum_suf(i+1); }
	T sum_suf(size_t i) const {
		T s{};
		for(; i<size(f); i|=i+1) s+=f[i];
		return s;
	}
	private: vector<T> f;
};
