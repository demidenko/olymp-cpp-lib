template<class T, class A = T>
struct rsq_range_assign {
	explicit rsq_range_assign(size_t sz = 0): d(std::bit_ceil(sz)), t(d*2) {}
	
	rsq_range_assign(const std::ranges::range auto &vals): rsq_range_assign(size(vals)) {
		for(auto it=begin(t)+d; auto &&val : vals) it++->first = val;
		for(size_t i=d; i-->1; ) t[i].first = t[i*2].first + t[i*2+1].first;
	}
	
	void assign(size_t l, size_t r, const A &value) {
		if(l < r) assign(l, r, value, 0, d, 1);
	}
	
	T operator()(size_t l, size_t r) const {
		return l < r ? sum(l, r, 0, d, 1) : T{};
	}
	
	private:
	size_t d;
	vector<pair<T,optional<A>>> t;
	
	void assign(size_t i, size_t j, const A &val, size_t l, size_t r, size_t v) {
		if(i == l && j == r) {
			t[v] = {T(r-l) * val, val};
			return ;
		}
		size_t m = std::midpoint(l, r);
		if(auto &a = t[v].second) {
			t[v*2] = t[v*2+1] = {T(m-l) * *a, a};
			a.reset();
		}
		if(i < m) assign(i, std::min(j,m), val, l, m, v*2);
		if(m < j) assign(std::max(i,m), j, val, m, r, v*2+1);
		t[v].first = t[v*2].first + t[v*2+1].first;
	}
	
	T sum(size_t i, size_t j, size_t l, size_t r, size_t v) const {
		if(i == l && j == r) return t[v].first;
		if(auto a = t[v].second) return T(j-i) * *a;
		if(size_t m = std::midpoint(l, r); j <= m) return sum(i, j, l, m, v*2);
		else if(i >= m) return sum(i, j, m, r, v*2+1);
		else return sum(i, m, l, m, v*2) + sum(m, j, m, r, v*2+1);
	}
};
