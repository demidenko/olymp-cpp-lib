struct binary_lifter {
	binary_lifter(const vector<size_t> &p): n(size(p)) {
		for(size_t i=0;i<n;++i) assert(0<=p[i] && p[i]<=n && p[i]!=i); //weak check
		for(L = 1; (1<<L)<n; ++L);
		jump.assign(L, vector(n+1,n));
		copy(ALL(p), begin(jump[0]));
		jump[0][n] = n;
		for(size_t h=1;h<L;++h){
			for(size_t i=0;i<=n;++i) jump[h][i] = jump[h-1][jump[h-1][i]];
		}
	}
	
	struct result { size_t first_fail, count_of_jumps, last_nonfail; };
	result first_fail(size_t i, const function<bool(size_t)> &is_fail) {
		if(is_fail(i)) return { i, 0, n };
		size_t jumps = 0;
		for(size_t h=L; h--;){
			size_t j = jump[h][i];
			if(j<n && !is_fail(j)) i = j, jumps+=1<<h;
		}
		return { jump[0][i], jumps+1, i };
	}
	
	size_t kth_jump(size_t i, size_t k) {
		for(size_t l=0;l<L;++l) if(k>>l&1) i = jump[l][i];
		return i;
	}
	
	private:
	size_t n, L;
	vector<vector<size_t>> jump;
};