template<typename T, T(*f)(const T&, const T&)>
struct segt {
	
	segt(size_t n, function<T(size_t)> gen) {
		for(d=1; d<n; d<<=1);
		t.assign(d*2, T());
		for(size_t i=0;i<n;++i) t[i+d] = gen(i);
		for(size_t i=d;i-->1;) t[i] = f(t[i*2], t[i*2+1]);
	}
	
	void set_value(size_t i, const T& val){
		t[i+=d] = val;
		for(i>>=1; i; i>>=1) t[i] = f(t[i*2], t[i*2+1]);
	}
	
	T operator()(size_t l, size_t r){
		const static T neutral = T();
		T fl = neutral, fr = neutral;
		for(l+=d,r+=d; l<r; l>>=1,r>>=1){
			if(l&1) fl = f(fl, t[l]), ++l;
			if(r&1) --r, fr = f(t[r], fr);
		}
		return f(fl,fr);
	}
	
	const T& operator()(){ return t[1]; }
	
	private:
	vector<T> t;
	size_t d;
};