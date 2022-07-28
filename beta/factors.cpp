using int128_t = __int128_t;
template<class T> using Squared = conditional_t<is_same_v<T,int64_t>,int128_t,int64_t>;

template<class T, int... A>
bool miller_rabin_multitest(T n) {
	assert(n > 1 && n % 2 == 1);
	auto miller_rabin_test = [n](T d, uint32_t s, Squared<T> a) {
		Squared<T> r = 1; 
		for(; d; d>>=1, a = a*a %n) if(d&1) r = r*a %n;
		for(; s--; r = r*r %n) if(r == 1 || r == n-1) return true;
		return false;
	};
	T d = n-1; 
	uint32_t s = 0;
	while(d%2 == 0) d/=2, ++s;
	return ((A >= n || miller_rabin_test(d, s, A)) && ...);
}

template<class T>
bool is_prime(T n) {
	if(n < 2) return false;
	if(n%2 == 0) return n == 2;
	if constexpr (!is_same_v<T,int32_t>) {
		if(n <= numeric_limits<int32_t>::max()) return is_prime((int32_t)n);
	}
	if constexpr (is_same_v<T,int32_t>)
		return miller_rabin_multitest<T, 2, 7, 61>(n);
	else
		return miller_rabin_multitest<T, 2, 325, 9375, 28178, 450775, 9780504, 1795265022>(n);
}

template<class T>
auto pollard_fact(T n) -> enable_if_t<is_signed_v<T>, vector<T>> {
	assert(n > 0 && n % 2 == 1);
	if(n == 1) return {};
	if(is_prime(n)) return {n};
	
	if constexpr (!is_same_v<T,int32_t>) {
		if(n <= numeric_limits<int32_t>::max()) {
			auto &&f2 = pollard_fact((int32_t)n);
			return {begin(f2), end(f2)};
		}
	}
	
	auto&& g = [n](Squared<T> x) -> T { return (x * x + 1) %n; };
	static mt19937 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());
	
	for(;;) {
		for(T x = rnd()%(n-1)+1, y = g(x); ; x=g(x), y=g(g(y))) {
			const T d = gcd(abs(y-x), n);
			if(d == 1) continue ;
			if(x == y || d == n) break ;
			vector<T> f;
			for(T p : pollard_fact(min(d, n/d))) while(n%p == 0) n/=p, f.push_back(p);
			for(T p : pollard_fact(n)) f.push_back(p);
			return f;
		}
	}
}

template<class T>
vector<T> factorize(T n) {
	assert(n > 0);
	vector<T> f;
	while(n%2 == 0) n/=2, f.push_back(2);
	for(int32_t p=3; p<=37 && p*p<=n; p+=2) {
		while (n%p == 0) n/=p, f.push_back(p);
	}
	for(T p : pollard_fact(n)) f.push_back(p);
	return f;
}
