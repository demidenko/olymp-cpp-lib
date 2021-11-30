template<class T = mint> struct binomial {
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
	static T go(T cnk, size_t n, size_t k, const size_t target_n, const size_t target_k) {
		while(n < target_n) if(++n == k) cnk = 1; else cnk *= n, cnk /= n-k;
		while(n > target_n) if(--n < k) cnk = 0; else cnk *= n+1-k, cnk /= n+1;
		while(k < target_k) if(n < ++k) cnk = 0; else cnk *= n-k+1, cnk /= k;
		while(k > target_k) if(n == --k) cnk = 1; else cnk *= k+1, cnk /= n-k;
		return cnk;
	}
	private: vector<T> f, finv;
};
