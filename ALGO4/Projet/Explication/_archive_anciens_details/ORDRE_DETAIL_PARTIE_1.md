# Ordre de comprehension du projet - partie 1

# Ordre simple pour comprendre le projet Pac-Man ALG4 ðŸ™‚

Ce fichier rÃ©pond Ã  une question trÃ¨s importante :

```text
Est-ce que je dois comprendre toutes les classes Berkeley ?
```

RÃ©ponse courte :

```text
Non.
Tu dois surtout comprendre les classes et fichiers qu'on a ajoutÃ©s ou modifiÃ©s.
Pour Berkeley, tu dois comprendre le rÃ´le gÃ©nÃ©ral et les mÃ©thodes qu'on utilise.
```

Berkeley fournit le moteur du jeu Pac-Man.
Nous, on a ajoutÃ© l'intelligence de Pac-Man.

---

## 1. Les fichiers Ã  comprendre Ã  fond ðŸ§ 

Ce sont les fichiers les plus importants pour l'examen.
Ce sont ceux qui expliquent vraiment notre travail.

| PrioritÃ© | Fichier | Qui l'a fait ? | Pourquoi le comprendre ? |
|---|---|---|---|
| 1 | `features.py` | Nous | Il calcule ce que Pac-Man regarde : nourriture, fantÃ´mes, capsules, score |
| 2 | `rlMinimaxAgents.py` | Nous | Il contient l'agent final `RLMinimaxAgent` |
| 3 | `train.py` | Nous | Il apprend les poids avec le reinforcement learning |
| 4 | `compare.py` | Nous | Il compare notre agent avec `AlphaBetaAgent` |
| 5 | `weights.json` | Nous | Il contient les poids appris |
| 6 | `multiAgents.py` | Berkeley + nous | Il contient les agents du TD : Minimax, AlphaBeta, Expectimax |

Phrase simple :

```text
Je dois comprendre notre intelligence, pas tout le moteur Berkeley.
```

---

## 2. Nos classes et fonctions importantes âœ…

### `features.py` ðŸ§©

Ce fichier ne contient pas de classe.
Il contient des fonctions.

Ã€ comprendre :

| Fonction | RÃ´le simple |
|---|---|
| `extract_features(game_state)` | Retourne les 7 informations importantes sur l'Ã©tat du jeu |
| `nearest_food_bfs(game_state)` | Regarde si la nourriture est proche |
| `ghosts_within_3(game_state)` | Compte les fantÃ´mes dangereux proches |
| `scared_ghosts_nearby(game_state)` | Compte les fantÃ´mes mangeables proches |
| `remaining_food(game_state)` | Regarde combien de nourriture reste |
| `nearest_capsule_bfs(game_state)` | Regarde si une capsule est proche |
| `current_score(game_state)` | RÃ©cupÃ¨re le score actuel |
| `nearest_dangerous_ghost_bfs(game_state)` | Regarde si un fantÃ´me dangereux est proche |

Pourquoi c'est important :

```text
Les features sont les yeux de Pac-Man.
Si les features sont mauvaises, Pac-Man juge mal la situation.
```

---

### `rlMinimaxAgents.py` ðŸ¤–

Ce fichier est le coeur du projet.

Ã€ comprendre :

| Ã‰lÃ©ment | RÃ´le simple |
|---|---|
| `RLMinimaxAgent` | Notre agent final |
| `getAction()` | Choisit l'action Ã  jouer |
| `_value()` | Explore l'arbre Minimax |
| `_max_value()` | Tour de Pac-Man : il cherche le meilleur score |
| `_min_value()` | Tour des fantÃ´mes : ils cherchent Ã  gÃªner Pac-Man |
| `rl_evaluation_function()` | Calcule la note d'un Ã©tat |
| `load_weights()` | Charge les poids depuis `weights.json` |
| `save_weights()` | Sauvegarde les poids |
| `_normalize_weights()` | VÃ©rifie que les poids sont valides |

Formule Ã  connaÃ®tre :

```text
f(s) = a1*x1(s) + a2*x2(s) + ... + a7*x7(s) + C
```

Explication trÃ¨s simple :

```text
x = ce que Pac-Man voit
a = importance de chaque chose
C = petit bonus/malus constant
f(s) = note finale de la situation
```

---

### `train.py` ðŸŽ“

Ce fichier sert Ã  entraÃ®ner les poids.

Ã€ comprendre :

| Ã‰lÃ©ment | RÃ´le simple |
|---|---|
| `TrainingRLMinimaxAgent` | Version entraÃ®nable de notre agent |
| `train()` | Lance beaucoup de parties pour amÃ©liorer les poids |
| `evaluate_weights()` | Teste si les poids sont bons |
| `alpha` | Vitesse d'apprentissage |
| `epsilon` | Exploration : parfois Pac-Man tente autre chose |
| `gamma` | Importance du futur |

Phrase simple :

```text
train.py fait jouer Pac-Man plusieurs fois pour amÃ©liorer les coefficients de la formule.
```

---

### `compare.py` ðŸ“Š

Ce fichier sert Ã  prouver que notre agent est meilleur.

Ã€ comprendre :

| Fonction | RÃ´le simple |
|---|---|
| `run_games()` | Lance plusieurs parties |
| `summarize()` | Calcule moyenne, victoire, temps, Stop |
| `print_stats()` | Affiche les rÃ©sultats |
| `main()` | Compare AlphaBeta et RLMinimax |

Phrase simple :

```text
compare.py ne joue pas mieux.
Il mesure objectivement qui joue mieux.
```

---

### `multiAgents.py` ðŸ§ 

Ce fichier vient du TD Berkeley, mais on a complÃ©tÃ© les agents.

Ã€ comprendre :

| Classe / fonction | Origine | Ã€ comprendre ? | Pourquoi |
|---|---|---|---|
