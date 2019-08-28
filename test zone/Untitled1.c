#include<stdio.h>

int main() {
    int a[100][3];
    for (int i = 0; i < 100; i++) {
        a[i][0] = i / 10;
        a[i][1] = i % 10;
        a[i][2] = i / 10 + i % 10;
    }

    int out[100][3];
    for (int i = 0; i < 100; i++) {
        int to_i = 9 - i / 10;
        int to_j = to_i % 2 ? 9 - i % 10 : i % 10;
        out[i][0] = a[to_i * 10 + to_j][0];
        out[i][1] = a[to_i * 10 + to_j][1];
        out[i][2] = a[to_i * 10 + to_j][2];
    }

    for (int i = 0; i < 100; i ++) {
        if (i%10 == 0)
            printf("\n");
        printf("[%d, %d, %2d]   ", out[i][0], out[i][1], out[i][2]);
    }
}
