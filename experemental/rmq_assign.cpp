template<typename T, const T&(*f)(const T&, const T&) = std::min<T> >
struct rmq_assign {
	constexpr static T neutral = f(numeric_limits<T>::min(), numeric_limits<T>::max()) ^ numeric_limits<T>::min() ^ numeric_limits<T>::max();
	
	template<typename C>
	rmq_assign(const vector<C> &vals, const T&_u=T()) : undefined(_u) {
		for(d=2; d<size(vals); d<<=1);
		t.assign(d*2, pair(neutral, undefined));
		for(size_t i=0; i<size(vals); ++i) t[i+d].first = vals[i];
		_build();
	}
	
	void assign(size_t l, size_t r, const T &value){
		assert(value!=undefined);
		if(r>d) r = d;
		if(l<r) _assign(l,r,value,0,d,1);
	}
	
	T operator()(size_t l, size_t r){
		if(r>d) r = d;
		T buf = neutral;
		if(l<r) _calc(l,r,0,d,1,buf);
		return buf;
	}
	
	private:
	
	size_t d;
	const T undefined;
	vector<pair<T,T>> t;
	
	void _build(){
		for(size_t i=d; i-->1; ) t[i].first = f(t[i*2].first, t[i*2+1].first);
	}
	
	void _push(size_t v, size_t len){
		if(T &a = t[v].second; a!=undefined){
			t[v*2].second = t[v*2+1].second = a;
			t[v*2].first = t[v*2+1].first = a;
			a = undefined;
		}
	}
	
	void _assign(size_t i, size_t j, const T &val, size_t l, size_t r, size_t v){
		if(i==l && j==r){
			t[v].second = val;
			t[v].first = val;
			return ;
		}
		_push(v,r-l);
		size_t m = (l+r)>>1;
		if(i<m) _assign(i,min(j,m),val,l,m,v*2);
		if(m<j) _assign(max(i,m),j,val,m,r,v*2+1);
		t[v].first = f(t[v*2].first, t[v*2+1].first);
	}
	
	void _calc(size_t i, size_t j, size_t l, size_t r, size_t v, T &buf){
		if(i==l && j==r) buf = f(buf, t[v].first); else
		if(t[v].second==undefined){
			size_t m = (l+r)>>1;
			if(i<m) _calc(i,min(j,m),l,m,v*2,buf);
			if(m<j) _calc(max(i,m),j,m,r,v*2+1,buf);
		}else buf = f(buf, t[v].second);
	}
	
};