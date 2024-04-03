template<class T> T fibonacci(uint64_t n) { 
	T x = 0, y = 1;
	for(auto i=bit_width(n); i--; ) {
		T xx = x*x, xy = x*y, yy = y*y;
		if(n>>i&1) x = xx + yy, y = xy + xy + yy;
		else x = xy + xy - xx, y = xx + yy;
	}
	return x;
}
