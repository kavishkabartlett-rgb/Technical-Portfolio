#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Enter correct number of arguments (one integer)");
        return EXIT_FAILURE;
    }
    int N;
    if (sscanf(argv[1], "%d", &N) != 1) {
        return EXIT_FAILURE;
    }
    for (int i=1; i<= N; i++){
        int answer = i*7;
        printf("%d\n", answer);
    }
    return EXIT_SUCCESS;
}