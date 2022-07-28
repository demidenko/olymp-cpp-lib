//g(n) = a(n) - sum i=2..n g(n/i)
//almost O(n ** 2/3)
template<class T>
T g(int64_t maxn, auto &&a) {
	const int32_t sq = max(sqrt(maxn), pow(maxn / log(maxn + 1), 2./3) * 3);
	vector<T> small(sq + 1, 0), big(maxn / (sq + 1) + 1);
	
	for(int32_t n = 1; n <= sq; ++n) {
		small[n] += small[n-1] - a(n-1) + a(n);
		const T dif = small[n] - small[n-1];
		for(int32_t i = 2; i <= sq / n; ++i) small[i * n] -= dif;
	}
	
	for(int32_t d = maxn / (sq + 1) ; d >= 1; --d) {
		const int64_t n = maxn / d;
		const int32_t sn = sqrt(n);
		T res = a(n);
		
		for(int32_t k = 1; k <= sn; ++k) {
			int64_t r = n / k, l = n / (k + 1) + 1;
			assert(l > 1);
			res -= small[k] * T(r - l + 1);
		}
		
		for(int32_t i = 2; i <= n / (sn + 1); ++i) {
			int64_t k = n / i;
			res -= (k <= sq ? small[k] : big[maxn / k]);
		}
		
		big[d] = res;
	}
	
	return maxn <= sq ? small[maxn] : big[1];
}
