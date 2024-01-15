

#include <stdio.h>
#include <malloc.h>
#define rint register int
typedef unsigned short ushort;
char* TVA;
char* S;
int n;
int cmpPos(const void*pa,const void*pb){
    rint a = *((ushort*)pa);
    rint b = *((ushort*)pb);
    int r = 1;
    if(a>b){
        r = a;
        a = b;
        b = r;
        r = -1;
    }    
    char* VA = TVA+ a*10;
    char* VB = TVA + b*10;
    for(;b<n;a++,b++){
        int dr = (int)VA[S[a]] - (int)VB[S[b]];
        if(dr) return dr*r;
    }
    return r;
}

inline int sameLen(int pa,int pb){
    rint a = pa;
    rint b = pb;
    if(a>b){
        a ^= b;
        b ^= a;
        a ^= b;
    }
    pa = a;
    char* VA = TVA+a*10;
    char* VB = TVA+b*10;
    for(;b<n;a++,b++)
        if(VA[S[a]] != VB[S[b]]) return a - pa;
    return a-pa;
}

int main(void) {
    int q;
    scanf("%d %d",&n,&q);
    S = (char*)malloc(sizeof(char)*(n+1));
    scanf("%s",S);
    S[n] = -1;
    for(rint i=0;i<n;i++) S[i] -= 'a';
    TVA = (char*)malloc(sizeof(char)*(n+1)*10);
    for(rint i =0;i<10;i++) TVA[i+(n)*10] = i;
    for(rint i=n-1;i>=0;i--){
        char* TVAi = TVA + i*10;
        char sip = TVAi[S[i]+10];
        for(rint j=0;j<10;j++){
            if(TVAi[j+10] < sip) TVAi[j] = TVAi[j+10] + 1;
            else TVAi[j] = TVAi[j+10];
        }
        TVAi[S[i]] = 0;
    }
    ushort* SA = (ushort*)malloc(sizeof(ushort)*n);
    for(rint i=0;i<n;i++) SA[i] = (ushort)i;
    qsort(SA,n,sizeof(ushort),cmpPos);
    ushort* SB = (ushort*)malloc(sizeof(ushort)*n);
    ushort* KB = (ushort*)malloc(sizeof(ushort)*n);
    for(rint i=1;i<n;i++){
        SB[i] = sameLen(SA[i-1],SA[i]);
        KB[SA[i]] = i;
    }
    for(int w=0;w<q;w++){
        int x,y;
        scanf("%d %d",&x,&y);
        int d = y - x + 1;
        rint tx = KB[x-1];
        while(tx>0 && SB[tx]>=d) tx--;
        rint ty = KB[x-1]+1;
        while(ty<n && SB[ty]>=d) ty++;
        printf("%d\n",ty-tx);
    }
    return 0;
}
