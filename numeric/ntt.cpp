namespace NTT {
	constexpr size_t ctz(uint32_t n) { return std::countr_zero(n); }
	
	template<int mod> struct ntt_device {
		using mint = modint<mod>;
		static constexpr size_t H = ctz(mod-1), MAXN = size_t(1) << H;
		
		static inline const auto roots = [] {
			array<mint, H+1> r{};
			for(int g=2; g<mod; ++g) {
				r[H] = g;
				for(size_t i=H; i--; ) r[i] = r[i+1] * r[i+1];
				if(r[0] == 1 && r[1] != 1) break;
			}
			return r;
		}();
		
		static void ntt(vector<mint> &a) {
			const size_t n = size(a); assert((n&(n-1)) == 0 && n <= MAXN);
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
			for(int i=3; i*i<=p; i+=2) if(p%i == 0) return false;
			return true;
		}
		
		template<class T, size_t min_h> constexpr bool is_ntt_modint = false;
		template<decltype(auto) mod, size_t min_h> constexpr bool is_ntt_modint<modint<mod>,min_h> = is_same_v<decltype(mod),int> && ctz(mod-1)>=min_h && is_ntt_prime(mod);
		
		template<size_t min_h, int min_mod, size_t ...I>
		constexpr auto __gen_ntt_mods(index_sequence<I...>) {
			constexpr auto mods = [] {
				array<int, sizeof...(I)> ar{};
				for(size_t i = 0, h = 30; h >= min_h; --h)
				for(int c = 1; i < size(ar) && c <= (numeric_limits<int>::max()>>(h+1)); c+=2)
				if(int mod = (c<<h)+1; mod >= min_mod && is_ntt_prime(mod)) ar[i++] = mod;
				return ar;
			}();
			static_assert(mods.back() != 0, "Can't find enough required ntt mods");
			return integer_sequence<int, mods[I]...>{};
		}
		
		template<size_t count, size_t min_h, int min_mod> using make_ntt_mods = decltype(__gen_ntt_mods<min_h, min_mod>(make_index_sequence<count>{}));
		
		template<int mod> vector<modint<mod>> convolution(const vector<auto> &a, const vector<auto> &b) {
			if(size(a) < size(b)) return convolution<mod>(b, a);
			if(empty(b)) return {};
			const size_t n = size(a)+size(b)-1, d = std::bit_ceil(n);
			vector<modint<mod>> fa(d), fb(d);
			copy(begin(a), end(a), begin(fa));
			copy(begin(b), end(b), begin(fb));
			bool equal = fa == fb;
			ntt_device<mod>::ntt(fa);
			if(equal) fb = fa; else ntt_device<mod>::ntt(fb);
			for(size_t i=0; i<d; ++i) fa[i]*=fb[i];
			ntt_device<mod>::ntt_inv(fa);
			fa.resize(n);
			return fa;
		}
		
		template<class T, int ...mod> auto convolution(const vector<auto> &a, const vector<auto> &b, integer_sequence<int, mod...>) {
			return crt_device::evaluate<T>(convolution<mod>(a, b)...);
		}
	}
	
	template<class T, size_t max_size = 1<<23, size_t count_mods = 3, int min_mod = (int)8e8>
	auto convolution(const vector<auto> &a, const vector<auto> &b) {
		constexpr size_t h = max(std::bit_width(max_size - 1), min_adequate_h);
		if constexpr (is_ntt_modint<T, h>) return convolution<T::get_mod()>(a, b);
		else return convolution<T>(a, b, make_ntt_mods<count_mods, h, min_mod>{});
	}
}
