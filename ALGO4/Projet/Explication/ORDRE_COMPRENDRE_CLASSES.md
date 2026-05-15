# Ordre simple pour comprendre le projet

## 1. Reponse courte

Non, tu ne dois pas comprendre tout Berkeley ligne par ligne.

Tu dois comprendre :

1. Les fichiers que nous avons ajoutes.
2. Les fonctions Berkeley que notre code appelle.
3. Le chemin logique entre features, poids, Alpha-Beta et resultats.

## 2. Ordre de lecture recommande

| Ordre | Fichier | Pourquoi le lire |
|---:|---|---|
| 1 | `README.md` | Comprendre le but global |
| 2 | `RESULTATS_FINAUX_RL_MINIMAX.md` | Voir ce que le projet prouve |
| 3 | `weights.json` | Voir les poids appris |
| 4 | `features.py` | Comprendre les informations donnees a l'IA |
| 5 | `rlMinimaxAgents.py` | Comprendre l'agent final |
| 6 | `train.py` | Comprendre l'apprentissage |
| 7 | `compare.py` | Comprendre les tests |
| 8 | `multiAgents.py` | Comprendre Minimax et AlphaBeta classiques |

## 3. Minimum Berkeley a comprendre

| Fichier Berkeley | A comprendre |
|---|---|
| `game.py` | `Agent`, `Directions` |
| `pacman.py` | `GameState` |
| `layout.py` | Chargement des cartes |
| `util.py` | `Queue`, distances, outils |
| `ghostAgents.py` | Comportement simple des fantomes |
| `textDisplay.py` | Affichage sans interface graphique |

Tu peux lire ces fichiers en surface. Le but est de comprendre l'interface, pas chaque detail interne.

## 4. Methodes Berkeley les plus importantes

| Methode | Sens simple |
|---|---|
| `getLegalActions(agent_index)` | Donne les actions possibles |
| `generateSuccessor(agent_index, action)` | Simule un coup |
| `getPacmanPosition()` | Donne la position de Pac-Man |
| `getGhostStates()` | Donne les infos des fantomes |
| `getFood()` | Donne la nourriture restante |
| `getCapsules()` | Donne les capsules |
| `getScore()` | Donne le score |
| `isWin()` | Dit si Pac-Man a gagne |
| `isLose()` | Dit si Pac-Man a perdu |
| `getNumAgents()` | Donne Pac-Man + fantomes |

## 5. Ordre pour comprendre `features.py`

| Ordre | Element | Role |
|---:|---|---|
| 1 | `_neighbor_cells` | Trouver les cases voisines |
| 2 | `_bfs_distance` | Calculer une distance dans le labyrinthe |
| 3 | `_closest_target_score` | Transformer une distance en score |
| 4 | `nearest_food_bfs` | Trouver la nourriture proche |
| 5 | `ghosts_within_3` | Compter les fantomes dangereux proches |
| 6 | `scared_ghosts_nearby` | Reperer les fantomes mangeables |
| 7 | `remaining_food` | Evaluer la nourriture restante |
| 8 | `nearest_capsule_bfs` | Trouver les capsules |
| 9 | `extract_features` | Regrouper toutes les features |

## 6. Ordre pour comprendre `rlMinimaxAgents.py`

| Ordre | Element | Role |
|---:|---|---|
| 1 | Chargement des poids | Lire `weights.json` |
| 2 | `evaluation_function` | Donner une note a une position |
| 3 | `getAction` | Choisir l'action finale |
| 4 | Partie Alpha-Beta | Tester les futurs coups plus vite |
| 5 | Anti-Stop | Eviter de rester immobile sans raison |

## 7. Revision rapide en 30 minutes

| Temps | A faire |
|---:|---|
| 5 min | Lire le README et les resultats |
| 5 min | Lire `weights.json` et comprendre les poids |
| 8 min | Lire `features.py` |
| 7 min | Lire `rlMinimaxAgents.py` |
| 5 min | Lire `compare.py` et les commandes |

## 8. Phrase courte pour l'oral

```text
Notre agent Pac-Man utilise Alpha-Beta pour chercher les actions possibles, puis une fonction d'evaluation affine apprise par reinforcement learning pour choisir la meilleure action.
```
