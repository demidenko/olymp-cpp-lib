int inv_mod(int a, int m) { assert(0<a); assert(a<m);
	return a==1 ? 1 : m - (int64_t)m * inv_mod(m%a, a) / a;
}

template<int mod> struct modint {
	modint(): x(0) {}
	template<class T, class=enable_if_t<is_integral_v<T>>> modint(T val): x(val%mod) { if(x<0) x+=mod; }
	static int get_mod() { return mod; }
	void operator+=(const modint &b) { x+=b.x; if(x>=mod) x-=mod; }
	void operator-=(const modint &b) { x-=b.x; if(x<0) x+=mod; }
	void operator*=(const modint &b) { x = int64_t(x)*b.x %mod; }
	void operator/=(const modint &b) { x = int64_t(x)*inv_mod(b.x,mod) %mod; }
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
