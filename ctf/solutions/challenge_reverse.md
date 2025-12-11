# Challenge Reverse – Basic Check

## Scénario
Télécharger le binaire `rev_check`, trouver la chaîne attendue pour afficher le flag.

## Étapes
- Inspecter les chaînes: `strings rev_check | Select-String reverserocks`
- Ou désassembler; comparaison `strcmp(input, "reverserocks")`
- Exécuter: `echo reverserocks | ./rev_check` → `CTF{reverse_basic_check}`

## Flag
`CTF{reverse_basic_check}`
