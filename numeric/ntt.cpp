namespace NTT {
	template<int mod> struct ntt_device {
		using mint = modint<mod>;
		static constexpr size_t H = std::countr_zero<uint32_t>(mod-1);
		
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
			const size_t n = size(a); assert(std::has_single_bit(n) && n <= (1uz << H));
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
		constexpr bool is_ntt_prime(int p, size_t size) {
			if(p < 2 || p%2 == 0 || (1u << std::countr_zero<uint32_t>(p-1)) < size) return false;
			for(int i=3; i*i<=p; i+=2) if(p%i == 0) return false;
			return true;
		}
		
		template<class T, size_t size> constexpr bool is_ntt_modint = false;
		template<decltype(auto) mod, size_t size> constexpr bool is_ntt_modint<modint<mod>, size> = is_same_v<decltype(mod),int> && is_ntt_prime(mod, size);
		
		template<size_t size, int min_mod, int max_mod = numeric_limits<int>::max(), size_t ...I>
		constexpr auto __gen_ntt_mods(index_sequence<I...>) {
			constexpr auto mods = [] {
				array<int, sizeof...(I)> ar{};
				for(size_t i = 0, h = 30; (1u << h) >= size; --h)
				for(int c = 1; i < ar.size() && c <= ((max_mod-1)>>h); c+=2)
				if(int mod = (c<<h)+1; mod >= min_mod && is_ntt_prime(mod, size)) ar[i++] = mod;
				return ar;
			}();
			static_assert(mods.back() != 0, "Can't find enough required ntt mods");
			return integer_sequence<int, mods[I]...>{};
		}
		
		template<size_t count, size_t size, int min_mod> using make_ntt_mods = decltype(__gen_ntt_mods<size, min_mod>(make_index_sequence<count>{}));
		
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
		
		template<class T, class CRT, int ...mod> auto convolution(const vector<auto> &a, const vector<auto> &b, integer_sequence<int, mod...>) {
			return CRT::template evaluate<T>(convolution<mod>(a, b)...);
		}
	}
	
	template<class T, size_t max_size = 1<<23> requires is_ntt_modint<T, max_size>
	auto convolution(const vector<auto> &a, const vector<auto> &b) { return convolution<T::get_mod()>(a, b); }
	
	template<class T, class CRT, size_t max_size = 1<<23, size_t count_mods = 3, int min_mod = (int)1e9>
	auto convolution(const vector<auto> &a, const vector<auto> &b) {
		return convolution<T, CRT>(a, b, make_ntt_mods<count_mods, max_size, min_mod>{});
	}
}
