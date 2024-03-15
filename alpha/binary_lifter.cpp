struct binary_lifter {
	binary_lifter(const std::ranges::sized_range auto &f = std::ranges::empty_view<size_t>{}) {
		const size_t n = size(f);
		if(n == 0) return ;
		
		const vector<size_t> p(begin(f), end(f));
		assert(*std::ranges::max_element(p) < n);
		
		jumps_to_cyc.assign(n, 0);
		first_cyc_v.resize(n);
		cyc_pos.resize(n);
		vector<size_t> used(n, n);
		for(size_t i=0; i<n; ++i) if(used[i] == n) {
			vector<size_t> path;
			for(size_t v=i; ; ) {
				used[v] = i;
				path.push_back(v);
				v = p[v];
				if(used[v] <= i) {
					if(used[v] == i) {
						auto it = find(begin(path), end(path), v);
						auto &cyc = cycs.emplace_back(it, end(path));
						for(size_t pos=0; pos<size(cyc); ++pos) {
							size_t x = cyc[pos];
							first_cyc_v[x] = x;
							cyc_pos[x] = {size(cycs) - 1, pos};
						}
						path.erase(it, end(path));
					}
					break ;
				}
				used[v] = i;
			}
			for(size_t x : std::views::reverse(path)) {
				first_cyc_v[x] = first_cyc_v[p[x]];
				jumps_to_cyc[x] = jumps_to_cyc[p[x]] + 1;
			}
		}
		
		size_t d = *std::ranges::max_element(jumps_to_cyc);
		size_t L = std::countr_zero(std::bit_ceil(d));
		if(L == 0) return ;
		jump.assign(L, vector(n, n));
		jump[0] = std::move(p);
		for(size_t h=1; h<L; ++h)
		for(size_t i=0; i<n; ++i) jump[h][i] = jump[h-1][jump[h-1][i]];
	}
	
	size_t kth_jump(size_t i, std::unsigned_integral auto k) const {
		if(size_t d = jumps_to_cyc[i]; k < d) {
			for(size_t h=0; k>0; ++h, k>>=1) if(k&1) i = jump[h][i];
			return i;
		} else {
			auto [row, pos] = cyc_pos[first_cyc_v[i]];
			auto &cyc = cycs[row];
			return cyc[(pos + (k - d)) % size(cyc)];
		}
	}
	
	/*struct result { size_t first_false, last_true, jumps_to_false; };
	result first_false(size_t i, auto &&pred) const {
		if(!pred(i)) return { i, none, 0 };
		size_t jumps = 0;
		for(size_t h = L; h--; )
		if(size_t j = jump[h][i]; j != none && pred(j)) {
			i = j;
			jumps += size_t(1) << h;
		}
		return { jump[0][i], i, jumps+1 };
	}*/
	
	private:
	vector<vector<size_t>> jump;
	vector<vector<size_t>> cycs;
	vector<pair<size_t,size_t>> cyc_pos;
	vector<size_t> jumps_to_cyc, first_cyc_v;
};
