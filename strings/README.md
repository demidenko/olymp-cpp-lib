# Hasher
Polynomial hashing with MOD = 2**61 - 1 and random BASE

```hash_t``` - hash integer value

```hashed``` - hashed string

```c++
hashed h = str;
h.length // length of hashed string
*h // hash of string
if(h1 == h2) // equality check in O(1)
h = h1 + h2; // concatenations in O(1)
h += 'a';
```

```hasher``` - hash array of string/vector

```hash_span``` - reference to some range in hasher

```c++
hasher a(str);
a.substr(start, length); // hashed
a.subhash(start, length); // hash_t
hash_span s = a.subspan(start, length); 
s.length();
assert(a[s.start()] == s[0]);

if(s1 == s2) // equality in O(1)
if(s1 < s2) // operator< in O(log(Length))
lcp(s1, s2) // longest common prefix in O(log(Length))

hashed h = s1 + s2; // concatenation of spans is hashed
```
