template<class T> struct binomial {
	binomial(size_t n): f(n+1), finv(n+1) {
		f[0] = finv[0] = 1;
		for(size_t i=1; i<=n; ++i) f[i] = f[i-1] * i;
		finv[n] = T(1) / f[n];
		for(size_t i=n; i>1; --i) finv[i-1] = finv[i] * i;
	}
	T operator()(size_t n, size_t k) const {
		return n < k ? 0 : f.at(n) * finv[n-k] * finv[k];
	}
	T factorial(size_t n) const { return f.at(n); }
	T inv_factorial(size_t n) const { return finv.at(n); }
	private: vector<T> f, finv;
};
