#include <stdio.h>

int main() {
    int Row = 1;
    int Col = 1;
    FILE * pFile;
    pFile = fopen ( "13.txt" , "rb" );

    /// Read file info

    fseek(pFile, 0L, SEEK_END);
    unsigned int sz = ftell(pFile);

    fseek(pFile, 1 - (sz % 300), SEEK_CUR);

    unsigned char Height;
    fread(&Height, 1, 1, pFile);

    printf("height : %u\n", Height);

    unsigned int Frame = 0;
    unsigned char temp;
    while (fread(&temp, 1, 1, pFile) != 0) {
        Frame *= 256;
        Frame += temp;
    }

    printf("frame : %u\n", Frame);

    unsigned char Width = sz / 3 / Height / Frame;
    printf("width : %u\n", Width);

    /// Prevent out of screen

    Row %= Height / 10;
    Col %= Width / 10;

    /// Load data, select and map pixel address

    fseek(pFile, (3 * Col * 10) + (Width * 3 * Row * 10), SEEK_SET);
    for (int frame = 0; frame < 5; frame++) {
        unsigned char selectBoxColor[100][3];

        /// Get Color Data

        for (int i = 0; i < 10; i++) {
            fread(selectBoxColor[i * 10], 1, 30, pFile);
            // Seek to next row
            fseek(pFile, (Width - 10) * 3, SEEK_CUR);
        }

        for (int i = 0; i < 100; i++) {
            if (i % 10 == 0) puts("\n");
            printf("[%3d %3d %3d] ", selectBoxColor[i][0], selectBoxColor[i][1], selectBoxColor[i][2]);
        }

        /// Map pixel address

        unsigned char out[100][3];
        for (int i = 0; i < 100; i++) {
            int to_i = 9 - i / 10;
            int to_j = to_i % 2 ? 9 - i % 10 : i % 10;
            memcpy(out[i], selectBoxColor[to_i * 10 + to_j], 3);
        }

        // Seek to next frame
        fseek(pFile, (Width * (Height - 10)) * 3, SEEK_CUR);
        puts("\n\n");
    }

    fclose(pFile);
}
