template<class T, class P>
struct rsq_2d_point_add_rectangle_sum {
	rsq_2d_point_add_rectangle_sum(vector<pair<P,P>> points): ys(size(points)) {
		for(size_t k=0; k<size(ys); ++k) ys[k] = points[k].second;
		sort(begin(ys), end(ys));
		make_unique(ys);
		t.resize(size(ys));
		sort(begin(points), end(points));
		for(auto [x, y] : points)
		for(size_t v=lb(ys,y); v<size(t); v|=v+1) t[v].first.push_back(x);
		for(auto &[xs, f] : t) {
			make_unique(xs);
			f = rsq<T>(size(xs));
		}
	}
	
	T get_sum(const P &xl, const P &xr, const P &yl, const P &yr) {
		if(xl>=xr || yl>=yr) return T{};
		return get_sum(xl, xr, yr) - get_sum(xl, xr, yl);
	}
	
	void add(const P &x, const P &y, const T &val) {
		for(size_t v=lb(ys, y); v<size(ys); v|=v+1) 
			t[v].second.add(lb(t[v].first, x), val);
	}
	
	private:
	T get_sum(const P &xl, const P &xr, const P &yr) {
		T res{};
		for(size_t v = lb(ys, yr); v--; v&=v+1)
		if(size_t l = lb(t[v].first, xl), r = lb(t[v].first, xr); l < r)
			res += t[v].second(l, r);
		return res;
	}
	
	vector<pair<vector<P>,rsq<T>>> t;
	vector<P> ys;
	static void make_unique(vector<P> &v) { v.resize(unique(begin(v),end(v))-begin(v)); }
	static inline size_t lb(const vector<P> &v, const P &y) { return lower_bound(begin(v), end(v), y) - begin(v); }
};
