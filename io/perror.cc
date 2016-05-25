#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    const char *file_name = "NULL";
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        perror(file_name);
        exit(-1);
    }

    return 0;
}
