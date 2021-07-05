namespace NTT {
	static_assert(is_same_v<mint, modint<998244353>>);
	
	namespace {
		constexpr size_t MAXH = 23, MAXN = size_t(1)<<MAXH;
		
		auto make_roots() {
			array<mint, MAXH+1> r;
			r[MAXH] = 31;
			for(size_t i=MAXH; i--; ) r[i] = r[i+1] * r[i+1];
			return r;
		}
		
		size_t rev(size_t i, size_t H) {
			const size_t w = sizeof(size_t) * 8;
			for(size_t k = w, ms = (size_t(1)<<k)-1; k>>=1; ) {
				ms^=(ms<<k);
				size_t a = i&(ms<<k), b = i&ms;
				i = (a>>k)|(b<<k);
			}
			return i>>(w-H);
		}
		
		void ntt(const vector<mint> &a, vector<mint> &ans) {
			static const auto roots = make_roots();
			const size_t n = size(a), H = __lg(n);
			for(size_t i=0; i<n; ++i) ans[rev(i,H)] = a[i];
			for(size_t h=1, k=2; h<=H; ++h, k<<=1) {
				size_t step = n>>h;
				for(size_t p=0; p<n; p+=k) {
					mint cw = 1;
					for(size_t i=0, j=k/2; j<k; ++i, ++j) {
						mint v = cw*ans[p+j], u = ans[p+i];
						ans[p+i] = u + v;
						ans[p+j] = u - v;
						cw *= roots[h];
					}
				}
			}
		}
		
		void ntt_inv(const vector<mint> &a, vector<mint> &ans) {
			ntt(a, ans);
			reverse(begin(ans)+1, end(ans));
			auto div = 1 / mint(size(a));
			for(auto &val : ans) val*=div;
		}
	}
	
	vector<mint> convolution(const vector<mint> &a, const vector<mint> &b) {
		if(size(a) < size(b)) return convolution(b, a);
		if(empty(b)) return {};
		size_t n = size(a), m = size(b), d = 1;
		while(d < n+m-1) d<<=1; assert(d<=MAXN);
		vector<mint> t(d), fa(d), fb(d);
		copy(begin(b), end(b), begin(t));
		ntt(t, fb);
		copy(begin(a), end(a), begin(t));
		ntt(t, fa);
		for(size_t i=0; i<d; ++i) fa[i]*=fb[i];
		ntt_inv(fa, t);
		t.resize(n+m-1);
		return t;
	}
}
