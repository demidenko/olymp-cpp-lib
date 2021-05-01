template<typename T, const T&(*f)(const T&, const T&) = std::min<T> >
struct rmq {
	rmq(size_t sz = 0, const T &val = T()): t(sz*2,val), d(sz) {}
	rmq(const vector<T> &vals): d(size(vals)) {
		t.resize(d*2);
		copy(begin(vals), end(vals), begin(t)+d);
		for(size_t i=d; i-->1;) t[i] = f(t[i*2], t[i*2+1]);
	}
	
	void set_value(size_t i, const T& val) {
		for(t[i+=d]=val; i>1; i>>=1) t[i>>1] = f(t[i], t[i^1]);
	}
	
	void update(size_t i, const T& val) {
		set_value(i, f(t[i+d],val));
	}
	
	T operator()(size_t l, size_t r) {
		T res = t[l+=d];
		for(++l,r+=d; l<r; l>>=1,r>>=1){
			if(l&1) res = f(res, t[l]), ++l;
			if(r&1) --r, res = f(res, t[r]);
		}
		return res;
	}
	
	T operator()(){ return t[1]; }
	
	private:
	vector<T> t;
	size_t d;
};