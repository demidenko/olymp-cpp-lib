template<class T, auto cmp = std::less<T>{}, class A = T >
struct rmq_range_add {
	explicit rmq_range_add(size_t sz = 0): d(std::bit_ceil(sz)), t(d*2) {}
	
	rmq_range_add(const std::ranges::range auto &vals): rmq_range_add(size(vals)) {
		for(auto it=begin(t)+d; auto &&val : vals) it++->first = val;
		for(size_t i=d; i-->1; ) t[i].first = std::min(t[i*2].first, t[i*2+1].first, cmp);
	}
	
	void add(size_t l, size_t r, const A &val) {
		if(l < r) add(l, r, val, 0, d, 1);
	}
	
	T operator()(size_t l, size_t r) const {
		assert(l < r);
		return min(l, r, 0, d, 1);
	}
	
	T operator[](size_t i) const {
		T result = t[i+=d].first;
		while(i>>=1) result += t[i].second;
		return result;
	}
	
	private:
	size_t d;
	vector<pair<T,A>> t;
	
	void add(size_t i, size_t j, const A &val, size_t l, size_t r, size_t v) {
		if(i == l && j == r) {
			t[v].first += val;
			t[v].second += val;
			return ;
		}
		size_t m = std::midpoint(l, r);
		if(i < m) add(i, std::min(j,m), val, l, m, v*2);
		if(m < j) add(std::max(i,m), j, val, m, r, v*2+1);
		t[v].first = std::min(t[v*2].first, t[v*2+1].first, cmp) + t[v].second;
	}
	
	T min(size_t i, size_t j, size_t l, size_t r, size_t v) const {
		if(i == l && j == r) return t[v].first;
		size_t m = std::midpoint(l, r);
		return (j<=m ? min(i, j, l, m, v*2) : i>=m ? min(i, j, m, r, v*2+1)
		 : std::min(min(i, m, l, m, v*2), min(m, j, m, r, v*2+1))) + t[v].second;
	}
};
