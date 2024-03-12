struct binary_lifter {
	constexpr static size_t none = -1;
	
	binary_lifter(): binary_lifter(0) {}
	
	binary_lifter(const vector<size_t> &p): binary_lifter(size(p)) {
		const size_t n = size(p);
		for(size_t i=0; i<n; ++i) assert(p[i]==none || (0<=p[i] && p[i]<n && p[i]!=i)); //weak check
		jump[0] = p;
		for(size_t h=1; h<L; ++h)
		for(size_t i=0; i<n; ++i)
			if(size_t j=jump[h-1][i]; j!=none) jump[h][i] = jump[h-1][j];
	}
	
	struct result { size_t first_false, last_true, jumps_to_false; };
	result first_false(size_t i, auto &&pred) const {
		if(!pred(i)) return { i, none, 0 };
		size_t jumps = 0;
		for(size_t h = L; h--; )
		if(size_t j = jump[h][i]; j != none && pred(j)) {
			i = j;
			jumps += size_t(1) << h;
		}
		return { jump[0][i], i, jumps+1 };
	}
	
	size_t kth_jump(size_t i, size_t k) const {
		if(k >= (size_t(1) << L)) return none;
		for(size_t h=0; k>0 && i!=none; ++h, k>>=1) if(k&1) i = jump[h][i];
		return i; 
	}
	
	private:
	size_t L;
	vector<vector<size_t>> jump;
	
	binary_lifter(size_t n) {
		for(L = 1; (size_t(1)<<L) < n; ++L);
		jump.assign(L, vector(n, none));
	}
};
