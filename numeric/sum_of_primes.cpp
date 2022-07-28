template<class T, size_t K>
struct sum_of_primes {
	
	vector<int32_t> pr;
	sum_of_primes(int64_t n): pr(get_primes(root2(n+1)*2)) {
		mem_pi.resize(pr.back() + 1);
		for(int32_t p : pr) mem_pi[p] = pow<K>(p);
		for(size_t i=1; i<size(mem_pi); ++i) mem_pi[i] += mem_pi[i-1];
		
		size_t bufn = min<size_t>(1 << 17, n);
		size_t bufm = min<size_t>(1 << 6, size(pr));
		mem_f.assign(bufm, vector<T>(bufn));
		for(size_t i=0; i<bufn; ++i) mem_f[0][i] = sum_pows<K>(i);
		for(size_t j=1; j<bufm; ++j) {
			T pk = pow<K>(pr[j-1]);
			for(size_t i=0; i<bufn; ++i) mem_f[j][i] = mem_f[j-1][i] - mem_f[j-1][i / pr[j-1]] * pk;
		}
	}
	
	///sum of i**K for 1 <= i <= n where all prime factors of i >= pr[m]
	vector<vector<T>> mem_f;
	T f(int64_t n, size_t m) {
		if(m == 0) return sum_pows<K>(n);
		if(m < size(mem_f) && n < size(mem_f[m])) return mem_f[m][n];
		if(int64_t p = pr[m]; p > n) return 1; //1**K
		else if(p * p > n) return pi(n) - pi(p - 1) + 1;
		return f(n, m - 1) - f(n / pr[m-1], m - 1) * pow<K>(pr[m-1]);
	}
	
	//sum of p**K for prime numbers 1 <= p <= n
	vector<T> mem_pi;
	T pi(int64_t n) {
		if(n < 2) return 0;
		if(n <= pr.back()) return mem_pi[n];
		const size_t m = upper_bound(begin(pr), end(pr), root3(n)) - begin(pr);
		T res = f(n, m) + pi(pr[m-1]) - 1;
		for(size_t k = m; k < size(pr); ++k)
			if(int64_t p = pr[k]; p * p > n) break ;
			else res -= (pi(n / p) - pi(p - 1)) * pow<K>(p);
		return res;
	}
	
	static vector<int> get_primes(int32_t n) {
		vector<int32_t> md(n + 1), pr;
		for(int32_t i = 2; i <= n; ++i) {
			if(md[i] == 0) pr.push_back(md[i] = i);
			for(int32_t p : pr) {
				if(p > md[i] || p * i > n) break ;
				md[p * i] = p;
			}
		}
		return pr;
	}
	
	static int32_t root2(int64_t x) {
		int64_t s = sqrt(x);
		while(s * s <= x) ++s;
		return s - 1;
	}
	
	static int32_t root3(int64_t x) {
		int64_t s = cbrt(x);
		while(s * s * s <= x) ++s;
		return s - 1;
	}
	
	template<size_t P> static inline T pow(int64_t n) {
		if constexpr (P == 0) return 1; else
		if constexpr (P %2 == 1) return pow<P-1>(n) * T(n); 
		else {
			T s = pow<P/2>(n);
			return s * s;
		}
	}
	
	template<size_t P> static T sum_pows(int64_t n) {
		static_assert(P <= 3);
		if constexpr (P == 0) return n; else
		if constexpr (P == 1) {
			int64_t a = n, b = n + 1;
			if(a %2 == 0) a /= 2; else b /= 2;
			return T(a) * T(b);
		} else
		if constexpr (P == 2) {
			int64_t a = n, b = n + 1, c = 2 * n + 1;
			if(a %2 == 0) a /= 2; else b /= 2;
			if(a %3 == 0) a /= 3; else if(b %3 == 0) b /= 3; else c /= 3;
			return T(a) * T(b) * T(c);
		} else
		if constexpr (P == 3) {
			T s1 = sum_pows<1>(n);
			return s1 * s1;
		}
	}
};
