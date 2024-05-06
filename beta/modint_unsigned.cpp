template<decltype(auto) mod> requires same_as<decay_t<decltype(mod)>, uint32_t>
struct modint {
	modint(): x(0) {}
	modint(int64_t val): x(val%mod) { if(x<0) x+=mod; }
	modint(unsigned_integral auto val): x(val%mod) {}
	static constexpr uint32_t get_mod() { return mod; }
	#define __op(O, E, F) modint& operator E(const modint &b) { F return *this; } friend modint operator O(modint a, const modint &b) { return a E b; }
	__op(+, +=,  if(uint32_t v=mod-b.x; x >= v) x -= v; else x += b.x; )
	__op(-, -=,  if(x < b.x) x += mod-b.x; else x -= b.x; )
	__op(*, *=,  x = uint64_t(x)*b.x %mod; )
	__op(/, /=,  auto i=b.inverse(); assert(i); x = uint64_t(i->x)*x %mod; )
	friend modint operator-(modint a) { if(a.x) a.x = mod - a.x; return a; }
	friend modint pow(modint a, uint64_t n) { modint p=1; for(; n; n>>=1, a*=a) if(n&1) p*=a; return p; }
	optional<modint> inverse() const {
		uint32_t a = x, m = mod, p = 1, q = 0; bool q_neg = true;
		while(a) swap(p, q += (m / a) * p), swap(a, m %= a), q_neg ^= true;
		if(m == 1) return q_neg ? mod - q : q; else return nullopt;
	}
	bool operator==(const modint&) const = default;
	friend ostream& operator<<(ostream &o, const modint &m) { return o<<m.x; }
	const uint32_t& operator*() const { return x; }
	private: uint32_t x;
};
