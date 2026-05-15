# Ordre de comprehension du projet - partie 2

| `ReflexAgent` | TD Berkeley + complÃ©tÃ© | Un peu | Agent simple qui regarde le prochain coup |
| `MultiAgentSearchAgent` | Berkeley | Oui, rÃ´le gÃ©nÃ©ral | Classe parent avec `depth` et `evaluationFunction` |
| `MinimaxAgent` | TD + complÃ©tÃ© | Oui | Base de la recherche adversariale |
| `AlphaBetaAgent` | TD + complÃ©tÃ© | Oui | RÃ©fÃ©rence utilisÃ©e pour comparer notre agent |
| `ExpectimaxAgent` | TD + complÃ©tÃ© | Moins prioritaire | Variante avec hasard |
| `betterEvaluationFunction` | TD + complÃ©tÃ© | Moins prioritaire | Fonction d'Ã©valuation classique |

Le plus important ici :

```text
MinimaxAgent et AlphaBetaAgent.
```

Pourquoi ?

```text
Notre RLMinimaxAgent reprend l'idÃ©e d'AlphaBeta,
mais il utilise une Ã©valuation apprise avec des poids.
```

---

## 3. Les classes Berkeley Ã  ne pas apprendre ligne par ligne ðŸ“¦

Ces classes sont le moteur du jeu.
Tu dois savoir Ã  quoi elles servent, mais pas les recoder.

| Fichier | Classe Berkeley | Ce qu'il faut comprendre |
|---|---|---|
| `game.py` | `Agent` | Tous les agents hÃ©ritent de cette classe |
| `game.py` | `Directions` | Contient `North`, `South`, `East`, `West`, `Stop` |
| `game.py` | `Actions` | Convertit les directions en mouvements |
| `game.py` | `Grid` | ReprÃ©sente les murs et la nourriture |
| `game.py` | `Game` | Boucle principale du jeu |
| `pacman.py` | `GameState` | ReprÃ©sente une situation complÃ¨te du jeu |
| `pacman.py` | `ClassicGameRules` | RÃ¨gles gÃ©nÃ©rales d'une partie |
| `pacman.py` | `PacmanRules` | RÃ¨gles de Pac-Man |
| `pacman.py` | `GhostRules` | RÃ¨gles des fantÃ´mes |
| `ghostAgents.py` | `RandomGhost` | FantÃ´me qui bouge alÃ©atoirement |
| `ghostAgents.py` | `DirectionalGhost` | FantÃ´me un peu plus intelligent |
| `layout.py` | `Layout` | Charge les cartes `.lay` |
| `util.py` | `Queue` | File utilisÃ©e pour le BFS |
| `util.py` | `Counter` | Dictionnaire pratique de Berkeley |

Phrase Ã  retenir :

```text
Berkeley donne le jeu.
Nous donnons le cerveau de Pac-Man.
```

---

## 4. Les mÃ©thodes Berkeley vraiment utilisÃ©es ðŸ”§

Tu ne dois pas connaÃ®tre tout Berkeley.
Mais tu dois comprendre ces mÃ©thodes, car notre code les appelle.

| MÃ©thode Berkeley | UtilisÃ©e oÃ¹ ? | Explication simple |
|---|---|---|
| `gameState.getLegalActions(agentIndex)` | `rlMinimaxAgents.py`, `multiAgents.py` | Donne les actions possibles |
| `gameState.generateSuccessor(agentIndex, action)` | `rlMinimaxAgents.py`, `multiAgents.py`, `train.py` | Simule le prochain Ã©tat aprÃ¨s une action |
| `gameState.getNumAgents()` | `rlMinimaxAgents.py`, `multiAgents.py` | Donne Pac-Man + nombre de fantÃ´mes |
| `gameState.isWin()` | Plusieurs fichiers | Dit si Pac-Man a gagnÃ© |
| `gameState.isLose()` | Plusieurs fichiers | Dit si Pac-Man a perdu |
| `gameState.getScore()` | `features.py`, `compare.py`, `train.py` | Donne le score |
| `gameState.getPacmanPosition()` | `features.py` | Donne la position de Pac-Man |
| `gameState.getGhostStates()` | `features.py` | Donne les infos des fantÃ´mes |
| `gameState.getFood()` | `features.py` | Donne la grille de nourriture |
| `gameState.getCapsules()` | `features.py` | Donne les capsules restantes |
| `gameState.getWalls()` | `features.py` | Donne les murs pour le BFS |
| `ghostState.scaredTimer` | `features.py` | Dit si un fantÃ´me est mangeable |
| `ghostState.getPosition()` | `features.py` | Donne la position d'un fantÃ´me |

Ce niveau suffit largement :

```text
Je sais ce que la mÃ©thode me donne.
Je n'ai pas besoin de savoir comment Berkeley l'a codÃ©e en interne.
```

---

## 5. Ordre recommandÃ© pour Ã©tudier rapidement ðŸš€

Si tu as peu de temps, lis dans cet ordre :

```text
1. features.py
2. rlMinimaxAgents.py
3. train.py
4. compare.py
5. multiAgents.py : surtout MinimaxAgent et AlphaBetaAgent
6. game.py : seulement Agent, Directions, Actions
7. pacman.py : seulement GameState et les mÃ©thodes utilisÃ©es
8. util.py : seulement Queue
9. layout.py : seulement le rÃ´le gÃ©nÃ©ral
```

Pourquoi cet ordre ?

```text
On commence par ce que notre agent voit.
Puis on comprend comment il dÃ©cide.
Puis comment il apprend.
Puis comment on prouve qu'il est meilleur.
Puis seulement on regarde le moteur Berkeley utilisÃ© autour.
```

---

## 6. Ce qu'il faut dire si le prof parle de Berkeley ðŸŽ“

RÃ©ponse propre :

```text
Les classes Berkeley fournissent le moteur du jeu :
GameState, actions, rÃ¨gles, fantÃ´mes, score, layouts.

Je ne les ai pas rÃ©Ã©crites.
Je les utilise comme API pour construire mon agent.

Mon travail porte surtout sur :
les features,
la fonction affine,
RLMinimaxAgent,
l'entraÃ®nement des poids,
et la comparaison statistique avec AlphaBeta.
```

---

## 7. RÃ©sumÃ© enfant de 5 ans ðŸ‘¶

```text
Berkeley a construit le terrain de jeu Pac-Man.
Nous, on a construit le cerveau de Pac-Man.

Le cerveau regarde :
- la nourriture
- les fantÃ´mes
- les capsules
- le score

Puis il donne une note Ã  chaque situation.
Ensuite il choisit le meilleur chemin avec Minimax + AlphaBeta.
Les poids de la note ont Ã©tÃ© appris en jouant plusieurs parties.
```

---

## 8. Conclusion finale âœ…

Ã€ comprendre Ã  fond :

```text
features.py
rlMinimaxAgents.py
train.py
compare.py
