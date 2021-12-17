namespace NTT {
	constexpr int get_pow2(int x) { return x%2 ? 0 : get_pow2(x>>1) + 1; }
	
	template<int mod> struct ntt_device {
		using mint = modint<mod>;
		static constexpr size_t H = get_pow2(mod-1), MAXN = size_t(1) << H;
		
		static auto make_roots() {
			array<mint, H+1> r;
			for(int g=1; g<mod; ++g) {
				r[H] = g;
				for(size_t i=H; i--; ) r[i] = r[i+1] * r[i+1];
				if(r[0] == 1 && r[1] != 1) return r;
			}
		}
		
		static void ntt(vector<mint> &a) {
			static const auto roots = make_roots();
			const size_t n = size(a);
			for(size_t i=1, j=0, bit; i<n; ++i) {
				for(bit = n>>1; j&bit; bit>>=1) j^=bit;
				if((j^=bit) < i) swap(a[i], a[j]);
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
			auto div = 1 / mint(size(a));
			for(auto &val : a) val*=div;
		}
	};
	
	namespace {
		using ntt_mods = integer_sequence<int, 
			998244353,
			//754974721,
			//985661441,
			//943718401,
			897581057,
			880803841
		>;
		
		template<class T, int ...mod> constexpr bool is_valid_modint(integer_sequence<int, mod...>) {
			return (is_same_v<T, modint<mod>> || ...);
		}
		template<class T> using is_ntt_modint = bool_constant<is_valid_modint<T>(ntt_mods{})>;
		
		template<int mod, class A>
		vector<modint<mod>> convolution(const vector<A> &a, const vector<A> &b) {
			if(size(a) < size(b)) return convolution<mod>(b, a);
			if(empty(b)) return {};
			size_t n = size(a), m = size(b), d = 1;
			while(d < n+m-1) d<<=1; assert(d <= ntt_device<mod>::MAXN);
			vector<modint<mod>> fa(d), fb(d);
			copy(begin(a), end(a), begin(fa));
			copy(begin(b), end(b), begin(fb));
			ntt_device<mod>::ntt(fa);
			ntt_device<mod>::ntt(fb);
			for(size_t i=0; i<d; ++i) fa[i]*=fb[i];
			ntt_device<mod>::ntt_inv(fa);
			fa.resize(n+m-1);
			return fa;
		}
		
		template<class T, class A, int ...mods>
		vector<T> convolution(const vector<A> &a, const vector<A> &b, integer_sequence<int,mods...>) {
			return crt_device::evaluate<T>(convolution<mods>(a, b)...);
		}
	}
	
	template<class T, class A>
	vector<T> convolution(const vector<A> &a, const vector<A> &b) {
		if constexpr (is_ntt_modint<T>::value) return convolution<T::get_mod()>(a, b);
		else return convolution<T>(a, b, ntt_mods{});
	}
}
