template<class T> pair<T, T> fib_pair(uint64_t n) {
	if (n == 0) return {0, 1};
	if (n&1) {
		auto [x, y] = fib_pair<T>(n-1);
		return {y, x+y};
	}
	auto [x, y] = fib_pair<T>(n>>1);
	return {x*y + x*(y-x), x*x + y*y};
}
template<class T> inline T fibonacci(uint64_t n) { return fib_pair<T>(n).first; }
