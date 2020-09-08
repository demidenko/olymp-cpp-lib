int inv_mod(int a, int m){
	assert(0<a);
	assert(a<m);
	return a==1 ? 1 : m-m*1LL*inv_mod(m%a, a)/a;
}

const int mod = 1e9+7 ;
struct mint {
	int x;
	mint(): x(0) {}
	mint(const auto &val): x(val%mod) { if(x<0) x+=mod; }
};

void operator+=(mint &a, const mint &b){ a.x+=b.x; if(a.x>=mod) a.x-=mod; }
mint operator+(mint a, const mint &b){ a+=b; return a; }
void operator-=(mint &a, const mint &b){ a.x-=b.x; if(a.x<0) a.x+=mod; }
mint operator-(mint a, const mint &b){ a-=b; return a; }
void operator*=(mint &a, mint b){ a.x = 1LL*a.x*b.x %mod; }
mint operator*(mint a, const mint &b){ a*=b; return a; }
void operator/=(mint &a, const mint &b){ a.x = 1LL*a.x*inv_mod(b.x,mod) %mod; }
mint operator/(mint a, const mint &b){ a/=b; return a; }
mint operator-(mint a){ if(a.x) a.x = mod - a.x; return a; }
mint operator""m(unsigned long long x){return mint(x);}
ostream& operator<<(ostream &o, const mint &m){ return o<<m.x; }
