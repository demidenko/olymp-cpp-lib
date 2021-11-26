template<class T = mint> struct binomial {
	vector<T> f, finv;
	binomial(size_t n): f(n+1), finv(n+1) {
		f[0] = finv[0] = 1;
		for(size_t i=1; i<=n; ++i) f[i] = f[i-1] * i;
		finv[n] = 1 / f[n];
		for(size_t i=n; i>1; --i) finv[i-1] = finv[i] * i;
	}
	T operator()(size_t n, size_t k) {
		if(n<k) return 0;
		return f.at(n) * finv[n-k] * finv[k];
	}
};
