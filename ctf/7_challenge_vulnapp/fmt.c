#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char FLAG[] = "CTF{pwn_easy_format_string}";

int main(){
    setvbuf(stdout, NULL, _IONBF, 0);
    char buf[256];
    puts("Welcome to fmt server. Enter input:");
    if(!fgets(buf, sizeof(buf), stdin)) return 1;
    buf[strcspn(buf, "\n")] = 0;
    printf(buf); // Vulnerable
    puts("\nBye.");
    return 0;
}
