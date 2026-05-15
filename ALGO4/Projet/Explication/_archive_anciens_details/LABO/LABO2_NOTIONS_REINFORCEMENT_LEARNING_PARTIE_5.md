## 18. Pourquoi tester les mauvais poids ? 💣

Parce que `weights.json` peut être cassé.

Exemples dangereux :

```text
NaN
inf
1e308
"abc"
true
mauvais nombre de poids
JSON invalide
```

Notre code doit éviter :

```text
crash brutal
calcul infini
agent impossible à lancer
```

Donc on valide les poids dans :

```text
rlMinimaxAgents.py
```

Phrase simple :

```text
Même si le fichier de poids est mauvais, le projet doit rester robuste.
```

---

## 19. Lien direct avec RLMinimaxAgent 🔗

Notre agent final combine :

```text
Labo 1 : Alpha-Beta
Labo 2 : poids appris par reinforcement learning
```

Schéma :

```text
                 features.py
                     |
                     v
            fonction affine f(s)
                     |
                     v
train.py ---> apprend les poids ---> weights.json
                     |
                     v
          RLMinimaxAgent utilise ces poids
                     |
                     v
        Alpha-Beta choisit la meilleure action
```

---

## 20. Ce qu'il faut savoir pour l'oral 🎓

Tu dois savoir expliquer :

```text
1. Les features décrivent l'état du jeu.
2. Les poids disent l'importance de chaque feature.
3. La fonction d'évaluation est affine.
4. Les poids sont appris par entraînement.
5. L'entraînement utilise reward, alpha, gamma, epsilon.
6. Les meilleurs poids sont sauvegardés dans weights.json.
7. Les poids sont ensuite utilisés par RLMinimaxAgent.
8. On teste les poids sur plusieurs layouts.
```

Phrase parfaite :

```text
Le labo 2 m'a permis de comprendre comment ajuster automatiquement les poids
d'une fonction d'évaluation. Dans le projet, ces poids sont appris par entraînement,
sauvegardés dans weights.json, puis utilisés par RLMinimaxAgent.
```

---

## 21. Résumé ultra simple 👶

```text
Pac-Man regarde le jeu avec des features.
Chaque feature a un poids.
Les poids disent ce qui est important.
Pac-Man joue beaucoup de parties.
Après chaque partie, il corrige ses poids.
Quand les poids sont bons, on les garde dans weights.json.
Ensuite RLMinimaxAgent utilise ces poids pour mieux jouer.
```
