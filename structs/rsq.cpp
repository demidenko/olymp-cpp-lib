template<typename T>
struct rsq {
	vector<T> f;
	rsq(size_t sz = 0): f(sz) {}
	template<typename C>
	rsq(const vector<C> &vals): f(size(vals)) {
		for(auto i = size(vals); i--;){
			f[i]+=vals[i];
			if(auto j = i&(i+1); j) f[j-1]+=f[i];
		}
	}
	T sum_suf(size_t i){
		T s = T();
		for(;i<size(f);i|=i+1) s+=f[i];
		return s;
	}
	T operator()(size_t l, size_t r){
		if(l>=r) return T();
		return sum_suf(l) - sum_suf(r);
	}
	void add(size_t i, const T &val){
		for(++i; i; i&=i+1) f[--i]+=val;
	}
};
