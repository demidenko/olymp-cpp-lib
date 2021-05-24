int inv_mod(int a, int m){
	assert(0<a);
	assert(a<m);
	return a==1 ? 1 : m-m*int64_t(inv_mod(m%a, a))/a;
}

struct mint {
	constexpr inline static int mod = 998'244'353 ;
	int x;
	mint(): x(0) {}
	mint(const auto &val): x(val%mod) { if(x<0) x+=mod; }
	
	void operator+=(const mint &b){ x+=b.x; if(x>=mod) x-=mod; }
	void operator-=(const mint &b){ x-=b.x; if(x<0) x+=mod; }
	void operator*=(const mint &b){ x = int64_t(x)*b.x %mod; }
	void operator/=(const mint &b){ x = int64_t(x)*inv_mod(b.x,mod) %mod; }
	friend mint operator+(mint a, const mint &b){ a+=b; return a; }
	friend mint operator-(mint a, const mint &b){ a-=b; return a; }
	friend mint operator*(mint a, const mint &b){ a*=b; return a; }
	friend mint operator/(mint a, const mint &b){ a/=b; return a; }
	friend mint operator-(mint a){ if(a.x) a.x = mod - a.x; return a; }
	friend bool operator==(const mint &a, const mint &b){ return a.x==b.x; }
	friend bool operator!=(const mint &a, const mint &b){ return a.x!=b.x; }
	friend ostream& operator<<(ostream &o, const mint &m){ return o<<m.x; }
};

mint operator""m(unsigned long long x){ return mint(x); }

mint mpow(mint a, uint64_t n){
	mint res = 1;
	for(; n; n>>=1, a*=a) if(n&1) res*=a;
	return res;
}

struct Cnk {
	vector<mint> f, fi;
	Cnk(size_t n): f(n+1), fi(n+1){
		f[0] = fi[0] = 1;
		for(size_t i=1; i<=n; ++i) f[i] = f[i-1]*i;
		fi[n] = 1 / f[n];
		for(size_t i=n; i>1; --i) fi[i-1] = fi[i]*i;
	}
	mint operator()(size_t n, size_t k){
		if(n<k) return 0; assert(n<size(f));
		return f[n]*fi[n-k]*fi[k];
	}
};

