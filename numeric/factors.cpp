using uint128_t = __uint128_t;
template<class T> using Squared = conditional_t<sizeof(T) == 8, uint128_t, uint64_t>;

template<class T, int... A>
bool miller_rabin(T n) {
	assert(n > 1 && n % 2 == 1);
	auto test = [n](T d, uint32_t s, Squared<T> a) {
		Squared<T> r = 1; 
		for(; d; d>>=1, a = a*a %n) if(d&1) r = r*a %n;
		if(r == 1) return true;
		for(; s--; r = r*r %n) if(r == n-1) return true;
		return false;
	};
	T d = n-1;
	uint32_t s = 0;
	while(d%2 == 0) d/=2, ++s;
	return ((A >= n || test(d, s, A)) && ...);
}

template<class T>
auto is_prime(T n) -> enable_if_t<is_integral_v<T>, bool> {
	if(n < 2) return false;
	if(n%2 == 0) return n == 2;
	if constexpr (is_signed_v<T>) return is_prime<make_unsigned_t<T>>(n);
	if constexpr (sizeof(T) > sizeof(uint32_t)) {
		if(n <= numeric_limits<uint32_t>::max()) return is_prime<uint32_t>(n);
		return miller_rabin<T, 2, 325, 9375, 28178, 450775, 9780504, 1795265022>(n);
	} else return miller_rabin<T, 2, 7, 61>(n);
}

template<class T, class P = T>
auto pollard(T n) -> enable_if_t<is_integral_v<T>, vector<P>> {
	assert(n > 0 && n % 2 == 1);
	if constexpr (is_signed_v<T>) return pollard<make_unsigned_t<T>,P>(n);
	if(n == 1) return {};
	if(is_prime(n)) return {P(n)};
	
	if constexpr (sizeof(T) > sizeof(uint32_t)) {
		if(n <= numeric_limits<uint32_t>::max()) return pollard<uint32_t,P>(n);
	}
	
	auto &&g = [n](Squared<T> x) -> T { return (x * x + 1) %n; };
	static mt19937 rnd(chrono::high_resolution_clock::now().time_since_epoch().count());
	
	for(;;) for(T x = rnd()%(n-3)+3, y = g(x); x != y; x=g(x), y=g(g(y)))
	if(const T d = gcd(x<y ? y-x : x-y, n); d == n) break; else if(d > 1) {
		vector<P> f;
		for(P p : pollard<T,P>(min(d,n/d))) while(n%p == 0) n/=p, f.push_back(p);
		for(P p : pollard<T,P>(n)) f.push_back(p);
		return f;
	}
}

template<class T>
vector<T> factors(T n) {
	assert(n > 0);
	vector<T> f;
	while(n%2 == 0) n/=2, f.push_back(2);
	for(T p=3; p <= 37 && p*p <= n; p += 2) 
		while(n%p == 0) n/=p, f.push_back(p);
	for(T p : pollard(n)) f.push_back(p);
	return f;
}
