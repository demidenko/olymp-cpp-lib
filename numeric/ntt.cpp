namespace NTT {
	constexpr size_t ctz(int x) { return x%2 ? 0 : ctz(x>>1) + 1; }
	
	template<int mod> struct ntt_device {
		using mint = modint<mod>;
		static constexpr size_t H = ctz(mod-1), MAXN = size_t(1) << H;
		
		static auto make_roots() {
			array<mint, H+1> r;
			for(int g=2; g<mod; ++g) {
				r[H] = g;
				for(size_t i=H; i--; ) r[i] = r[i+1] * r[i+1];
				if(r[0] == 1 && r[1] != 1) return r;
			}
		}
		
		static void ntt(vector<mint> &a) {
			static const auto roots = make_roots();
			const size_t n = size(a);
			for(size_t i=1, j=0, t; i<n; ++i) {
				for(t = n>>1; j&t; t>>=1) j^=t;
				if((j^=t) < i) swap(a[i], a[j]);
			}
			for(size_t h=1, k=1; k*2<=n; ++h, k<<=1)
				for(size_t p=0; p<n; p+=k*2) {
					mint cw = 1;
					for(size_t i=0; i<k; ++i) {
						mint v = cw*a[p+i+k], &u = a[p+i];
						a[p+i+k] = u - v;
						u += v;
						cw *= roots[h];
					}
				}
		}
		
		static void ntt_inv(vector<mint> &a) {
			ntt(a);
			reverse(begin(a)+1, end(a));
			mint i = 1 / mint(size(a));
			for(mint &val : a) val*=i;
		}
	};
	
	namespace {
		constexpr size_t min_adequate_h = 13;
		constexpr bool is_ntt_prime(int p) {
			if(p < 2 || p%2 == 0 || ctz(p-1) < min_adequate_h) return false;
			for(int i=3; i*i*i*i<=p; i+=2) if(p%i == 0) return false;
			int64_t r = 1, t = 2;
			for(int n = p-1; n>0; n>>=1, t = t*t %p) if(n&1) r = r*t %p;
			return r == 1;
		}
		
		using ntt_mods = integer_sequence<int, 
			998244353,
			//754974721,
			//985661441,
			//943718401,
			897581057,
			880803841
		> ;
		
		template<class T> struct is_ntt_modint: false_type {};
		template<decltype(auto) mod> struct is_ntt_modint<modint<mod>>: bool_constant<is_same_v<decltype(mod),int> && is_ntt_prime(mod)> {};
		
		template<int mod> vector<modint<mod>> convolution(const vector<auto> &a, const vector<auto> &b) {
			if(size(a) < size(b)) return convolution<mod>(b, a);
			if(empty(b)) return {};
			size_t n = size(a) + size(b) - 1, d = 1;
			while(d < n) d<<=1; assert(d <= ntt_device<mod>::MAXN);
			vector<modint<mod>> fa(d), fb(d);
			copy(begin(a), end(a), begin(fa));
			copy(begin(b), end(b), begin(fb));
			ntt_device<mod>::ntt(fa);
			ntt_device<mod>::ntt(fb);
			for(size_t i=0; i<d; ++i) fa[i]*=fb[i];
			ntt_device<mod>::ntt_inv(fa);
			fa.resize(n);
			return fa;
		}
		
		template<class T, int ...mod> auto convolution(const vector<auto> &a, const vector<auto> &b, integer_sequence<int, mod...>) {
			return crt_device::evaluate<T>(convolution<mod>(a, b)...);
		}
	}
	
	template<class T> auto convolution(const vector<auto> &a, const vector<auto> &b) {
		if constexpr (is_ntt_modint<T>::value) return convolution<T::get_mod()>(a, b);
		else return convolution<T>(a, b, ntt_mods{});
	}
}
