template<class T, const T& f(const T&, const T&) = std::min<T> >
struct rmq_range_assign {
	explicit rmq_range_assign(size_t sz = 0): d(std::bit_ceil(sz)), t(d*2) {}
	
	rmq_range_assign(const std::ranges::range auto &vals): rmq_range_assign(size(vals)) {
		for(auto it=begin(t)+d; auto &&val : vals) it++->first = val;
		for(size_t i=d; i-->1; ) t[i].first = f(t[i*2].first, t[i*2+1].first);
	}
	
	void assign(size_t l, size_t r, const T &value) {
		if(l < r) assign(l, r, value, 0, d, 1);
	}
	
	T operator()(size_t l, size_t r) const {
		assert(l < r);
		return calc(l, r, 0, d, 1);
	}
	
	T operator[](size_t i) const {
		T result = t[i+=d].first;
		while(i>>=1) if(t[i].second) result = t[i].first;
		return result;
	}
	
	private:
	size_t d;
	vector<pair<T,bool>> t;
	
	void assign(size_t i, size_t j, const T &val, size_t l, size_t r, size_t v) {
		if(i == l && j == r) {
			t[v] = {val, true};
			return ;
		}
		if(t[v].second) {
			t[v*2] = t[v*2+1] = t[v];
			t[v].second = false;
		}
		size_t m = std::midpoint(l, r);
		if(i < m) assign(i, std::min(j,m), val, l, m, v*2);
		if(m < j) assign(std::max(i,m), j, val, m, r, v*2+1);
		t[v].first = f(t[v*2].first, t[v*2+1].first);
	}
	
	T calc(size_t i, size_t j, size_t l, size_t r, size_t v) const {
		if((i == l && j == r) || t[v].second) return t[v].first;
		if(size_t m = std::midpoint(l, r); j <= m) return calc(i, j, l, m, v*2);
		else if(i >= m) return calc(i, j, m, r, v*2+1);
		else return f(calc(i, m, l, m, v*2), calc(m, j, m, r, v*2+1));
	}
};
