namespace NTT {
	constexpr size_t MAXH = 23, MAXN = size_t(1)<<MAXH;
	mint root[MAXH+1];

	bool init_ntt() {
		static_assert(is_same_v<mint, modint<998244353>>);
		root[MAXH] = 31;
		for(size_t i=MAXH-1; i; --i) root[i] = root[i+1]*root[i+1];
		return true;
	}
	
	template<size_t H = MAXH, size_t step = 1>
	void ntt(vector<mint> &a, vector<mint> &ans, size_t l, size_t cl) {
		if constexpr (H==0) ans[l] = a[cl];
		else {
			constexpr size_t n = size_t(1)<<H;
			if(size(a) < n) {
				ntt<H-1,step>(a, ans, l, cl);
				return ;
			}
			ntt<H-1,step*2>(a, ans, l, cl);
			ntt<H-1,step*2>(a, ans, l+n/2, cl+step);
			mint cw = 1, gw = root[H];
			for(size_t i=l, j=l+n/2; j<l+n; i++, j++) {
				mint u = ans[i], v = cw*ans[j];
				ans[i] = u + v;
				ans[j] = u - v;
				cw *= gw;
			}
		}
	}
	
	void ntt_inv(vector<mint> &a, vector<mint> &ans, size_t n) {
		ntt(a, ans, 0, 0);
		reverse(begin(ans)+1, end(ans));
		auto div = mint(1) / n;
		for(auto &val : ans) val*=div;
	}
	
	vector<mint> convolution(const vector<mint> &a, const vector<mint> &b) {
		if(size(a) < size(b)) return convolution(b, a);
		if(empty(b)) return {};
		size_t n = size(a), m = size(b), d = 1;
		while(d < n+m-1) d<<=1; assert(d<=MAXN);
		static bool init_called = NTT::init_ntt();
		vector<mint> t(d), fa(d), fb(d);
		copy(begin(b), end(b), begin(t));
		ntt(t, fb, 0, 0);
		copy(begin(a), end(a), begin(t));
		ntt(t, fa, 0, 0);
		for(size_t i=0; i<d; ++i) fa[i]*=fb[i];
		ntt_inv(fa, t, d);
		t.resize(n+m-1);
		return t;
	}
};
