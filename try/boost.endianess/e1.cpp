#include <cstdint>
#include <cstdio>

#include <fstream>

void f1() {
    int16_t i = 0x0102;
    FILE *file = fopen("test.bin", "wb");   // binary file!
    fwrite(&i, sizeof(int16_t), 1, file);
    fclose(file);
}

void f2() {
    int16_t i = 0x0102;
    std::ofstream outfile("test2.bin", std::ios::out | std::ios::binary);
    outfile.write(reinterpret_cast<char *>(&i), sizeof(i));
};

int main() {
    f2();
};
