#include <stdio.h>
#include <string.h>

int main(){
    char input[64];
    printf("Enter key: ");
    if(!fgets(input, sizeof(input), stdin)) return 1;
    // strip newline
    input[strcspn(input, "\n")] = 0;
    if(strcmp(input, "reverserocks") == 0){
        printf("CTF{reverse_basic_check}\n");
    } else {
        printf("Wrong!\n");
    }
    return 0;
}
