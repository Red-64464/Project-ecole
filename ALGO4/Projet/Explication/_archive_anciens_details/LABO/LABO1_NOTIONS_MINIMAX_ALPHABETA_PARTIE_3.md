## 9. La fonction d'évaluation ⭐

Quand Pac-Man ne peut pas regarder plus loin, il doit donner une note à l'état.

Exemple :

```text
score élevé = bonne situation
fantôme proche = mauvaise situation
nourriture proche = bonne situation
```

Dans le labo 1, on peut utiliser :

```python
score_evaluation_function(state)
```

Elle retourne simplement :

```python
state.getScore()
```

Dans notre projet, on fait mieux :

```text
on utilise une fonction affine apprise par reinforcement learning.
```

---

## 10. Minimax expliqué simplement 🧠

Idée :

```text
Pac-Man choisit l'action qui reste bonne même si les fantômes jouent contre lui.
```

Donc :

```text
Pac-Man teste ses actions.
Pour chaque action, il imagine les réponses des fantômes.
Les fantômes choisissent les pires réponses.
Pac-Man garde l'action avec le meilleur résultat final.
```

---

## 11. Pseudo-code Minimax avec explication ligne par ligne 🧾

Pseudo-code :

```text
fonction minimax(state, depth, agent):
    si state est terminal:
        retourner evaluation(state)

    si agent est Pac-Man:
        valeur = -infini
        pour chaque action possible:
            prochain = generateSuccessor(state, action)
            valeur = max(valeur, minimax(prochain, depth, prochain_agent))
        retourner valeur

    sinon agent est un fantôme:
        valeur = +infini
        pour chaque action possible:
            prochain = generateSuccessor(state, action)
            valeur = min(valeur, minimax(prochain, depth, prochain_agent))
        retourner valeur
```

Explication :

| Ligne | Sens simple |
|---|---|
| `fonction minimax(...)` | On crée une fonction qui évalue un futur possible |
| `si state est terminal` | Si la partie est finie ou si on a assez regardé |
| `retourner evaluation(state)` | On donne une note à cette situation |
| `si agent est Pac-Man` | C'est le tour de Pac-Man |
| `valeur = -infini` | On commence avec une valeur très mauvaise |
| `pour chaque action possible` | On teste tous les coups possibles |
| `prochain = generateSuccessor(...)` | On simule le coup |
| `max(...)` | Pac-Man garde le meilleur score |
| `sinon agent est un fantôme` | C'est le tour d'un fantôme |
| `valeur = +infini` | Le fantôme cherche à faire baisser le score |
| `min(...)` | Le fantôme garde le pire score pour Pac-Man |

---

## 12. Exemple visuel Minimax 🌳

Ici Pac-Man choisit entre A et B.
Les fantômes choisissent ensuite le pire cas.

```text
             Pac-Man MAX
             /         \
          A /           \ B
           /             \
      Fantôme MIN     Fantôme MIN
       /      \         /      \
      5        2       8        4
```

Pour A :

```text
le fantôme choisit min(5, 2) = 2
```

Pour B :

```text
le fantôme choisit min(8, 4) = 4
```

Pac-Man choisit :

```text
max(2, 4) = 4
```

Donc Pac-Man choisit B ✅

---
