template<class T, const T& f(const T&, const T&) = std::min<T> >
struct rmq {
	rmq() {}
	rmq(const vector<T> &vals): t(empty(vals) ? 1 : __lg(size(vals)) + 1) {
		t[0] = vals;
		for(size_t k=1, p=1; k<size(t); ++k, p<<=1) {
			auto &a = t[k], &b = t[k-1];
			a.resize(size(vals)-p*2+1);
			for(size_t i=0; i<size(a); ++i) a[i] = f(b[i], b[i+p]);
		}
	}
	const T& operator()(size_t l, size_t r) const {
		auto h = __lg(r-l);
		return f(t[h][l], t[h][r-(size_t(1)<<h)]);
	}
	private: vector<vector<T>> t;
};
