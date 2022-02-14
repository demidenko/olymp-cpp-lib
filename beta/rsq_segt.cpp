template<class T, bool has_minus = true>
struct rsq {
	explicit rsq(size_t n = 0): d(n), t(d*2) {}
	rsq(const vector<auto> &vals): rsq(size(vals)) {
		copy(begin(vals), end(vals), begin(t)+d);
		for(size_t i=d; i-->1;) t[i] = t[i*2]+t[i*2+1];
	}
	void add(size_t i, const T &val) {
		for(i+=d; i; i>>=1) t[i] += val;
	}
	void set_value(size_t i, const T &val) {
		if constexpr (has_minus) add(i, val - t[i+d]);
		else for(t[i+=d]=val; i>1; i>>=1) t[i>>1] = t[i] + t[i^1];
	}
	T operator()(size_t l, size_t r) const {
		T s{};
		for(l+=d, r+=d; l<r; l>>=1, r>>=1) {
			if(l&1) s+=t[l], ++l;
			if(r&1) --r, s+=t[r];
		}
		return s;
	}
	const T& operator()() const { return t[1]; }
	const T& operator[](size_t i) const { return t[i+d]; }
	private:
	size_t d;
	valarray<T> t;
};
