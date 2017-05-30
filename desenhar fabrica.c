#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAXN 100
#define MAXTAM 160

double xe[MAXN], ye[MAXN], xd[MAXN], yd[MAXN];
char mat[MAXTAM][MAXTAM];

int main() {
    int n;
    scanf("%d", &n);
    int i, j, k;
    for (i=0; i<n; i++) {
        scanf("%lf %lf %lf %lf", &xe[i], &ye[i], &xd[i], &yd[i]);
    }
    memset(mat, ' ', sizeof(mat));
    for (i=0; i<n; i++) {
        for (j=floor(xe[i]+0.5); j<=floor(xd[i]+0.5); j++) {
            mat[(int) floor(yd[i]+0.5)][j]='a'+i;
            mat[(int) floor(ye[i]+0.5)][j]='a'+i;
        }
        for (j=floor(yd[i]+0.5); j<=floor(ye[i]+0.5); j++) {
            mat[j][(int) floor(xe[i]+0.5)]='a'+i;
            mat[j][(int) floor(xd[i]+0.5)]='a'+i;
        }
    }
    freopen("desenho.txt", "w", stdout);
    for (i=0; i<MAXTAM; i++) {
        for (j=0; j<MAXTAM; j++) {
            printf("%c", mat[i][j]);
        }
        printf("\n");
    }
    return 0;
}
