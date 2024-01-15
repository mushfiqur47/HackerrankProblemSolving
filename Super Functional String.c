

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define A_SIZE 26
#define MIN_C 'a'
#define MOD 1000000007
typedef struct _st_node st_node;
typedef struct _st_edge st_edge;
struct _st_node{
  st_node *suffix_link;
  st_edge *edges[A_SIZE+1];
};
struct _st_edge{
  int from;
  int to;
  int suffix_index;
  st_node *node;
};
void print(st_node *root,int len);
void suffix_tree(st_node *root,char *str,int len);
long long modPow(long long a,int x);
void sort_a(int*a,int size);
void merge(int*a,int*left,int*right,int left_size, int right_size);
char str[100001];
int dp[100000][26];
long long ans,pows[26][100001];

int main(){
  int T,len,i,j;
  st_node root;
  for(i=0;i<26;i++)
    for(j=1;j<=100000;j++)
      pows[i][j]=(pows[i][j-1]+modPow(j,i+1))%MOD;
  scanf("%d",&T);
  while(T--){
    scanf("%s",str);
    len=strlen(str);
    for(i=0;i<26;i++)
      dp[len-1][i]=-1;
    dp[len-1][str[len-1]-MIN_C]=len-1;
    for(i=len-2;i>=0;i--){
      memcpy(&dp[i][0],&dp[i+1][0],26*sizeof(int));
      dp[i][str[i]-MIN_C]=i;
    }
    suffix_tree(&root,str,len);
    ans=0;
    print(&root,0);
    printf("%lld\n",ans);
  }
  return 0;
}
void print(st_node *root,int len){
  int i,j,idx,from,to,s,dc,last,t,a[26];
  if(!root)
    return;
  for(i=0;i<A_SIZE;i++)
    if(root->edges[i]){
      idx=root->edges[i]->suffix_index;
      from=idx+len;
      to=idx+len+root->edges[i]->to-root->edges[i]->from;
      s=dc=0;
      last=idx+len-1;
      for(j=0;j<26;j++)
        if(dp[idx][j]!=-1 && dp[idx][j]<from)
          dc++;
        else if(dp[idx][j]>=from && dp[idx][j]<=to)
          a[s++]=dp[idx][j];
      sort_a(a,s);
      for(j=0;j<s;j++,dc++){
        t=a[j]-1;
        if(dc)
          ans=(ans+pows[dc-1][t-idx+1]-pows[dc-1][last-idx+1]+MOD)%MOD;
        last=t;
      }
      t=to;
      ans=(ans+pows[dc-1][t-idx+1]-pows[dc-1][last-idx+1]+MOD)%MOD;
      print(root->edges[i]->node,len+root->edges[i]->to-root->edges[i]->from+1);
    }
  return;
}
void suffix_tree(st_node *root,char *str,int len){
  int a_edge,a_len=0,remainder=0,need_insert,from,i;
  st_node *a_node=root,*pre_node,*t_node;
  st_edge *t_edge;
  memset(root,0,sizeof(st_node));
  for(i=0;i<=len;i++){
    need_insert=0;
    pre_node=NULL;
    remainder++;
    if(i==len)
      need_insert=1;
    else if(a_len)
      if(str[a_node->edges[a_edge]->from+a_len]==str[i])
        if(a_node->edges[a_edge]->from+a_len==a_node->edges[a_edge]->to){
          a_node=a_node->edges[a_edge]->node;
          a_len=0;
        }
        else
          a_len++;
      else
        need_insert=1;
    else
      if(a_node->edges[str[i]-MIN_C])
        if(a_node->edges[str[i]-MIN_C]->from==a_node->edges[str[i]-MIN_C]->to)
          a_node=a_node->edges[str[i]-MIN_C]->node;
        else{
          a_edge=str[i]-MIN_C;
          a_len=1;
        }
      else
        need_insert=1;
    if(need_insert)
      for(;remainder>0;remainder--){
        if(!a_len)
          if(i==len){
            a_node->edges[A_SIZE]=(st_edge*)malloc(sizeof(st_edge));
            a_node->edges[A_SIZE]->suffix_index=i-remainder+1;
            a_node->edges[A_SIZE]->node=NULL;
          }
          else{
            if(a_node->edges[str[i]-MIN_C]){
              if(pre_node)
                pre_node->suffix_link=a_node;
              if(a_node->edges[str[i]-MIN_C]->from==a_node->edges[str[i]-MIN_C]->to)
                a_node=a_node->edges[str[i]-MIN_C]->node;
              else{
                a_edge=str[i]-MIN_C;
                a_len=1;
              }
              break;
            }
            t_edge=(st_edge*)malloc(sizeof(st_edge));
            t_edge->from=i;
            t_edge->to=len-1;
            t_edge->suffix_index=i-remainder+1;
            t_edge->node=NULL;
            a_node->edges[str[i]-MIN_C]=t_edge;
            t_node=a_node;
          }
        else{
          if(i!=len && str[a_node->edges[a_edge]->from+a_len]==str[i]){
            if(pre_node)
              pre_node->suffix_link=a_node;
            if(a_node->edges[a_edge]->from+a_len==a_node->edges[a_edge]->to){
              a_node=a_node->edges[a_edge]->node;
              a_len=0;
            }
            else
              a_len++;
            break;
          }
          t_node=(st_node*)malloc(sizeof(st_node));
          memset(t_node,0,sizeof(st_node));
          t_edge=(st_edge*)malloc(sizeof(st_edge));
          t_edge->from=a_node->edges[a_edge]->from+a_len;
          t_edge->to=a_node->edges[a_edge]->to;
          t_edge->suffix_index=a_node->edges[a_edge]->suffix_index;
          t_edge->node=a_node->edges[a_edge]->node;
          from=a_node->edges[a_edge]->from;
          a_node->edges[a_edge]->node=t_node;
          a_node->edges[a_edge]->to=a_node->edges[a_edge]->from+a_len-1;
          t_node->edges[str[t_edge->from]-MIN_C]=t_edge;
          if(i==len){
            t_node->edges[A_SIZE]=(st_edge*)malloc(sizeof(st_edge));
            t_node->edges[A_SIZE]->suffix_index=i-remainder+1;
            t_node->edges[A_SIZE]->node=NULL;
          }
          else{
            t_edge=(st_edge*)malloc(sizeof(st_edge));
            t_edge->from=i;
            t_edge->to=len-1;
            t_edge->suffix_index=i-remainder+1;
            t_edge->node=NULL;
            t_node->edges[str[i]-MIN_C]=t_edge;
          }
        }
        if(pre_node)
          pre_node->suffix_link=t_node;
        pre_node=t_node;
        if(a_node==root && a_len>0){
          if(remainder>1)
            a_edge=str[i-remainder+2]-MIN_C;
          from=i-remainder+2;
          a_len--;
        }
        else if(a_node!=root)
          if(a_node->suffix_link)
            a_node=a_node->suffix_link;
          else
            a_node=root;
        while(a_len>0 && a_len>=a_node->edges[a_edge]->to-a_node->edges[a_edge]->from+1){
          a_len-=a_node->edges[a_edge]->to-a_node->edges[a_edge]->from+1;
          from+=a_node->edges[a_edge]->to-a_node->edges[a_edge]->from+1;
          a_node=a_node->edges[a_edge]->node;
          a_edge=str[from]-MIN_C;
        }
      }
  }
  return;
}
long long modPow(long long a,int x){
  long long res = 1;
  while(x>0){
    if(x%2)
      res=(res*a)%MOD;
    a=(a*a)%MOD;
    x>>=1;
  }
  return res;
}
void sort_a(int*a,int size){
  if (size < 2)
    return;
  int m = (size+1)/2,i;
  int *left,*right;
  left=(int*)malloc(m*sizeof(int));
  right=(int*)malloc((size-m)*sizeof(int));
  for(i=0;i<m;i++)
    left[i]=a[i];
  for(i=0;i<size-m;i++)
    right[i]=a[i+m];
  sort_a(left,m);
  sort_a(right,size-m);
  merge(a,left,right,m,size-m);
  free(left);
  free(right);
  return;
}
void merge(int*a,int*left,int*right,int left_size, int right_size){
    int i = 0, j = 0;
    while (i < left_size|| j < right_size) {
        if (i == left_size) {
            a[i+j] = right[j];
            j++;
        } else if (j == right_size) {
            a[i+j] = left[i];
            i++;
        } else if (left[i] <= right[j]) {
            a[i+j] = left[i];
            i++;                
        } else {
            a[i+j] = right[j];
            j++;
        }
    }
    return;
}





//


This code implements an algorithm to solve a specific problem related to suffix trees. The problem is not explicitly described in the code, but it appears to involve counting certain substrings in a given string. Let's go through the code line by line to explain its functionality:

1. **Header Files:**
   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   ```
   Include standard C libraries for input/output, dynamic memory allocation, and string manipulation.

2. **Macros and Constants:**
   ```c
   #define A_SIZE 26
   #define MIN_C 'a'
   #define MOD 1000000007
   ```
   Define some constants: `A_SIZE` is the size of the alphabet (assuming lowercase English letters), `MIN_C` is the minimum character ('a'), and `MOD` is a large prime number used for modular arithmetic.

3. **Custom Data Types:**
   ```c
   typedef struct _st_node st_node;
   typedef struct _st_edge st_edge;
   ```
   Define structures for nodes and edges in the suffix tree.

4. **Node Structure:**
   ```c
   struct _st_node {
       st_node *suffix_link;
       st_edge *edges[A_SIZE + 1];
   };
   ```
   Each node in the suffix tree has a suffix link to another node and an array of edges representing transitions to other nodes.

5. **Edge Structure:**
   ```c
   struct _st_edge {
       int from;
       int to;
       int suffix_index;
       st_node *node;
   };
   ```
   An edge represents a substring from `from` to `to` in the input string. `suffix_index` stores the starting index of the suffix represented by this edge, and `node` points to the next node.

6. **Function Prototypes:**
   ```c
   void print(st_node *root, int len);
   void suffix_tree(st_node *root, char *str, int len);
   long long modPow(long long a, int x);
   void sort_a(int *a, int size);
   void merge(int *a, int *left, int *right, int left_size, int right_size);
   ```

7. **Global Variables:**
   ```c
   char str[100001];
   int dp[100000][26];
   long long ans, pows[26][100001];
   ```
   Declare a global character array `str`, a 2D array `dp`, and some global variables to store the result (`ans`) and precomputed powers (`pows`).

8. **Main Function:**
   ```c
   int main() {
       // ...
   }
   ```
   Entry point of the program.

9. **Precompute Powers of Integers:**
   ```c
   for (i = 0; i < 26; i++)
       for (j = 1; j <= 100000; j++)
           pows[i][j] = (pows[i][j - 1] + modPow(j, i + 1)) % MOD;
   ```
   Precompute powers of integers from 1 to 100000 for each alphabet letter and store them in `pows` array.

10. **Number of Test Cases:**
    ```c
    scanf("%d", &T);
    ```
    Read the number of test cases.

11. **Test Case Loop:**
    ```c
    while (T--) {
        // ...
    }
    ```

12. **String Input and Initialization:**
    ```c
    scanf("%s", str);
    len = strlen(str);
    ```
    Read the input string and calculate its length. 

13. **Dynamic Programming Table Initialization:**
    ```c
    for (i = 0; i < 26; i++)
        dp[len - 1][i] = -1;
    dp[len - 1][str[len - 1] - MIN_C] = len - 1;
    for (i = len - 2; i >= 0; i--) {
        // ...
    }
    ```
    Initialize a dynamic programming table `dp` to store the last occurrence index of each character in the string.

14. **Suffix Tree Construction:**
    ```c
    suffix_tree(&root, str, len);
    ```

15. **Initialize Answer and Print Result:**
    ```c
    ans = 0;
    print(&root, 0);
    printf("%lld\n", ans);
    ```

16. **Print Function:**
    ```c
    void print(st_node *root, int len) {
        // ...
    }
    ```
    The `print` function processes the constructed suffix tree and calculates the final answer.

17. **Suffix Tree Construction Function:**
    ```c
    void suffix_tree(st_node *root, char *str, int len) {
        // ...
    }
    ```
    The `suffix_tree` function constructs the suffix tree based on the Ukkonen's algorithm.

18. **Modular Exponentiation Function:**
    ```c
    long long modPow(long long a, int x) {
        // ...
    }
    ```
    The `modPow` function calculates modular exponentiation.

19. **Sorting Function:**
    ```c
    void sort_a(int *a, int size) {
        // ...
    }
    ```
    The `sort_a` function sorts an array of integers using merge sort.

20. **Merge Function:**
    ```c
    void merge(int *a, int *left, int *right, int left_size, int right_size) {
        // ...
    }
    ```
    The `merge` function merges two sorted arrays during the merge sort process.

This code combines various algorithms and data structures to solve a specific problem related to suffix trees and dynamic programming. The specific problem and the nature of the output are not explicitly mentioned in the code.
