#include<cstdio>
#include<vector>
#include<algorithm>
#include<cmath>

using namespace std;

template<typename NUM> class Sieve {
private:
 vector<NUM> div;

 vector<bool> is_small_prime;

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
   div(limit, 1)
 {
   simple_sieve((NUM) sqrt(limit), &is_small_prime);

   NUM sp = 2;
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
         div[next[i]] = sps[i];
   }
 }

 NUM get_div(NUM n) {
   return div.at(n);
 }
};

int main() {
  unsigned n;
  scanf("%u", &n);
  Sieve<unsigned> s(n);

  printf("%d\n", s.get_div(17341*33331));
}
