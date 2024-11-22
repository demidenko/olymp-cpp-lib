template<class T, auto cmp = std::less<T>{} >
struct rmq {
	explicit rmq(size_t n = 0, const T &val = {}): d(n), t(d*2,val) {}
	rmq(const vector<T> &vals): d(size(vals)), t(d*2) {
		copy(begin(vals), end(vals), begin(t)+d);
		for(size_t i=d; i-->1;) t[i] = std::min(t[i*2], t[i*2+1], cmp);
	}
	void set_value(size_t i, const T &val) {
		for(t[i+=d]=val; i>1; i>>=1) t[i>>1] = std::min(t[i], t[i^1], cmp);
	}
	void update(size_t i, const T &val) { 
		set_value(i, std::min(t[i+d], val, cmp)); 
	}
	T operator()(size_t l, size_t r) const { assert(l < r);
		size_t p = l+=d;
		for(++l,r+=d; l<r; l>>=1,r>>=1) {
			if(l&1) { if(cmp(t[l], t[p])) p = l; ++l; }
			if(r&1) { --r; if(cmp(t[r], t[p])) p = r; }
		}
		return t[p];
	}
	const T& operator()() const { return t[1]; }
	const T& operator[](size_t i) const { return t[i+d]; }
	private:
	size_t d;
	vector<T> t;
};
