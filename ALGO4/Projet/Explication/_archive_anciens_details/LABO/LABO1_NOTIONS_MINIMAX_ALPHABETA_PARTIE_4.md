## 13. Problème de Minimax 🐢

Minimax est logique, mais il peut être lent.

Pourquoi ?

```text
Il explore presque toutes les possibilités.
```

Si Pac-Man a 4 actions et chaque fantôme a 4 actions :

```text
4 x 4 x 4 x 4 ...
```

Ça grossit très vite.

Solution :

```text
Alpha-Beta pruning.
```

---

## 14. Alpha-Beta : même résultat, plus rapide ⚡

Alpha-Beta ne change pas la décision finale.

Il fait juste ça :

```text
Si une branche ne peut plus devenir intéressante,
on arrête de la calculer.
```

Image simple :

```text
Minimax normal :
je lis tout l'arbre.

Alpha-Beta :
je coupe les branches inutiles.
```

---

## 15. Alpha et Beta expliqués simplement 🔢

### Alpha 🟢

Alpha est la meilleure valeur déjà trouvée pour Pac-Man.

```text
alpha = ce que Pac-Man sait déjà pouvoir obtenir
```

### Beta 🔴

Beta est la meilleure limite déjà trouvée pour les fantômes.

```text
beta = ce que les fantômes peuvent déjà imposer
```

Règle de coupe :

```text
si alpha >= beta:
    on coupe
```

Pourquoi ?

```text
Parce qu'on sait déjà que cette branche ne changera pas la décision finale.
```

---

## 16. Pseudo-code Alpha-Beta avec explication ligne par ligne 🧾⚡

Pseudo-code :

```text
fonction alphabeta(state, depth, agent, alpha, beta):
    si state est terminal:
        retourner evaluation(state)

    si agent est Pac-Man:
        valeur = -infini
        pour chaque action:
            prochain = generateSuccessor(state, action)
            valeur = max(valeur, alphabeta(prochain, depth, prochain_agent, alpha, beta))
            alpha = max(alpha, valeur)
            si alpha >= beta:
                couper la branche
        retourner valeur

    sinon agent est un fantôme:
        valeur = +infini
        pour chaque action:
            prochain = generateSuccessor(state, action)
            valeur = min(valeur, alphabeta(prochain, depth, prochain_agent, alpha, beta))
            beta = min(beta, valeur)
            si alpha >= beta:
                couper la branche
        retourner valeur
```

Explication :

| Ligne | Sens simple |
|---|---|
| `alphabeta(..., alpha, beta)` | Minimax avec deux limites |
| `alpha` | Meilleur score déjà connu pour Pac-Man |
| `beta` | Meilleure limite déjà connue côté fantômes |
| `valeur = -infini` | Pac-Man cherche à améliorer |
| `alpha = max(alpha, valeur)` | Pac-Man met à jour son meilleur score garanti |
| `si alpha >= beta` | La branche est inutile |
| `couper la branche` | On arrête de calculer cette partie |
| `valeur = +infini` | Le fantôme cherche à diminuer |
| `beta = min(beta, valeur)` | Le fantôme met à jour sa meilleure limite |

---
