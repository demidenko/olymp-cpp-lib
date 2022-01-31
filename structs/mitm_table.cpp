template<class T>
struct mitm_table {
	mitm_table(const vector<auto> &vals): t(empty(vals) ? 1 : __lg(size(vals)) + 1) {
		t[0].assign(begin(vals), end(vals));
		t[0].resize(size_t(1) << size(t));
		for(size_t p=4, k=1; k<size(t); ++k, p<<=1) {
			auto &a = t[k] = t[0];
			for(size_t l=0; l<size(a); l+=p) {
				for(size_t i=l+(p>>1)-1; i>l; --i) a[i-1] = T(a[i-1], a[i]);
				for(size_t i=l+(p>>1); i+1<l+p; ++i) a[i+1] = T(a[i], a[i+1]);
			}
		}
	}
	T operator()(size_t l, size_t r) const {
		if(l >= r) return T{};
		if(l == --r) return t[0][l];
		size_t h = __lg(l^r);
		return T(t[h][l], t[h][r]);
	}
	private: vector<vector<T>> t;
};
