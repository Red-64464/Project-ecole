## 32. Explication des fichiers importants

| Fichier | Rôle |
|---|---|
| `features.py` | calcule les 7 features |
| `rlMinimaxAgents.py` | contient l'agent final `RLMinimaxAgent` |
| `train.py` | entraîne les poids |
| `compare.py` | compare RLMinimax avec l'agent du Lab 1 |
| `multiAgents.py` | contient Minimax et AlphaBeta du Lab 1 |
| `weights.json` | stocke les poids appris |
| `pacman.py` | lance les parties |
| `game.py` | moteur du jeu |
| `ghostAgents.py` | comportement des fantômes |

## 33. Explication des fonctions corrigées

### `compare.py`

Problème avant :

```text
La comparaison allait chercher un fichier dans le dossier des laboratoires.
Ce n'etait pas bon pour un projet Git isole.
```

Correction :

```text
compare.py utilise seulement AlphaBetaAgent depuis le multiAgents.py du projet.
```

Pourquoi :

```text
Le projet peut etre pousse seul, sans dependre d'un dossier externe.
```

### Isolation du projet

Problème avant :

```text
Certains morceaux du squelette pouvaient regarder hors du dossier Projet.
```

Correction :

```text
pacman.py charge les agents seulement depuis le dossier Projet.
layout.py charge les cartes seulement depuis Projet/layouts.
compare.py compare avec le AlphaBetaAgent local du projet.
```

Pourquoi :

```text
Le projet peut etre mis sur Git tout seul.
Il ne depend plus du dossier des laboratoires ni du correctif de quelqu'un d'autre.
```

### `train.py --resume`

Problème avant :

```text
Chaque entraînement repartait de poids aléatoires.
```

Correction :

```text
Avec --resume, train.py reprend les poids de weights.json.
```

Pourquoi :

```text
C'est utile pour continuer un entraînement long.
```

### `train.py --epsilon`

Problème avant :

```text
Pendant l'entraînement, l'agent suivait toujours sa stratégie actuelle.
Il explorait peu de nouvelles situations.
```

Correction :

```text
Avec --epsilon, il teste parfois une action aléatoire pendant l'entraînement.
```

Pourquoi :

```text
C'est cohérent avec le Lab 2 et aide l'apprentissage.
```

### Sécurité contre les poids extrêmes

Problème avant :

```text
Le code refusait NaN et inf, mais pas un nombre fini enorme comme 1e308.
```

Correction :

```text
Les poids et le biais trop grands sont maintenant refuses.
```

Pourquoi :

```text
Cela evite les calculs infinis et les poids qui explosent.
```

### Anti-Stop dans `RLMinimaxAgent`

Problème avant :

```text
Sur les grandes cartes, Pac-Man pouvait choisir Stop trop souvent.
```

Correction :

```text
Quand Pac-Man peut bouger, l'agent retire Stop de ses actions possibles.
```

Pourquoi :

```text
Cela evite de perdre du temps inutilement et rend le comportement plus propre.
```
