## 3. Les classes / fichiers que NOUS avons vraiment travaillés 🛠️

### 3.1 `features.py` : pas une classe, mais très important

Ce fichier définit les facteurs `x1(s), x2(s), ..., x7(s)`.

Ce sont les informations simples que Pac-Man utilise pour juger un état.

| Feature | Rôle simple |
|---|---|
| `nearest_food_bfs` | Est-ce que la nourriture est proche ? |
| `ghosts_within_3` | Combien de fantômes dangereux sont très proches ? |
| `scared_ghosts_nearby` | Combien de fantômes mangeables sont proches ? |
| `remaining_food` | Combien de nourriture reste-t-il ? |
| `nearest_capsule_bfs` | Est-ce qu'une capsule est proche ? |
| `current_score` | Quel est le score actuel ? |
| `nearest_dangerous_ghost_bfs` | Le fantôme dangereux le plus proche est-il trop proche ? |

Ordre pour lire ce fichier :

```text
1. lire les commentaires du haut ;
2. lire _bfs_distance ;
3. lire _closest_target_score ;
4. lire les 7 features ;
5. lire extract_features.
```

Phrase orale :

```text
Les features transforment un état compliqué en une liste de nombres simples.
Ces nombres sont ensuite multipliés par les poids appris.
```

---

### 3.2 `ReflexAgent` dans `multiAgents.py`

Difficulté : facile 🙂

Idée :

```text
Il regarde seulement les états juste après une action.
Il ne prévoit pas vraiment les fantômes dans le futur.
```

Pourquoi le lire :

```text
C'est le premier niveau d'intelligence.
Il aide à comprendre ce qu'est une fonction d'évaluation.
```

Ce n'est pas l'agent final.

---

### 3.3 `MinimaxAgent` dans `multiAgents.py`

Difficulté : moyenne 🧠

Idée :

```text
Pac-Man veut maximiser le score.
Les fantômes veulent minimiser le score.
```

Image simple :

```text
Pac-Man dit : je choisis le meilleur futur.
Les fantômes disent : on choisit le pire futur pour Pac-Man.
```

Pourquoi le lire avant AlphaBeta :

```text
AlphaBeta est juste une optimisation de Minimax.
Si tu comprends Minimax, AlphaBeta devient beaucoup plus logique.
```

---

### 3.4 `AlphaBetaAgent` dans `multiAgents.py`

Difficulté : moyenne+ ✂️

Idée :

```text
C'est Minimax, mais on arrête de calculer les branches inutiles.
```

Alpha et beta :

```text
alpha = meilleur score garanti pour Pac-Man
beta  = meilleur score que les fantômes peuvent imposer
```

Pourquoi c'est important :

```text
Le PDF demande un agent Minimax avec élagage alpha-bêta.
RLMinimax reprend cette idée.
```

À retenir :

```text
AlphaBeta n'est pas plus intelligent que Minimax.
Il calcule juste plus efficacement.
```

---

### 3.5 `RLMinimaxAgent` dans `rlMinimaxAgents.py`

Difficulté : importante ⭐

C'est la classe principale du projet.

Idée :

```text
RLMinimaxAgent fait une recherche AlphaBeta.
Mais pour évaluer les feuilles de l'arbre,
il utilise une fonction affine avec des poids appris.
```

Formule du PDF :

```text
f(s) = a1*x1(s) + a2*x2(s) + ... + ak*xk(s) + C
```

Dans le code :

```text
features = extract_features(game_state)
score = bias + somme(weight_i * feature_i)
```

À lire dans cet ordre :

```text
1. DEFAULT_WEIGHTS, DEFAULT_BIAS, WEIGHTS_FILE
2. _coerce_finite_float
3. _normalize_weights
4. load_weights
5. save_weights
6. rl_evaluation_function
7. RLMinimaxAgent.get_action
```

Pourquoi cet ordre :

```text
D'abord on comprend les poids.
Ensuite on comprend la formule.
Enfin on comprend comment AlphaBeta utilise cette formule.
```

Phrase orale :

```text
RLMinimaxAgent est un AlphaBetaAgent dont la fonction d'évaluation
vient de poids appris par renforcement.
```

---

### 3.6 `TrainingRLMinimaxAgent` dans `train.py`

Difficulté : complexe 🏋️

Cette classe sert seulement pendant l'entraînement.

Différence avec `RLMinimaxAgent` :

| RLMinimaxAgent | TrainingRLMinimaxAgent |
|---|---|
| Joue avec les poids de `weights.json` | Reçoit les poids en paramètre |
| Sert à jouer normalement | Sert à apprendre |
| Ne garde pas d'historique | Enregistre les états visités |
| Pas d'exploration aléatoire | Peut utiliser `epsilon` |

Idée simple :

```text
Pendant l'entraînement, on doit se souvenir des états visités.
Sinon on ne sait pas comment corriger les poids.
```

Phrase orale :

```text
TrainingRLMinimaxAgent est une version spéciale de RLMinimaxAgent
qui garde l'historique pour apprendre.
```

---

### 3.7 La fonction `train()` dans `train.py`

Ce n'est pas une classe, mais c'est le coeur de l'apprentissage.

Idée :

```text
On joue beaucoup de parties.
On regarde les récompenses.
On ajuste les poids petit à petit.
On sauvegarde les meilleurs poids.
```

Ordre logique :

```text
1. charger la carte ;
2. choisir les poids de départ ;
3. jouer une partie ;
4. récupérer l'historique ;
5. calculer l'erreur TD ;
6. modifier les poids ;
7. sauvegarder les meilleurs.
```
