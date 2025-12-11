// program_obf.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
  #include <windows.h>
#endif

// ---------- Configuration d'obfuscation ----------
static const unsigned char XOR_KEY = 0x5A; // clef simple pour XOR

// NOTE: chaque chaîne est stockée ici chiffrée (XOR). Elle sera déchiffrée au runtime.
static unsigned char enc_master_pass[] = {
    'Z'^XOR_KEY, '!'^XOR_KEY, 'i'^XOR_KEY, '2'^XOR_KEY, '4'^XOR_KEY, 'c'^XOR_KEY,
    'Q'^XOR_KEY, 'g'^XOR_KEY, '!'^XOR_KEY, 'n'^XOR_KEY, 'Z'^XOR_KEY, 'm'^XOR_KEY,
    '?'^XOR_KEY, 0 ^ XOR_KEY
};

static unsigned char enc_secret_0[] = {
    'a'^XOR_KEY,'l'^XOR_KEY,'i'^XOR_KEY,'c'^XOR_KEY,'e'^XOR_KEY,'@'^XOR_KEY,
    'e'^XOR_KEY,'x'^XOR_KEY,'a'^XOR_KEY,'m'^XOR_KEY,'p'^XOR_KEY,'l'^XOR_KEY,
    'e'^XOR_KEY,'.'^XOR_KEY,'c'^XOR_KEY,'o'^XOR_KEY,'m'^XOR_KEY,' '^XOR_KEY,
    ':'^XOR_KEY,' '^XOR_KEY,' '^XOR_KEY,'a'^XOR_KEY,'1'^XOR_KEY,'!'^XOR_KEY,
    'c'^XOR_KEY,'3'^XOR_KEY,'_'^XOR_KEY,'p'^XOR_KEY,'w'^XOR_KEY, 0 ^ XOR_KEY
};

static unsigned char enc_secret_1[] = {
    'b'^XOR_KEY,'o'^XOR_KEY,'b'^XOR_KEY,'@'^XOR_KEY,'e'^XOR_KEY,'x'^XOR_KEY,
    'a'^XOR_KEY,'m'^XOR_KEY,'p'^XOR_KEY,'l'^XOR_KEY,'e'^XOR_KEY,'.'^XOR_KEY,
    'c'^XOR_KEY,'o'^XOR_KEY,'m'^XOR_KEY,' '^XOR_KEY,':'^XOR_KEY,' '^XOR_KEY,
    'B'^XOR_KEY,'0'^XOR_KEY,'b'^XOR_KEY,'S'^XOR_KEY,'e'^XOR_KEY,'c'^XOR_KEY,
    'u'^XOR_KEY,'r'^XOR_KEY,'e'^XOR_KEY,'#'^XOR_KEY, 0 ^ XOR_KEY
};

static unsigned char enc_secret_2[] = {
    's'^XOR_KEY,'e'^XOR_KEY,'r'^XOR_KEY,'v'^XOR_KEY,'e'^XOR_KEY,'r'^XOR_KEY,
    ' '^XOR_KEY,'r'^XOR_KEY,'o'^XOR_KEY,'o'^XOR_KEY,'t'^XOR_KEY,' '^XOR_KEY,
    ':'^XOR_KEY,' '^XOR_KEY,'r'^XOR_KEY,'0'^XOR_KEY,'0'^XOR_KEY,'t'^XOR_KEY,
    'A'^XOR_KEY,'d'^XOR_KEY,'m'^XOR_KEY,'1'^XOR_KEY,'n'^XOR_KEY,'$'^XOR_KEY,
    0 ^ XOR_KEY
};

static unsigned char *enc_listed[] = {
    enc_secret_0,
    enc_secret_1,
    enc_secret_2,
    NULL
};

// ---------- Flag obfusqué (XOR) ----------
static unsigned char enc_flag[] = {
    'C'^XOR_KEY,'T'^XOR_KEY,'F'^XOR_KEY,'{'^XOR_KEY,
    'r'^XOR_KEY,'e'^XOR_KEY,'v'^XOR_KEY,'e'^XOR_KEY,'r'^XOR_KEY,'s'^XOR_KEY,'e'^XOR_KEY,'_'^XOR_KEY,
    'b'^XOR_KEY,'a'^XOR_KEY,'s'^XOR_KEY,'i'^XOR_KEY,'c'^XOR_KEY,'_'^XOR_KEY,'c'^XOR_KEY,'h'^XOR_KEY,'e'^XOR_KEY,'c'^XOR_KEY,'k'^XOR_KEY,
    '}'^XOR_KEY, 0 ^ XOR_KEY
};

// ---------- Helpers ----------
static void xor_inplace(unsigned char *buf, size_t n) {
    // volatile pour limiter certaines optimisations agressives
    volatile unsigned char *p = (volatile unsigned char *)buf;
    for (size_t i = 0; i < n; ++i) p[i] = p[i] ^ XOR_KEY;
}

static size_t cstr_len_xored(const unsigned char *enc) {
    // mesurer la "longueur" en regardant jusqu'au 0^XOR_KEY
    size_t i = 0;
    while ((enc[i] ^ XOR_KEY) != '\0') i++;
    return i;
}

// une copie sûre : déchiffre la source chiffrée dans un buffer nouvellement alloué
static char *decrypt_to_newbuf(const unsigned char *enc) {
    size_t n = cstr_len_xored(enc) + 1;
    char *buf = malloc(n);
    if (!buf) return NULL;
    for (size_t i = 0; i < n; ++i) buf[i] = (char)(enc[i] ^ XOR_KEY);
    return buf;
}

// ---------- Junk / noise functions (ne changent pas le comportement) ----------
static int junk_compute(int seed) {
    int a = seed ^ 0x1337;
    for (int i = 0; i < 7; ++i) {
        a = ((a << 3) | (a >> (29))) ^ (a + 0x9e3779b9);
    }
    return a ^ seed;
}

static void junk_void(int x) {
    // pas d'effet observé, juste pour polluer l'analyse statique
    volatile int v = junk_compute(x);
    (void)v;
}

// ---------- Anti-debug (Windows) ----------
static int detect_debugger(void) {
#if defined(_WIN32) || defined(_WIN64)
    // possibilité de contourner pendant le debug en exportant BUNKER_NO_ANTIDEBUG=1
    char *env = getenv("BUNKER_NO_ANTIDEBUG");
    if (env && strcmp(env, "1") == 0) return 0;

    if (IsDebuggerPresent()) return 1;

    // simple check additionnel: NtQueryInformationProcess could be used but
    // nous restons avec un check simple ici (en TP).
    return 0;
#else
    // sur Unix on pourrait utiliser ptrace, etc. mais on laisse 0 pour tests cross-plat.
    return 0;
#endif
}

// ---------- Contrôle de flux (exemple simple de flattening) ----------
static void display_secrets_flattened(void) {
    // on déchiffre et affiche via une machine à états simple
    enum { ST_INIT = 0, ST_PRINT_HDR = 1, ST_PRINT_ITEM = 2, ST_DONE = 3 } state = ST_INIT;
    int idx = 0;
    while (1) {
        switch (state) {
            case ST_INIT:
                junk_void(idx);
                state = ST_PRINT_HDR;
                break;
            case ST_PRINT_HDR: {
                /* construire une petite chaîne xordée '['\0 et la déchiffrer
                   l'expression "\"[\" ^ XOR_KEY" est invalide car "[" est
                   un littéral de chaîne (const char *) — on doit utiliser un
                   tableau d'octets chiffrés. */
                unsigned char enc_bracket[] = { (unsigned char)('[') ^ XOR_KEY, 0 ^ XOR_KEY };
                char *hdr = decrypt_to_newbuf(enc_bracket); // idiot, juste pour ajouter noise
                /* libération immédiate pour éviter fuite (ce code montre le pattern) */
                free(hdr);
                puts("[BunkerVault] Secrets listing:");
                state = ST_PRINT_ITEM;
                break;
            }
            case ST_PRINT_ITEM:
                if (enc_listed[idx] == NULL) {
                    state = ST_DONE;
                    break;
                }
                // déchiffrer chaque item dans un buffer dynamique (évite d'avoir la chaîne en clair dans .data)
                char *clear = decrypt_to_newbuf(enc_listed[idx]);
                if (clear) {
                    printf("  %s\n", clear);
                    // libère tout de suite
                    // (l'attaquant pourra toujours dumper la mémoire du processus pendant l'exécution)
                    free(clear);
                }
                idx++;
                // introduire un peu de junk entre les prints
                junk_void(idx * 13);
                break;
            case ST_DONE:
            default:
                return;
        }
    }
}

// ---------- Vérification du mot de passe ----------
static int check_password_runtime(const char *input) {
    // déchiffre la clé en mémoire stack (petite preuve de principe)
    char *clear_pass = decrypt_to_newbuf(enc_master_pass);
    if (!clear_pass) return 0;
    int ok = (strcmp(input, clear_pass) == 0);
    // efface le buffer avant free
    memset(clear_pass, 0, strlen(clear_pass));
    free(clear_pass);
    // ajouter du bruit
    junk_void(ok);
    return ok;
}

// ---------- main ----------
int main(void) {
    // anti-debug
    if (detect_debugger()) {
        puts("Debugger detected! Exiting.");
        return 1;
    }

    char entered[64];
    printf("=== BunkerVault access control ===\n");
    printf("Enter master password: ");
    if (!fgets(entered, sizeof entered, stdin)) {
        return 1;
    }
    entered[strcspn(entered, "\r\n")] = '\0';

    if (!check_password_runtime(entered)) {
        puts("Access denied.");
        return 1;
    }

    puts("Access granted.\n");
    display_secrets_flattened();
    // Final flag output (déchiffré à la volée)
    char *clear_flag = decrypt_to_newbuf(enc_flag);
    if (clear_flag) {
        puts(clear_flag);
        memset(clear_flag, 0, strlen(clear_flag));
        free(clear_flag);
    }
    return 0;
}
