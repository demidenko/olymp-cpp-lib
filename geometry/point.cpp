template<class T, class M = T>
struct point_t {
	T x, y;
	point_t(): x(0), y(0) {}
	point_t(const T&x, const T&y): x(x), y(y) {}
	
	point_t& operator+=(const point_t &p) { x+=p.x; y+=p.y; return *this; }
	friend point_t operator+(point_t a, const point_t &b) { return a+=b; }
	
	point_t& operator-=(const point_t &p) { x-=p.x; y-=p.y; return *this; }
	friend point_t operator-(point_t a, const point_t &b) { return a-=b; }
	
	point_t& operator*=(auto &&k) { x*=k; y*=k; return *this; }
	friend point_t operator*(point_t a, auto &&k) { return a*=k; }
	
	point_t operator-() const { return point_t{-x, -y}; }
	
	auto operator<=>(const point_t &p) const = default;
	
	friend istream& operator>>(istream&i, point_t &p) { return i >> p.x >> p.y; }
	
	friend auto sp(const point_t &a, const point_t &b) {
		return (M)a.x * b.x + (M)a.y * b.y;
	}
	
	friend auto vp(const point_t &a, const point_t &b) {
		return (M)a.x * b.y - (M)a.y * b.x;
	}
	
	friend auto vp(const point_t &a, const point_t &b, const point_t &c) {
		return vp(a - c, b - c);
	}
};

using point = point_t<int32_t, int64_t>;
ostream& operator<<(ostream&o, const point &p) { return o << "(" << p.x << ", " << p.y << ")"; }
