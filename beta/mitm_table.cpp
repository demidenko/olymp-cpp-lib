template<class T>
struct mitm_table {
	mitm_table(const vector<auto> &vals) {
		size_t n = size(vals), L = n ? __lg(n) + 1 : 1;
		size_t d = size_t(1)<<L;
		
		t.resize(L);
		auto &v = t[0];
		v.assign(d, neutral);
		copy(begin(vals), end(vals), begin(v));
		
		for(size_t p=4, k=1; k<L; ++k, p<<=1) {
			auto &a = t[k];
			a.assign(begin(v), end(v));
			for(size_t l = 0; l<d; l+=p) {
				size_t mid = l+(p>>1);
				for(size_t i=mid-1; i>l; --i) a[i-1] = T(a[i-1], a[i]);
				for(size_t i=mid; i+1<l+p; ++i) a[i+1] = T(a[i], a[i+1]);
			}
		}
	}
	
	T operator()(size_t l, size_t r) const {
		if(l>=r) return neutral;
		if(l == --r) return t[0][l];
		auto h = __lg(l^r);
		return T(t[h][l], t[h][r]);
	}
	
	inline const static T neutral{};
	private: vector<vector<T>> t;
};
