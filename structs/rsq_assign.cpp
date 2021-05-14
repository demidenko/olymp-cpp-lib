template<typename T, typename A = T>
struct rsq_assign {
	
	template<typename C>
	rsq_assign(const vector<C> &vals) {
		for(d=2; d<size(vals); d<<=1);
		t.assign(d*2, pair(T(), nullopt));
		for(size_t i=0; i<size(vals); ++i) t[i+d].first = vals[i];
		_build();
	}
	
	void assign(size_t l, size_t r, const A &value){
		if(r>d) r = d;
		if(l<r) _assign(l,r,value,0,d,1);
	}
	
	T operator()(size_t l, size_t r){
		if(r>d) r = d;
		T buf = T();
		if(l<r) _calc(l,r,0,d,1,buf);
		return buf;
	}
	
	pair<size_t, T> take(size_t start, T limit){
		limit+=operator()(0,start);
		size_t v = 1, l = 0, r = d;
		if(t[v].first<=limit) return {r, limit-t[v].first};
		for(;v<d;){
			if(auto a = t[v].second) return {size_t(limit / *a) + l, limit % *a};
			v<<=1;
			if(size_t m = (l+r)/2; t[v].first<=limit){
				limit-=t[v].first;
				++v;
				l = m;
			} else r = m;
		}
		return {l, limit};
	}
	
	private:
	size_t d;
	vector<pair<T,optional<A>>> t;
	
	void _build() {
		for(size_t i=d; i-->1; ) t[i].first = t[i*2].first + t[i*2+1].first;
	}
	
	void _push(size_t v, size_t len) {
		if(auto &a = t[v].second){
			t[v*2] = t[v*2+1] = {T(len>>1) * *a, a};
			a.reset();
		}
	}
	
	void _assign(size_t i, size_t j, const A &val, size_t l, size_t r, size_t v){
		if(i==l && j==r){
			t[v] = {T(r-l) * val, val};
			return ;
		}
		_push(v,r-l);
		size_t m = (l+r)>>1;
		if(i<m) _assign(i,min(j,m),val,l,m,v*2);
		if(m<j) _assign(max(i,m),j,val,m,r,v*2+1);
		t[v].first = t[v*2].first + t[v*2+1].first;
	}
	
	void _calc(size_t i, size_t j, size_t l, size_t r, size_t v, T &buf){
		if(i==l && j==r) buf+=t[v].first; else
		if(auto a = t[v].second; !a){
			size_t m = (l+r)>>1;
			if(i<m) _calc(i,min(j,m),l,m,v*2,buf);
			if(m<j) _calc(max(i,m),j,m,r,v*2+1,buf);
		} else buf+= T(j-i) * *a;
	}
};