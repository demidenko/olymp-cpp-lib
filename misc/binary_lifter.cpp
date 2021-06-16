struct binary_lifter {
	constexpr static inline size_t none = -1;
	
	binary_lifter() { init(0); }
	
	binary_lifter(const vector<size_t> &p) {
		size_t n = size(p);
		for(size_t i=0;i<n;++i) assert(p[i] == none || (0<=p[i] && p[i]<n && p[i]!=i)); //weak check
		init(n);
		jump[0] = p;
		for(size_t h=1; h<L; ++h)
			for(size_t i=0; i<n; ++i)
				if(size_t j=jump[h-1][i]; j!=none) jump[h][i] = jump[h-1][j];
	}
	
	void push_back(size_t parent) {
		const size_t cur = count();
		assert(parent == none || parent < cur);
		size_t j = jump[0].emplace_back(parent);
		for(size_t h=1; h<L; ++h) {
			size_t &p = jump[h].emplace_back(none);
			if(j!=none) p = jump[h-1][j];
			j = p;
		}
		if(j!=none && jump[L-1][j]!=none) {
			jump.emplace_back(cur+1, none);
			for(size_t i=0; i<=cur; ++i) 
				if(size_t j = jump[L-1][i]; j!=none)
					jump[L][i] = jump[L-1][j];
			++L;
		}
	}
	
	struct result { size_t first_fail, last_nonfail, count_of_jumps; };
	result first_fail(size_t i, const function<bool(size_t)> &is_fail) const {
		if(is_fail(i)) return { i, none, 0 };
		size_t jumps = 0;
		for(size_t h=L; h--;){
			size_t j = jump[h][i];
			if(j!=none && !is_fail(j)) i = j, jumps+=size_t(1)<<h;
		}
		return { jump[0][i], i, jumps+1 };
	}
	
	size_t kth_jump(size_t i, size_t k) const {
		if(k >= size_t(1)<<L) return none;
		for(size_t h=0; k>0 && i!=none; ++h, k>>=1) if(k&1) i = jump[h][i];
		return i; 
	}
	
	size_t count() const { return size(jump[0]); }
	
	private:
	size_t L;
	vector<vector<size_t>> jump;
	
	void init(size_t n) {
		for(L = 1; size_t(1)<<L < n; ++L);
		jump.assign(L, vector(n,none));
	}
};