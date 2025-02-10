// mex_rect(l1, l2, x, r1, r2)
// l1 <= l < l2 && r1 <= r < r2 : mex(a[l], ..., a[r]) = x
void mex_rects(const vector<int> &a, auto &&mex_rect) {
	const int n = size(a);
	//ignore a[i] >= n
	
	vector<int> st(n), sr(n), sx(n);
	// set<int> ls;
	
	auto del = [&](int l, int t) {
		int r = sr[l], x = sx[l];
		assert(r > l);
		// ls.erase(l);
		mex_rect(st[l], t, x, l, r);
		sr[l] = sx[l] = 0;
	};
	
	auto ins = [&](int l, int r, int x, int t) {
		if(l >= r) return ;
		// ls.insert(l);
		sr[l] = r;
		sx[l] = x;
		st[l] = t;
	};
	
	auto replace = [&](int l, int r, int x, int t) {
		if(l >= r) return ;
		
		assert(sr[l] > 0);
		for(int i = l; i < r; ) {
			int pr = sr[i], px = sx[i];
			del(i, t);
			if(pr > r) ins(r, pr, px, t);
			i = pr;
		}
		
		ins(l, r, x, t);
	};
	
	vector<int> u(n+1);
	for(int i=0, mex=0, l=0; i<n; ++i) {
		if(int x = a[i]; x < n) u[x] = 1;
		while(u[mex]) ++mex;
		if(i+1 == n || a[i+1] == mex) {
			ins(l, i+1, mex, 0);
			l = i+1;
		}
	}
	
	vector<int> nxt(n, n), fpos(n, n);
	for(int i=n-1; i>=0; --i) if(int x = a[i]; x < n) {
		nxt[i] = fpos[x];
		fpos[x] = i;
	}
	
	rmq<int, greater{}> mfpos(fpos);
	for(int i=0; i<n; ++i) {
		del(i, i+1);
		ins(i+1, sr[i], sx[i], i+1);
		if(int x = a[i]; x < n) {
			mfpos.set_value(x, fpos[x] = nxt[i]);
			int l = x > 0 ? mfpos(0, x) : i+1, r = fpos[x];
			replace(l, r, x, i+1);
		}
	}
}
