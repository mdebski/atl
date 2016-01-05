#include<cstdio>
#include<vector>
#include<algorithm>
#include<cmath>
#include<functional>

using namespace std;

template<typename NUM> NUM gcd(NUM a, NUM b) {
  NUM c;
  while ( a != 0 ) { c = a; a = b%a;  b = c; }
  return b;
}

template<typename NUM, int N> class PrimeArray {
  // an NUM->NUM array storying only at indexes co-prime to N,
  // and running predefined function for others.
private:
  vector<NUM> store;
  std::function<NUM(NUM)> fun;
  vector<NUM> offsets;
  NUM phi = 0;
  NUM size;

public:
  PrimeArray(NUM size, NUM val, std::function<NUM(NUM)> fun):
   fun(fun), size(size)
  {
    for(int i=0;i<N;++i) {
      if(gcd(i, N) == 1)
        offsets.push_back(phi++);
      else
        offsets.push_back(N);
    }

    store.resize(phi + phi*(size/N), val);
  }

  NUM get(NUM idx) {
    if(is_stored(idx))
     return store[get_idx(idx)];
    else
     return fun(idx);
  }

  void set(NUM idx, NUM val, bool ignore_bad = false) {
    if(is_stored(idx))
      store[get_idx(idx)] = val;
    else if(!ignore_bad)
      throw logic_error("Write to unstored value");
  }

  inline NUM get_idx(NUM idx) {
   NUM base = phi*(idx/N);
   NUM off = offsets[idx%N];
   // printf("%d -> %d (%d + %d) \n", idx, base+off, base, off);
   return base+off;
  }

  inline bool is_stored(NUM a) {
   return offsets[a%N] != N;
  }
};

template<typename NUM> class Sieve {
private:
 static constexpr NUM N = 2*3*5;
 static constexpr NUM start_prime = 7;

 PrimeArray<NUM, N> div;
 vector<bool> is_small_prime;

 static NUM get_small_div(NUM idx) {
  if(idx%2 == 0) return 2;
  if(idx%3 == 0) return 3;
  if(idx%5 == 0) return 5;
  throw logic_error("Get small div for unexpected index");
 }

 void simple_sieve(NUM n, vector<bool> * is_prime) {
   is_prime->resize(n+1, true);
   for(int i=2; i*i<=n; ++i)
     if(is_prime->at(i))
       for(int j=i*i; j<n; j+=i)
         (*is_prime)[j] = false;
 }

public:
 NUM limit;

 Sieve(NUM limit, int segment_size = 32768 / sizeof(NUM)):
   limit(limit),
   div(limit, 1, get_small_div)
 {
   simple_sieve((NUM) sqrt(limit), &is_small_prime);

   NUM sp = start_prime;
   vector<NUM> sps;  // small primes
   vector<NUM> next; // where to start for this small prime

   for(NUM low=0; low<=limit; low+=segment_size) {
     NUM high = min(low + segment_size - 1, limit);

     // add new small primes
     for(; sp*sp<=high; sp++) if(is_small_prime[sp]) {
       sps.push_back(sp);
       next.push_back(sp*sp);
     }

     for(NUM i=0; i<sps.size(); ++i)
       for(; next[i] < high; next[i] += sps[i])
         div.set(next[i], sps[i], true);
   }
 }

 NUM get_div(NUM n) {
   return div.get(n);
 }
};

int main() {
  unsigned n;
  scanf("%u", &n);
  Sieve<unsigned> s(n);

  printf("%d\n", s.get_div(17341*33331));
}
