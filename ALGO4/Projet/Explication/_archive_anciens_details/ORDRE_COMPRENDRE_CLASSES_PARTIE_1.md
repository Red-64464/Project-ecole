# ORDRE POUR COMPRENDRE LES CLASSES DU PROJET PAC-MAN IA 🙂

Ce fichier sert à réviser vite.

Le but n'est pas de lire tous les fichiers Berkeley ligne par ligne.
Le but est de comprendre :

```text
1. quelles classes sont juste le moteur du jeu ;
2. quelles classes viennent des labos ;
3. quelles classes sont vraiment importantes pour le projet ;
4. dans quel ordre les lire pour ne pas se perdre.
```

Phrase simple :

```text
Le moteur Berkeley fait tourner Pac-Man.
Nous, on a surtout ajouté/commenté les agents intelligents,
les features, les poids, l'entraînement et la comparaison.
```

---

## 0. Les fichiers à NE PAS lire en premier ⚠️

Ces fichiers sont fournis par Berkeley.
Ils sont importants pour que le jeu tourne, mais il ne faut pas commencer par eux.

| Fichier | Classes principales | Ce qu'il faut comprendre | Priorité |
|---|---|---|---|
| `graphicsDisplay.py` | `PacmanGraphics`, `InfoPane` | Affichage graphique | Très faible |
| `graphicsUtils.py` | `GraphicsUtil` | Outils graphiques | Très faible |
| `keyboardAgents.py` | `KeyboardAgent` | Jouer au clavier | Très faible |
| `textDisplay.py` | `NullGraphics`, `PacmanGraphics` | Affichage texte ou silencieux | Faible |
| `util.py` | `Queue`, `Stack`, `Counter` | Outils utiles, surtout `Queue` pour BFS | Moyenne |
| `game.py` | `Agent`, `Directions`, `Grid`, `Game` | Base du moteur Pac-Man | Moyenne |
| `pacman.py` | `GameState`, `PacmanRules`, `GhostRules` | Règles du jeu | Moyenne |
| `layout.py` | `Layout` | Charge les cartes `.lay` | Faible |
| `ghostAgents.py` | `RandomGhost`, `DirectionalGhost` | Comportement des fantômes | Moyenne |

À l'oral, tu peux dire :

```text
Ces classes forment le moteur Berkeley.
Je les utilise, mais mon travail principal est dans les agents, les features,
l'entraînement et la comparaison.
```

---

## 1. Ordre de lecture recommandé 🧭

Voici le meilleur ordre pour quelqu'un qui ne connaît pas le projet.

| Ordre | Fichier / classe | Difficulté | Pourquoi commencer ici ? |
|---:|---|---|---|
| 1 | `game.py` → `Agent` | Très facile | Tous les agents héritent de cette classe |
| 2 | `pacman.py` → `GameState` | Facile | C'est l'état du jeu que les agents regardent |
| 3 | `ghostAgents.py` → `RandomGhost`, `DirectionalGhost` | Facile | Pour comprendre les adversaires |
| 4 | `features.py` → fonctions de features | Facile à moyen | Les features sont les yeux de Pac-Man |
| 5 | `multiAgents.py` → `ReflexAgent` | Facile | Premier agent simple : il regarde un coup |
| 6 | `multiAgents.py` → `MinimaxAgent` | Moyen | Base de la recherche adversariale |
| 7 | `multiAgents.py` → `AlphaBetaAgent` | Moyen+ | Même idée que Minimax, mais plus rapide |
| 8 | `rlMinimaxAgents.py` → poids + évaluation | Moyen | On voit la formule affine du PDF |
| 9 | `rlMinimaxAgents.py` → `RLMinimaxAgent` | Important | C'est l'agent final du projet |
| 10 | `train.py` → `TrainingRLMinimaxAgent` | Complexe | Sert à apprendre les poids |
| 11 | `train.py` → `train()` | Complexe | Boucle d'entraînement par renforcement |
| 12 | `compare.py` | Facile | Compare RLMinimax avec AlphaBeta |

Résumé ultra court :

```text
Agent → GameState → features → Minimax → AlphaBeta → RLMinimax → train → compare
```

---

## 2. Les classes Berkeley minimum à comprendre 📦

Tu ne dois pas connaître tout Berkeley par coeur.
Mais tu dois comprendre ces classes-là.

### 2.1 `Agent` dans `game.py`

```python
class Agent:
    def get_action(self, state):
        ...
```

Idée simple :

```text
Un agent est un joueur automatique.
Il reçoit un état du jeu.
Il retourne une action.
```

Exemple :

```text
État reçu : Pac-Man est à gauche, un fantôme est proche.
Action retournée : East.
```

Pourquoi c'est important :

```text
ReflexAgent, MinimaxAgent, AlphaBetaAgent et RLMinimaxAgent
sont tous des agents.
```

---

### 2.2 `GameState` dans `pacman.py`

`GameState` représente une photo complète du jeu.

Méthodes importantes :

```python
game_state.getLegalActions(agentIndex)
game_state.generateSuccessor(agentIndex, action)
game_state.getPacmanPosition()
game_state.getGhostStates()
game_state.getFood()
game_state.getCapsules()
game_state.getScore()
game_state.isWin()
game_state.isLose()
```

Idée simple :

```text
GameState = tout ce que Pac-Man sait sur la situation actuelle.
```

Pourquoi c'est important :

```text
Minimax crée beaucoup de futurs possibles avec generateSuccessor.
Les features lisent GameState pour produire des nombres.
```

---

### 2.3 `Directions` dans `game.py`

Cette classe contient les actions possibles :

```text
North, South, East, West, Stop
```

Idée simple :

```text
C'est juste le vocabulaire des mouvements.
```

Dans ton projet, `Stop` est évité quand Pac-Man peut bouger.

---

### 2.4 `Queue` dans `util.py`

`Queue` est utilisée pour le BFS dans `features.py`.

Idée simple :

```text
Une Queue fonctionne comme une file d'attente.
Le premier arrivé est le premier sorti.
```

Pourquoi c'est important :

```text
Le BFS cherche la vraie distance dans le labyrinthe,
en tenant compte des murs.
```

---
