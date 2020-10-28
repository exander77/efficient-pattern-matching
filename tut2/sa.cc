#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// From:
//  Juha Karkkainen and Peter Sanders.
//  Simple linear work suffix array construction.
//  Proc. ICALP 2003.  pp 943
// Untouched other than adding a driver as the last function in the file

inline bool leq(int a1, int a2, int b1, int b2) {
  return(a1 < b1 || a1 == b1 && a2 <= b2); 
} 

inline bool leq(int a1, int a2, int a3, int b1, int b2, int b3) {
  return(a1 < b1 || a1 == b1 && leq(a2,a3, b2,b3)); 
} 

// stably sort a[0..n-1] to b[0..n-1] with keys in 0..K from r
static void radixPass(int* a, int* b, int* r, int n, int K) { 
  // count occurrences
  int* c = new int[K + 1]; // counter array
  for (int i = 0; i <= K; i++) c[i] = 0; // reset counters
  for (int i = 0; i < n; i++) c[r[a[i]]]++; // count occurrences
  for (int i = 0, sum = 0; i <= K; i++) // exclusive prefix sums
    {int t = c[i]; c[i] = sum; sum += t; }
  for (int i = 0; i < n; i++) b[c[r[a[i]]]++] = a[i]; // sort
  delete [] c;
}

// find the suffix array SA of s[0..n-1] in {1..K}n
// require s[n]=s[n+1]=s[n+2]=0, n>=2
void suffixArray(int* s, int* SA, int n, int K) {
  int n0=(n+2)/3, n1=(n+1)/3, n2=n/3, n02=n0+n2;
  int* s12 = new int[n02 + 3]; s12[n02]= s12[n02+1]= s12[n02+2]=0;
  int* SA12 = new int[n02 + 3]; SA12[n02]=SA12[n02+1]=SA12[n02+2]=0;
  int* s0 = new int[n0];
  int* SA0 = new int[n0];

  // generate positions of mod 1 and mod 2 suffixes
  // the "+(n0-n1)" adds a dummy mod 1 suffix if n%3 == 1
  for (int i=0, j=0; i < n+(n0-n1); i++) if (i%3 != 0) s12[j++] = i;
  // lsb radix sort the mod 1 and mod 2 triples
  radixPass(s12 , SA12, s+2, n02, K);
  radixPass(SA12, s12 , s+1, n02, K);
  radixPass(s12 , SA12, s , n02, K);

  // find lexicographic names of triples
  int name = 0, c0 = -1, c1 = -1, c2 = -1;
  for (int i = 0; i < n02; i++) {
    if (s[SA12[i]] != c0 || s[SA12[i]+1] != c1 || s[SA12[i]+2] != c2)
      {name++; c0 = s[SA12[i]]; c1 = s[SA12[i]+1]; c2 = s[SA12[i]+2]; }
    if (SA12[i] % 3 == 1) { s12[SA12[i]/3] = name; } // left half
    else {s12[SA12[i]/3 + n0] = name; } // right half
  }

  // recurse if names are not yet unique
  if (name < n02) {
    suffixArray(s12, SA12, n02, name);
    // store unique names in s12 using the suffix array
    for (int i = 0; i < n02; i++) s12[SA12[i]] = i + 1;
  } else // generate the suffix array of s12 directly
    for (int i = 0; i < n02; i++) SA12[s12[i] - 1] = i;

  // stably sort the mod 0 suffixes from SA12 by their first character
  for (int i=0, j=0; i < n02; i++) if (SA12[i] < n0) s0[j++] = 3*SA12[i];
  radixPass(s0, SA0, s, n0, K);

  // merge sorted SA0 suffixes and sorted SA12 suffixes
  for (int p=0, t=n0-n1, k=0; k < n; k++) {
#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)
    int i = GetI(); // pos of current offset 12 suffix
    int j = SA0[p]; // pos of current offset 0 suffix
    if (SA12[t] < n0 ? // different compares for mod 1 and mod 2 suffixes
	leq(s[i], s12[SA12[t] + n0], s[j], s12[j/3]) :
	leq(s[i],s[i+1],s12[SA12[t]-n0+1], s[j],s[j+1],s12[j/3+n0]))
      {// suffix from SA12 is smaller
	SA[k] = i; t++;
	if (t == n02) // done --- only SA0 suffixes left
	  for (k++; p < n0; p++, k++) SA[k] = SA0[p];
      } else {// suffix from SA0 is smaller
      SA[k] = j; p++;
      if (p == n0) // done --- only SA12 suffixes left
	for (k++; t < n02; t++, k++) SA[k] = GetI();
    }
  }
  delete [] s12; delete [] SA12; delete [] SA0; delete [] s0;
}

int* suffixArray(unsigned char* s, int n) { 
  int *ss = new int[n+3]; 
  int *SA = new int[n];
  ss[n] = ss[n+1] = ss[n+2] = 0;
  int mx = 0;
  for (int i=0; i < n; i++) 
    if (s[i] > mx) mx = s[i];

  // add 1 incase any of the characters in the file are null
  for (int i=0; i < n; i++) ss[i] = s[i]+1;

  suffixArray(ss, SA, n, mx+2);
  delete [] ss;
  return (int*)SA;
}

int* lcpArray(unsigned char *s, int n, int* sa) {
	int *lcp = new int[n];
	int *rank = new int[n];

	lcp[0] = 0;
	for (int i = 0; i < n; ++i)
		rank[sa[i]] = i;

	int h = 0;
	for (int i = 0; i < n; ++i)
		if (rank[i] > 0) {
			int j = sa[rank[i]-1];

			while (s[i+h] == s[j+h]) h++;
			lcp[rank[i]] = h;
			if(h > 0) h--;
		}
	return lcp;
}


int main (int argc, char *argv[]) {
    if (argc < 2) return 1;
    
    char* s = argv[1];
    int n = strlen(s);
    printf("Length: %3u\n", n);
    printf("Index:  ");
		for (int i=0;i<n;++i) {
        printf("%3i ", i);
    }
    printf("\n");
    printf("String:");
    for (int i=0;i<n;++i) {
        printf("   %c", s[i]);
    }
    printf("\n");
    int* sa = suffixArray((unsigned char*)s, n);
    printf("SA:     ");
    for (int i=0;i<n;++i) {
        printf("%3i ", sa[i]);
    }
    printf("\n");
    int* lcp = lcpArray((unsigned char*)s, n, sa);
    printf("LCP:    ");
    for (int i=0;i<n;++i) {
        printf("%3i ", lcp[i]);
    }
    printf("\n");
    free(sa);  

    return 0;
}
// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
