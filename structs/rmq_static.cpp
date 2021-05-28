template<typename T, const T& f(const T&, const T&) = std::min<T> >
struct rmq {
	rmq() {}
	rmq(const vector<T> &vals) {
		size_t n = size(vals), L = 1;
		while((1<<L) <= n) ++L;
		t.resize(L); 
		t[0] = vals;
		for(size_t p=1, k=1; k<L; ++k, p<<=1){
			vector<T> &a = t[k], &b = t[k-1];
			a.resize(n-p*2+1);
			for(size_t i=0; i<size(a); ++i) a[i] = f(b[i], b[i+p]);
		}
	}
	const T& operator()(size_t l, size_t r) const {
		int h = 31 - __builtin_clz(r-l);
		return f(t[h][l], t[h][r-(1<<h)]);
	}
	private: vector<vector<T>> t;
};