# Challenge Virology – Config Analysis

## Scénario
Analyser des faux logs et une config chiffrée pour extraire une URL C2 (flag).

## Étapes
- Lire `beacon.log` (facultatif, indicatif)
- Lire `pseudo.txt`: base64 puis XOR clé 0x42
- Charger `config.bin`, décoder base64 → octets
- XOR avec 0x42 → chaîne texte (C2) menant au flag
- Le flag: `CTF{malware_config_decoded}`

## Flag
`CTF{malware_config_decoded}`
