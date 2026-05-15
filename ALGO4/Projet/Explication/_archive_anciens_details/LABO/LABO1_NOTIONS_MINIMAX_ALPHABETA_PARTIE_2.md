## 5. MAX et MIN 🟢🔴

Dans Minimax, on voit le jeu comme un duel.

### Pac-Man = MAX 🟢

Pac-Man veut le score le plus grand possible.

```text
MAX = je veux maximiser le score
```

### Fantômes = MIN 🔴

Les fantômes veulent rendre la situation mauvaise pour Pac-Man.

```text
MIN = je veux minimiser le score de Pac-Man
```

Résumé :

```text
Pac-Man choisit le maximum.
Les fantômes choisissent le minimum.
```

---

## 6. L'arbre de recherche 🌳

Pac-Man imagine plusieurs futurs possibles.

Exemple :

```text
Etat actuel
|
+-- Pac-Man va North
|   |
|   +-- Fantôme va Left
|   +-- Fantôme va Right
|
+-- Pac-Man va East
|   |
|   +-- Fantôme va Left
|   +-- Fantôme va Right
|
+-- Pac-Man va West
    |
    +-- Fantôme va Left
    +-- Fantôme va Right
```

Chaque branche représente :

```text
une action possible
```

Chaque noeud représente :

```text
un état possible du jeu
```

---

## 7. C'est quoi la profondeur ? 📏

La profondeur dit combien de tours on regarde dans le futur.

Exemple :

```text
depth = 1
Pac-Man joue
Fantôme(s) jouent
Puis on évalue
```

```text
depth = 2
Pac-Man joue
Fantôme(s) jouent
Pac-Man joue
Fantôme(s) jouent
Puis on évalue
```

Plus la profondeur est grande :

```text
✅ Pac-Man voit plus loin
❌ le calcul devient beaucoup plus lent
```

Phrase simple :

```text
La profondeur, c'est combien de coups Pac-Man imagine avant de décider.
```

---

## 8. C'est quoi un état terminal ? 🏁

Un état terminal est une situation où on doit arrêter la recherche.

On arrête si :

```text
Pac-Man a gagné
Pac-Man a perdu
la profondeur est arrivée à 0
```

Dans le code :

```python
if depth == 0 or state.isWin() or state.isLose():
    return evaluationFunction(state)
```

Explication ligne par ligne :

```text
if depth == 0
    on a assez regardé dans le futur

or state.isWin()
    Pac-Man a gagné, inutile de continuer

or state.isLose()
    Pac-Man a perdu, inutile de continuer

return evaluationFunction(state)
    on donne une note à cette situation
```

---
