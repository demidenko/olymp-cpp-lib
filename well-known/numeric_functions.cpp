//g(n) = a(n) - sum i=2..n g(n/i) * b(i)
//almost O(n ** 2/3)
template<class T>
T g(uint64_t maxn, auto &&a, auto &&b, auto &&sumb) {
	const uint32_t sq = max(sqrt(maxn), pow(maxn / log(maxn + 1), 2./3) * 2);
	vector<T> small(sq + 1, 0), big(maxn / (sq + 1) + 1);
	
	for(uint32_t n = 1; n <= sq; ++n) {
		small[n] += small[n-1] - a(n-1) + a(n);
		const T dif = small[n] - small[n-1];
		for(uint32_t i = 2; i * n <= sq; ++i) small[i * n] -= dif * b(i);
	}
	
	for(uint32_t d = maxn / (sq + 1) ; d >= 1; --d) {
		const uint64_t n = maxn / d;
		const uint32_t sn = sqrt(n), sp = n / (sn + 1);
		T res = a(n);
		
		uint64_t l, r = n;
		for(uint32_t k = 1; k <= sn; ++k, r = l) {
			l = n / (k + 1);
			res -= small[k] * (sumb(r) - sumb(l));
			if(k+1 <= sp) res -= (l <= sq ? small[l] : big[maxn / l]) * b(k+1);
		}
		
		big[d] = res;
	}
	
	return maxn <= sq ? small[maxn] : big[1];
}
