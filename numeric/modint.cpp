template<decltype(auto) mod> struct modint { static_assert(is_same_v<decay_t<decltype(mod)>, int>);
	modint(): x(0) {}
	template<class T, class=enable_if_t<is_integral_v<T>>> modint(T val): x(val%mod) { if(x<0) x+=mod; }
	static constexpr int get_mod() { return mod; }
	#define __op(O, E, F) modint& operator E(const modint &b) { F return *this; } friend modint operator O(modint a, const modint &b) { return a E b; }
	__op(+, +=,  x+=b.x; if(x>=mod) x-=mod; )
	__op(-, -=,  x-=b.x; if(x<0) x+=mod; )
	__op(*, *=,  x = int64_t(x)*b.x %mod; )
	__op(/, /=,  auto i=b.inverse(); assert(i); x = int64_t(i->x)*x %mod; )
	friend modint operator-(modint a) { if(a.x) a.x = mod - a.x; return a; }
	friend modint pow(modint a, uint64_t n) { modint p=1; for(; n; n>>=1, a*=a) if(n&1) p*=a; return p; }
	optional<modint> inverse() const {
		int a = x, m = mod, p = 1, q = 0;
		while(a) swap(p, q -= (m / a) * p), swap(a, m %= a);
		if(m == 1) return q; else return nullopt;
	}
	friend bool operator==(const modint &a, const modint &b) { return a.x == b.x; }
	friend bool operator!=(const modint &a, const modint &b) { return a.x != b.x; }
	friend ostream& operator<<(ostream &o, const modint &m) { return o<<m.x; }
	const int& operator*() const { return x; }
	private: int x;
};
using mint = modint<(int)998244353>;
