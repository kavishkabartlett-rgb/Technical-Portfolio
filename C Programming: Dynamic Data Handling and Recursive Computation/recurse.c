#include <stdio.h>
#include <stdlib.h>

int recurse(int N) {
    if (N==0)
        return 2;
    else return 3*(N-1)+recurse(N-1)+1;
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Only type one input \n");
        return EXIT_FAILURE;
    }

    int N;
    if (sscanf(argv[1], "%d", &N) != 1 || N < 0) {
        printf("Invalid input. Please enter an integer greater than zero.\n");
        return EXIT_FAILURE;
    }

    int result = recurse(N);
    printf("%d\n", result);
    return EXIT_SUCCESS;
}
