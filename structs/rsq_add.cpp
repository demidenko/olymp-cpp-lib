template<typename T>
struct rsq_add {
	rsq_add(size_t sz): f(sz) {}
	
	void add(size_t l, size_t r, const T &val) {
		if(l>=r) return ;
		add_suf(l, val);
		add_suf(r, -val);
	}
	
	T operator()(size_t l, size_t r) {
		if(l>=r) return T();
		return sum_until(r) - sum_until(l);
	}
	
	T operator[](size_t i){ return operator()(i,i+1); }
	
	private:
	vector<pair<T,T>> f;
	
	void add_suf(size_t pos, const T &val) {
		for(size_t i=pos; i<size(f); i|=i+1) {
			f[i].first+=val;
			f[i].second+=val*pos;
		}
	}
	
	T sum_until(size_t pos) {
		T res = T();
		for(size_t i=pos; i-->0; i&=i+1) 
			res += f[i].first*pos - f[i].second;
		return res;
	}
};