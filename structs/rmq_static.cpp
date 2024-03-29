template<class T, auto cmp = std::less<T>{}>
struct rmq {
	rmq() {}
	rmq(const vector<T> &vals): t(std::bit_width(size(vals))) {
		if(!empty(t)) t[0] = vals;
		for(size_t k=1, p=1; k<size(t); ++k, p<<=1) {
			auto &a = t[k], &b = t[k-1];
			a.resize(size(vals)-p*2+1);
			for(size_t i=0; i<size(a); ++i) a[i] = std::min(b[i], b[i+p], cmp);
		}
	}
	const T& operator()(size_t l, size_t r) const {
		auto h = std::bit_width(r-l) - 1;
		return std::min(t[h][l], t[h][r-(1uz<<h)], cmp);
	}
	private: vector<vector<T>> t;
};
