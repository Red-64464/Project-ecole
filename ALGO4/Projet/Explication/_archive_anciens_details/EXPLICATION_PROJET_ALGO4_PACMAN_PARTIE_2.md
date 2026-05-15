## 6. C’est quoi une fonction d’évaluation ?

Une fonction d'évaluation donne une note à une situation.

Exemple simple :

```text
situation dangereuse -> note basse
situation avec nourriture proche -> note haute
situation gagnée -> note très haute
situation perdue -> note très basse
```

Dans ce projet, la fonction d'évaluation est affine :

```text
score = biais + somme(poids * feature)
```

Dans le code :

```python
score = bias
for w, x in zip(weights, features):
    score += w * x
```

## 7. C’est quoi Minimax ?

Minimax est un algorithme pour les jeux avec adversaires.

Idée simple :

- Pac-Man veut le meilleur score possible.
- Les fantômes veulent le pire score possible pour Pac-Man.

Donc Pac-Man choisit l'action dont le pire futur reste le meilleur.

Schéma :

```text
Pac-Man choisit
├── Action A
│   ├── Ghost fait X -> score 5
│   └── Ghost fait Y -> score 2
│   pire cas = 2
└── Action B
    ├── Ghost fait X -> score 4
    └── Ghost fait Y -> score 3
    pire cas = 3

Pac-Man choisit B, car 3 est meilleur que 2.
```

Pac-Man est MAX.  
Les fantômes sont MIN.

## 8. C’est quoi Alpha-Beta pruning ?

Alpha-bêta est une amélioration de Minimax.

Le résultat est le même que Minimax, mais on évite de calculer certaines branches inutiles.

Idée Feynman :

```text
Tu cherches le meilleur restaurant.
Tu as déjà trouvé un restaurant noté 9/10.
Un ami te dit qu'un autre restaurant ne peut pas dépasser 6/10.
Tu n'as plus besoin de lire tout son menu.
Tu peux passer au suivant.
```

En Minimax :

- `alpha` = meilleur score déjà garanti pour Pac-Man.
- `beta` = meilleur score que les fantômes peuvent imposer.

Si `alpha >= beta`, on coupe la branche.

```text
          MAX
        /     \
      MIN     MIN
     / | \    / | \
    3  5  8  ?  ?  ?

Si on sait déjà que cette branche ne peut pas battre le meilleur choix,
on arrête de l'explorer.
```

## 9. C’est quoi la profondeur ?

La profondeur dit combien de futur l'agent regarde.

Profondeur faible :

- rapide
- mais Pac-Man voit moins loin

Profondeur forte :

- plus intelligent
- mais beaucoup plus lent

Dans ce projet, `depth=2` ou `depth=3` est souvent raisonnable.  
`depth=4` peut devenir lent selon la carte.

## 10. C’est quoi un état terminal ?

Un état terminal est une situation où la partie est finie.

Deux cas :

```python
game_state.isWin()
game_state.isLose()
```

Si Pac-Man a gagné, on donne une très bonne valeur.  
S'il a perdu, on donne une très mauvaise valeur.

Dans `rl_evaluation_function` :

```python
if game_state.isWin():
    return 100000.0 + game_state.getScore()
if game_state.isLose():
    return -100000.0 + game_state.getScore()
```
