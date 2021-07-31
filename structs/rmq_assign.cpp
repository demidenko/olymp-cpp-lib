template<class T, const T& f(const T&, const T&) = std::min<T> >
struct rmq_assign {
	constexpr static T neutral = [](T m,T M){return f(m,M)==m?M:m;}(numeric_limits<T>::min(),numeric_limits<T>::max());
	
	explicit rmq_assign(size_t sz, const T &val = neutral) {
		for(d=1; d<sz; d<<=1);
		t.assign(d*2, {val, false});
	}
	
	rmq_assign(const vector<auto> &vals): rmq_assign(size(vals)) {
		for(size_t i=0; i<size(vals); ++i) t[i+d].first = vals[i];
		_build();
	}
	
	void assign(size_t l, size_t r, const T &value) {
		if(r>d) r = d;
		if(l<r) _assign(l,r,value,0,d,1);
	}
	
	T operator()(size_t l, size_t r) const {
		if(r>d) r = d;
		T buf = neutral;
		if(l<r) _calc(l,r,0,d,1,buf);
		return buf;
	}
	
	private:
	size_t d;
	vector<pair<T,bool>> t;
	
	void _build() {
		for(size_t i=d; i-->1; ) _build_node(i);
	}
	
	inline void _build_node(size_t v) {
		t[v].first = f(t[v*2].first, t[v*2+1].first);
	}
	
	void _push(size_t v) {
		if(t[v].second) {
			t[v*2] = t[v*2+1] = t[v];
			t[v].second = false;
		}
	}
	
	void _assign(size_t i, size_t j, const T &val, size_t l, size_t r, size_t v) {
		if(i==l && j==r) {
			t[v] = {val, true};
			return ;
		}
		_push(v);
		size_t m = (l+r)>>1;
		if(i<m) _assign(i,min(j,m),val,l,m,v*2);
		if(m<j) _assign(max(i,m),j,val,m,r,v*2+1);
		t[v].first = f(t[v*2].first, t[v*2+1].first);
	}
	
	void _calc(size_t i, size_t j, size_t l, size_t r, size_t v, T &buf) const {
		if(i==l && j==r || t[v].second) buf = f(buf, t[v].first);
		else {
			size_t m = (l+r)>>1;
			if(i<m) _calc(i,min(j,m),l,m,v*2,buf);
			if(m<j) _calc(max(i,m),j,m,r,v*2+1,buf);
		}
	}
};
