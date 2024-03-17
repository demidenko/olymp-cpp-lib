template<class T>
struct aggregator_static {
	aggregator_static(size_t n, auto &&gen): t(n ? std::bit_width(n) : 1) {
		t[0].resize(size_t(1) << size(t));
		for(size_t i=0; i<n; ++i) t[0][i] = gen(i);
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
		auto &row = t[std::bit_width(l^r)-1];
		return T(row[l], row[r]);
	}
	private: vector<vector<T>> t;
};
