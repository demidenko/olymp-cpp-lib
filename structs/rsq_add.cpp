template<typename T>
struct rsq_add {
	vector<pair<T,T>> f;
	
	rsq_add(size_t sz): f(sz) {}
	
	void add_suf(size_t pos, const T &val){
		for(size_t i=pos; i<size(f); i|=i+1){
			f[i].first+=val;
			f[i].second+=val*pos;
		}
	}
	
	void add(size_t l, size_t r, const T &val){
		add_suf(l, val);
		add_suf(r, -val);
	}
	
	T sum_pref(int pos){
		T res = T();
		for(int i=pos; i>=0; i=(i&(i+1))-1) 
			res += f[i].first*(pos+1) - f[i].second;
		return res;
	}
	
	T operator()(int l, int r){
		if(l>=r) return T();
		return sum_pref(r-1) - sum_pref(l-1);
	}
	
	T operator[](int i){ return operator()(i,i+1); }
};