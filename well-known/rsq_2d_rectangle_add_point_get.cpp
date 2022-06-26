template<class T, class P, template<class> class rsq>
struct rsq_2d_rectangle_add_point_get {
	rsq_2d_rectangle_add_point_get(vector<pair<P,P>> points): ys(size(points)) {
		for(size_t k=0; k<size(ys); ++k) ys[k] = points[k].second;
		sort(begin(ys), end(ys));
		t.resize(unique_sz(ys));
		sort(begin(points), end(points));
		for(auto [x, y] : points)
		for(size_t v=lb(ys,y); v<size(t); v|=v+1) t[v].first.push_back(x);
		for(auto &[xs, f] : t) f = rsq<T>(unique_sz(xs));
	}
	
	T get_value(const P &x, const P &y) const {
		T res{};
		for(size_t v=lb(ys, y); v<size(ys); v|=v+1) 
			res += t[v].second(lb(t[v].first, x), size(t[v].first));
		return res;
	}
	
	void add(const P &xl, const P &xr, const P &yl, const P &yr, const T &val) {
		if(xl < xr && yl < yr) add(xl, xr, yr, val), add(xl, xr, yl, -val);
	}
	
	private:
	void add(const P &xl, const P &xr, const P &yr, const T &val) {
		for(size_t v = lb(ys, yr); v--; v&=v+1)
		if(size_t l = lb(t[v].first, xl), r = lb(t[v].first, xr); l < r) {
			if(l) t[v].second.add(l-1, -val);
			t[v].second.add(r-1, val);
		}
	}
	
	vector<P> ys;
	vector<pair<vector<P>,rsq<T>>> t;
	static size_t unique_sz(auto&&v) { size_t sz = unique(begin(v), end(v)) - begin(v); v.resize(sz); return sz; }
	static inline size_t lb(auto&&v, auto&&val) { return lower_bound(begin(v), end(v), val) - begin(v); }
};
