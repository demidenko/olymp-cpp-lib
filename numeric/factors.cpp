using uint128_t = __uint128_t;
template<class T> using square_t = conditional_t<sizeof(T) < sizeof(uint64_t), uint64_t, uint128_t>;

template<uint32_t... A, class T>
bool miller_rabin(T n) {
	assert(n > 1 && n % 2 == 1);
	auto test = [n](uint32_t s, square_t<T> a) {
		square_t<T> r = 1; 
		for(T d = n>>s; d; d>>=1, a = a*a %n) if(d&1) r = r*a %n;
		if(r == 1) return true;
		for(; s--; r = r*r %n) if(r == n-1) return true;
		return false;
	};
	uint32_t s = 1;
	while(~n >> s &1) ++s;
	return ((A >= n || test(s, A)) && ...);
}

template<class T>
auto is_prime(T n) -> enable_if_t<is_integral_v<T>, bool> {
	if(n < 2 || n % 2 == 0) return n == 2;
	if constexpr (is_signed_v<T>) return is_prime<make_unsigned_t<T>>(n);
	if constexpr (sizeof(T) > sizeof(uint32_t)) {
		if(n <= numeric_limits<uint32_t>::max()) return is_prime<uint32_t>(n);
		return miller_rabin<2, 325, 9375, 28178, 450775, 9780504, 1795265022>(n);
	} else return miller_rabin<2, 7, 61>(n);
}

template<class T>
auto pollard(T n, auto &res) -> enable_if_t<is_integral_v<T>, void> {
	assert(n > 0 && n % 2 == 1);
	if constexpr (is_signed_v<T>) return pollard<make_unsigned_t<T>>(n, res);
	if constexpr (sizeof(T) > sizeof(uint32_t)) {
		if(n <= numeric_limits<uint32_t>::max()) return pollard<uint32_t>(n, res);
	}
	
	if(n == 1) return ;
	if(is_prime(n)) { res.push_back(n); return ; }
	
	auto &&g = [n](square_t<T> x) -> T { return (x * x + 1) %n; };
	static mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
	
	for(;;) for(T x = rnd()%(n-3)+3, y = g(x); x != y; x=g(x), y=g(g(y)))
	if(const T d = gcd(x<y ? y-x : x-y, n); d > 1)
		return pollard<T>(d, res), pollard<T>(n / d, res);
}

template<class T>
vector<T> factors(T n) {
	assert(n > 0);
	vector<T> f;
	while(n%2 == 0) n/=2, f.push_back(2);
	for(T p = 3; p <= 37 && p*p <= n; p += 2) 
		while(n%p == 0) n/=p, f.push_back(p);
	pollard(n, f);
	return f;
}
