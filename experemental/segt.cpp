template<typename T, T(*f)(const T&, const T&), T (*get_neutral)() >
struct segt {
	
	vector<T> t;
	size_t d;
	
	segt(size_t n, function<T(size_t)> get): d(n) {
		t.resize(d*2);
		for(size_t i=0;i<d;++i) t[i+d] = get(i);
		for(int i=d-1;i>0;--i) t[i] = f(t[i*2], t[i*2+1]);
	}
	
	void set_value(size_t i, const T& val){
		t[i+=d] = val;
		for(i>>=1; i; i>>=1) t[i] = f(t[i*2], t[i*2+1]);
	}
	
	T operator()(size_t l, size_t r){
		const static T neutral = get_neutral();
		T fl = neutral, fr = neutral;
		for(l+=d,r+=d; l<r; l>>=1,r>>=1){
			if(l&1) fl = f(fl, t[l]), ++l;
			if(r&1) --r, fr = f(t[r], fr);
		}
		return f(fl,fr);
	}
};