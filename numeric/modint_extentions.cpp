template<class mint> optional<mint> sqrt(mint a) { //correct for prime mod
	if(*a < 2) return a;
	int m = mint::get_mod();
	if(m%2 == 0 || pow(a, m>>1) == -1) return nullopt;
	static mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
	mint t; do t = rnd(); while(*pow(t*t-4*a, m>>1) < 2);
	mint d = 1, c = -t, b = -t;
	for(++m; m>>=1; b=a*2-b*b, a*=a) m%2 ? d=c-d*b, c*=a : c=d*a-c*b;
	return d;
}
