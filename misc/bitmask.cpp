template<std::unsigned_integral T> struct mask_t {
	explicit mask_t(T ms = 0): ms(ms) {}
	static mask_t ones(size_t n) { return mask_t((T(1) << n) - 1); }
	static mask_t ones(size_t l, size_t r) { return ones(r) ^ ones(l); }
	
	bool contains(const mask_t &m) const { return (ms & m.ms) == m.ms; }
	
	#define mask_op(O, E, F) mask_t& operator E(const mask_t &b) { F return *this; } friend mask_t operator O(mask_t a, const mask_t &b) { return a E b; }
	mask_op(&, &=, ms &= b.ms; )
	mask_op(|, |=, ms |= b.ms; )
	mask_op(^, ^=, ms ^= b.ms; )
	
	friend bool operator==(const mask_t &a, const mask_t &b) = default;
	
	operator T() const { return ms; }
	const T& operator*() const { return ms; }
	
	struct bit_reference {
		mask_t &ms;
		size_t pos;
		operator bool() const { return ms.ms >> pos &1; }
		void operator=(bool value) {
			T bit = T(1) << pos;
			if(value) ms.ms |= bit;
			else ms.ms &= ~bit;
		}
	};
	
	bit_reference operator[](size_t i) { return {*this, i}; }
	bool operator[](size_t i) const { return ms >> i &1; }
	mask_t operator()(size_t l, size_t r) const { return *this & ones(l, r); }
	
	struct submask_iterator {
		T ms, cur;
		void operator++() { cur = (cur & ms) - 1; }
		bool operator!=(const submask_iterator &it) const { return cur != it.cur; }
		mask_t operator*() const { return mask_t(ms & cur); }
	};
	
	submask_iterator begin() const { return {ms, ms}; }
	submask_iterator end() const { return {ms, ~T(0)}; }
	
	private: T ms;
};
using mask = mask_t<uint32_t>;
