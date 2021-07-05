struct binomial {
	vector<mint> f, fi;
	binomial(size_t n): f(n+1), fi(n+1) {
		f[0] = fi[0] = 1;
		for(size_t i=1; i<=n; ++i) f[i] = f[i-1]*i;
		fi[n] = 1 / f[n];
		for(size_t i=n; i>1; --i) fi[i-1] = fi[i]*i;
	}
	mint operator()(size_t n, size_t k) {
		if(n<k) return 0; assert(n<size(f));
		return f[n]*fi[n-k]*fi[k];
	}
};
