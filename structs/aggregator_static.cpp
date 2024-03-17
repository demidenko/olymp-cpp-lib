template<class T>
struct aggregator_static {
	aggregator_static(const auto &vals): t(empty(vals) ? 1 : std::bit_width(size(vals))) {
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
		auto &row = t[std::bit_width(l^r)-1];
		return T(row[l], row[r]);
	}
	private: vector<vector<T>> t;
};
