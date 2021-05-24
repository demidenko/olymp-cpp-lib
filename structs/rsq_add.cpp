template<typename T>
struct rsq_add {
	rsq_add(size_t sz): f(sz) {}
	
	rsq_add(const vector<auto> &vals): f(size(vals)) {
		for(size_t i=0; i<size(vals); ++i) {
			T x = i ? T(vals[i]) - T(vals[i-1]) : vals[i];
			f[i].first+=x;
			f[i].second+=x*T(i);
			if(size_t j = i|(i+1); j<size(f)){
				f[j].first+=f[i].first;
				f[j].second+=f[i].second;
			}
		}
	}
	
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
			f[i].second+=val*T(pos);
		}
	}
	
	T sum_until(size_t pos) {
		T res = T();
		for(size_t i=pos; i-->0; i&=i+1) 
			res += f[i].first*T(pos) - f[i].second;
		return res;
	}
};