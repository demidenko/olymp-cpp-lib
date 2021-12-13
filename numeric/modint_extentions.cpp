template<class mint> optional<mint> sqrt(const mint &a) {
	const int mod = mint::get_mod();
	if(mod == 2 || a == 0) return a;
	if(gcd((int)a, mod) != 1 || pow(a, (mod-1)>>1) == -1) return nullopt;
	int s = -1, t = mod - 1;
	while(t%2 == 0) ++s, t>>=1;
	mint a_inv = 1 / a, r = pow(a, (t+1)>>1), c;
	static mt19937 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());
	do c = rnd(); while(pow(c, (mod-1)/2) != -1);
	for(c = pow(c, t); s--; c*=c) if(mint d = pow(r*r*a_inv, 1<<s); d == -1) r*=c;
	return r;
}
