# Labo 1 - Minimax et Alpha-Beta pour Pac-Man 🧠🎮

Ce fichier explique uniquement les notions du labo 1 qui servent dans notre projet.

Objectif :

```text
Comprendre comment Pac-Man réfléchit avec Minimax et Alpha-Beta.
```

Dans notre projet, ces notions servent surtout dans :

```text
multiAgents.py
rlMinimaxAgents.py
compare.py
```

---

## 1. Le problème très simple 🙂

Pac-Man doit choisir une action :

```text
North
South
East
West
Stop
```

Mais il y a un problème :

```text
Pac-Man ne joue pas seul.
Les fantômes jouent aussi.
```

Donc Pac-Man doit réfléchir comme ça :

```text
Si je vais à droite...
    que peut faire le fantôme ?
Si je vais en haut...
    que peut faire le fantôme ?
Si je vais à gauche...
    est-ce que je me rapproche de la nourriture ?
    est-ce que je me rapproche d'un danger ?
```

---

## 2. C'est quoi un agent ? 🤖

Un agent est simplement un programme qui choisit une action.

Dans Pac-Man :

```text
Pac-Man = un agent
Fantôme 1 = un agent
Fantôme 2 = un agent
...
```

Dans Berkeley, tous les agents héritent de :

```python
class Agent
```

Dans notre projet :

```text
RLMinimaxAgent hérite de Agent.
```

Phrase simple :

```text
Un agent est un joueur automatique.
```

---

## 3. C'est quoi un état du jeu ? 📍

Un état du jeu, c'est une photo complète de la partie à un instant donné.

Il contient par exemple :

```text
position de Pac-Man
position des fantômes
nourriture restante
capsules restantes
score actuel
est-ce qu'on a gagné ?
est-ce qu'on a perdu ?
```

Dans Berkeley, cette situation s'appelle :

```python
GameState
```

Phrase simple :

```text
GameState = photo complète du jeu.
```

---

## 4. Les méthodes Berkeley utilisées 🔧

Tu n'as pas besoin de comprendre tout Berkeley.
Mais tu dois comprendre ces méthodes :

| Méthode | Explication simple |
|---|---|
| `getLegalActions(agentIndex)` | Donne les actions possibles |
| `generateSuccessor(agentIndex, action)` | Imagine le prochain état après une action |
| `getNumAgents()` | Donne le nombre total d'agents |
| `isWin()` | Dit si Pac-Man a gagné |
| `isLose()` | Dit si Pac-Man a perdu |
| `getScore()` | Donne le score actuel |

Exemple :

```text
getLegalActions(0)
= les actions possibles pour Pac-Man

getLegalActions(1)
= les actions possibles pour le fantôme 1
```

---
