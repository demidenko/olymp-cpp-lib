/*
	Given array a_1 .. a_n and m ranges [l, r)
	For each range calculate some value based on sorted values of this range
	
	If value can be changed in O(1) after insertion i between pr and nx
	then answers can be found in O(n \sqrt{m} + n \log{n})
	
	examples: 
	https://codeforces.com/contest/765/problem/F
	https://www.codechef.com/problems/MINXORSEG
*/
template<class V, class T>
vector<V> solve(
	const vector<T> &a,
	const vector<pair<size_t,size_t>> &segs,
	const V &neutral,
	auto &&f //(V &val, i, pr, nx)
) {
	
	const size_t n = size(a), m = size(segs); if(m == 0) return {};
	const size_t D = max<size_t>(1, n / sqrt(m*2));
	const size_t K = (n-1) / D + 1;
	
	vector<size_t> p(n);
	iota(begin(p), end(p), 0);
	sort(begin(p), end(p), [&a](size_t i, size_t j) { return a[i] < a[j]; });
	
	vector<size_t> in_st(K, n), in_nx(n, n);
	for(size_t k=n; k--; ) {
		size_t i = p[k], b = i / D;
		in_nx[i] = in_st[b];
		in_st[b] = i;
	}
	
	vector<V> ans(m, neutral);
	
	vector<vector<size_t>> qs(K);
	
	for(size_t k = 0; k < m; ++k) {
		auto [ql, qr] = segs[k];
		assert(0 <= ql && ql <= qr && qr <= n);
		
		const size_t bl = ql / D;
		const size_t br = (qr-1) / D;
		
		if(bl == br) {
			for(size_t i = in_st[bl], p = n; i < n; i = in_nx[i])
				if(ql <= i && i < qr) {
					f(ans[k], i, p, n);
					p = i;
				}
		} else {
			qs[bl].emplace_back(k);
		}
	}
	
	
	vector<size_t> pr(n, n), nx(n, n), hist;
	auto del = [&](size_t i) {
		hist.push_back(i);
		if(pr[i] < n) nx[pr[i]] = nx[i];
		if(nx[i] < n) pr[nx[i]] = pr[i];
	};
	
	auto undo = [&] {
		assert(!empty(hist));
		const size_t i = hist.back();
		hist.pop_back();
		if(size_t l = pr[i]; l < n) nx[l] = i;
		if(size_t r = nx[i]; r < n) pr[r] = i;
	};
	
	auto undo_calc = [&](V &cur) {
		const size_t i = hist.back();
		f(cur, i, pr[i], nx[i]);
		undo();
	};
	
	for(size_t k = 0; k + 1 < n; ++k) nx[p[k]] = p[k+1], pr[p[k+1]] = p[k];
	for(size_t i = 0; i < n; ++i) del(i);
	
	vector<V> snapshots(n, neutral);
	for(size_t bl = K; bl--;) {
		const size_t sl = bl * D, sr = min(sl + D, n) - 1;
		for(size_t i = sl; i <= sr; ++i) undo(); //#1
		
		for(size_t i = n; i-- > sl; ) del(i);
		
		auto &vec = qs[bl];
		sort(begin(vec), end(vec), [&](size_t i, size_t j) { return segs[i].second < segs[j].second; });
		auto it = begin(vec);
		
		V val = neutral;
		for(size_t i = sl; i < n; ++i) {
			undo_calc(val); //#2
			
			while(it != end(vec) && segs[*it].second == i + 1) {
				const size_t k = *it++, ql = segs[k].first;
				
				for(size_t i = sl; i <= sr; ++i) del(i);
				
				V cur = snapshots[i];
				for(size_t i = ql; i <= sr; ++i) undo_calc(cur); //#3
				ans[k] = cur;
				
				for(size_t i = sl; i < ql; ++i) undo(); //#3
			}
			
			snapshots[i] = val;
		}
	}
	
	// #1: n
	// #2: (K+1)*K/2 * D = K*K*D/2 = n*K/2
	// #3: m * D
	
	return ans;
}
