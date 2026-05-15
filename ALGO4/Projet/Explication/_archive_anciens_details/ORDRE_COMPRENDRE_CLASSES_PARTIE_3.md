
Mots à connaître :

| Mot | Sens simple |
|---|---|
| `alpha` | vitesse d'apprentissage |
| `epsilon` | probabilité de tester une action aléatoire |
| `gamma` | importance du futur |
| `reward` | récompense reçue |
| `td_error` | erreur entre prédiction et réalité |

---

### 3.8 `compare.py`

Ce fichier n'a pas de classe, mais il est important pour prouver que le projet marche.

Idée :

```text
On fait jouer AlphaBetaAgent.
On fait jouer RLMinimaxAgent.
On compare les scores, les victoires, le temps et Stop.
```

Fonctions importantes :

| Fonction | Rôle |
|---|---|
| `run_games` | Lance plusieurs parties |
| `summarize` | Calcule les moyennes |
| `print_stats` | Affiche les résultats |
| `main` | Lit les arguments de commande |

Phrase orale :

```text
compare.py sert à vérifier objectivement que RLMinimax est meilleur
que l'AlphaBeta local du labo 1.
```

---

## 4. Les classes optionnelles à connaître vite ⚪

### `ExpectimaxAgent`

Pas central pour le PDF.

Idée :

```text
Minimax suppose que les fantômes jouent parfaitement.
Expectimax suppose que les fantômes jouent plutôt au hasard.
```

Tu peux le connaître, mais ne passe pas trop de temps dessus.

---

### `betterEvaluationFunction`

Pas central pour le PDF.

Idée :

```text
C'est une fonction d'évaluation manuelle pour les agents du labo.
Elle n'est pas la fonction apprise principale du projet.
```

Important :

```text
La vraie fonction d'évaluation du projet est dans rlMinimaxAgents.py.
```

---

## 5. Carte des dépendances 🗺️

Voici comment les fichiers dépendent les uns des autres :

```text
game.py
  └── Agent, Directions, Grid

pacman.py
  └── GameState, règles du jeu

features.py
  ├── lit GameState
  ├── utilise Queue de util.py
  └── produit [x1, x2, ..., x7]

multiAgents.py
  ├── MinimaxAgent
  └── AlphaBetaAgent

rlMinimaxAgents.py
  ├── utilise features.py
  ├── charge weights.json
  └── définit RLMinimaxAgent

train.py
  ├── utilise TrainingRLMinimaxAgent
  ├── joue beaucoup de parties
  └── sauvegarde weights.json

compare.py
  ├── lance AlphaBetaAgent
  ├── lance RLMinimaxAgent
  └── compare les statistiques
```

Version ultra simple :

```text
GameState → Features → Fonction affine → RLMinimax → Training → Compare
```

---
