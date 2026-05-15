## 17. Exemple visuel de coupe ✂️

```text
             MAX
           /     \
          A       B
        /   \   /   \
       8     9 5    ???
```

Si Pac-Man sait déjà que A vaut au moins 8 :

```text
alpha = 8
```

Dans B, un fantôme trouve déjà une option à 5 :

```text
beta = 5
```

Comme :

```text
alpha >= beta
8 >= 5
```

On coupe le reste de B ✂️

Pourquoi ?

```text
Pac-Man ne choisira jamais B,
car les fantômes peuvent déjà le faire tomber à 5.
```

---

## 18. Lien direct avec notre projet 🔗

Dans notre projet :

```text
RLMinimaxAgent utilise Minimax + Alpha-Beta.
```

Mais au lieu d'utiliser seulement :

```python
state.getScore()
```

il utilise :

```text
f(s) = a1*x1(s) + a2*x2(s) + ... + a7*x7(s) + C
```

Donc :

```text
Labo 1 donne la recherche intelligente.
Projet ajoute l'évaluation apprise.
```

---

## 19. Ce qu'il faut savoir pour l'oral 🎓

Tu dois savoir expliquer :

```text
1. Pac-Man est MAX.
2. Les fantômes sont MIN.
3. Minimax imagine les coups futurs.
4. La profondeur limite le calcul.
5. Les feuilles sont évaluées avec une fonction d'évaluation.
6. Alpha-Beta coupe les branches inutiles.
7. Alpha-Beta donne le même choix que Minimax, mais plus vite.
8. Notre agent final reprend Alpha-Beta avec une évaluation apprise.
```

Phrase parfaite :

```text
Le labo 1 m'a donné la partie recherche adversariale :
Pac-Man maximise, les fantômes minimisent, et Alpha-Beta accélère Minimax.
Dans le projet, j'utilise cette recherche avec une fonction d'évaluation apprise.
```

---

## 20. Résumé ultra simple 👶

```text
Pac-Man imagine le futur.
Il suppose que les fantômes vont l'embêter.
Il choisit le chemin qui reste le meilleur même dans le pire cas.
Alpha-Beta l'aide à réfléchir plus vite en coupant les chemins inutiles.
```
