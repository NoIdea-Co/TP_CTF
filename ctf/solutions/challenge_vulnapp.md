# Challenge Vuln App – Format String

## Scénario
Service TCP vulnérable effectue `printf(user_input)` directement.

## Étapes
- Se connecter: `nc <host> 6009`
- Envoyer des payloads `%p` pour lire des pointeurs
- La chaîne `FLAG` est en global; avec `%s` + adresse, lire `CTF{pwn_easy_format_string}`
- Indice: En environnement local, brute-force l'emplacement ou utiliser gdb.

## Flag
`CTF{pwn_easy_format_string}`
