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
		
		void ntt(vector<mint> &a) {
			static const auto roots = make_roots();
			const size_t n = size(a), H = __lg(n);
			for(size_t i=1, j=0, bit; i<n; ++i) {
				for(bit = n>>1; j&bit; bit>>=1) j^=bit;
				if((j^=bit) < i) swap(a[i], a[j]);
			}
			for(size_t h=1, k=2; h<=H; ++h, k<<=1) {
				size_t step = n>>h;
				for(size_t p=0; p<n; p+=k) {
					mint cw = 1;
					for(size_t i=0, j=k/2; j<k; ++i, ++j) {
						mint v = cw*a[p+j], u = a[p+i];
						a[p+i] = u + v;
						a[p+j] = u - v;
						cw *= roots[h];
					}
				}
			}
		}
		
		void ntt_inv(vector<mint> &a) {
			ntt(a);
			reverse(begin(a)+1, end(a));
			auto div = 1 / mint(size(a));
			for(auto &val : a) val*=div;
		}
	}
	
	vector<mint> convolution(const vector<auto> &a, const vector<auto> &b) {
		if(size(a) < size(b)) return convolution(b, a);
		if(empty(b)) return {};
		size_t n = size(a), m = size(b), d = 1;
		while(d < n+m-1) d<<=1; assert(d<=MAXN);
		vector<mint> fa(d), fb(d);
		copy(begin(a), end(a), begin(fa));
		copy(begin(b), end(b), begin(fb));
		ntt(fa);
		ntt(fb);
		for(size_t i=0; i<d; ++i) fa[i]*=fb[i];
		ntt_inv(fa);
		fa.resize(n+m-1);
		return fa;
	}
}
