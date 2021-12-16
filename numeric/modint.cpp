optional<int> inv_mod(int a, int mod) {
	int x = 1, y = 0, m = mod; if(a >= mod || a < -mod) a %= mod; if(a < 0) a += mod;
	while(a) swap(x, y -= (m / a) * x), swap(a, m %= a);
	if(m == 1) return y < 0 ? y + mod : y; else return nullopt;
}

template<decltype(auto) mod> struct modint { static_assert(is_same_v<remove_reference_t<decltype(mod)>, int>);
	modint(): x(0) {}
	template<class T, class=enable_if_t<is_integral_v<T>>> modint(T val): x(val%mod) { if(x<0) x+=mod; }
	static int get_mod() { return mod; }
	void operator+=(const modint &b) { x+=b.x; if(x>=mod) x-=mod; }
	void operator-=(const modint &b) { x-=b.x; if(x<0) x+=mod; }
	void operator*=(const modint &b) { x = int64_t(x)*b.x %mod; }
	void operator/=(const modint &b) { auto i=inv_mod(b.x,mod); assert(i); x = int64_t(*i)*x %mod; }
	friend modint operator+(modint a, const modint &b) { a+=b; return a; }
	friend modint operator-(modint a, const modint &b) { a-=b; return a; }
	friend modint operator*(modint a, const modint &b) { a*=b; return a; }
	friend modint operator/(modint a, const modint &b) { a/=b; return a; }
	friend modint operator-(modint a) { if(a.x) a.x = mod - a.x; return a; }
	friend modint pow(modint a, uint64_t n) { modint p=1; for(; n; n>>=1, a*=a) if(n&1) p*=a; return p; }
	bool operator==(const modint &b) const { return x == b.x; }
	bool operator!=(const modint &b) const { return x != b.x; }
	friend ostream& operator<<(ostream &o, const modint &m) { return o<<m.x; }
	explicit operator int() const { return x; }
	private: int x;
};
using mint = modint<(int)998244353>;
mint operator""m(unsigned long long x) { return mint(x); }
