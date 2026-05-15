# Explication simple du projet Pac-Man IA

## 1. Idee generale

Le projet ajoute une intelligence a Pac-Man.

Pac-Man ne joue pas au hasard. Il regarde l'etat du jeu, il imagine plusieurs coups possibles, puis il choisit l'action qui semble la meilleure.

Phrase a retenir :

```text
On utilise Alpha-Beta pour chercher les bons coups, et une fonction d'evaluation apprise pour juger les positions.
```

## 2. Ce que le projet ajoute

| Fichier | Role simple |
|---|---|
| `features.py` | Transforme une situation du jeu en nombres faciles a comparer |
| `rlMinimaxAgents.py` | Contient l'agent final `RLMinimaxAgent` |
| `train.py` | Entraine les poids de la fonction d'evaluation |
| `compare.py` | Compare RLMinimax avec Minimax et AlphaBeta |
| `weights.json` | Stocke les poids appris |
| `RESULTATS_FINAUX_RL_MINIMAX.md` | Resume les resultats importants |

## 3. Comment Pac-Man choisit une action

1. Il recupere les actions possibles.
2. Il simule les coups avec `generateSuccessor`.
3. Il utilise Alpha-Beta pour eviter de tout tester inutilement.
4. Il evalue les situations avec les features et les poids.
5. Il choisit l'action avec le meilleur score.

En tres simple :

```text
Pac-Man teste plusieurs futurs possibles, puis il prend celui qui donne le meilleur score.
```

## 4. Fonction d'evaluation

La fonction d'evaluation sert a dire si une position est bonne ou mauvaise.

Elle utilise des features comme :

| Feature | Sens |
|---|---|
| nourriture proche | Pac-Man aime se rapprocher de la nourriture |
| fantome dangereux proche | Pac-Man doit eviter le danger |
| capsule proche | Pac-Man peut devenir plus fort |
| score actuel | Un bon score est positif |
| nourriture restante | Moins il en reste, mieux c'est |

Formule simple :

```text
score = feature_1 * poids_1 + feature_2 * poids_2 + ... + biais
```

Un poids positif veut dire : Pac-Man aime cette chose.

Un poids negatif veut dire : Pac-Man evite cette chose.

## 5. Minimax et Alpha-Beta

Minimax imagine que :

| Personnage | Objectif |
|---|---|
| Pac-Man | Maximiser le score |
| Fantomes | Minimiser le score de Pac-Man |

Alpha-Beta fait la meme chose que Minimax, mais plus vite.

Il coupe les branches inutiles :

```text
Si on sait deja qu'un choix est mauvais, on arrete de le calculer.
```

## 6. Reinforcement Learning

Le Reinforcement Learning sert a apprendre les poids.

Pac-Man joue plusieurs parties. Apres chaque partie, on ajuste les poids selon les resultats.

S'il gagne ou fait mieux, les bons choix sont renforces.

S'il perd ou fait pire, les mauvais choix sont diminues.

## 7. Pourquoi `weights.json` est important

`weights.json` contient la memoire apprise.

Sans ce fichier, l'agent ne sait pas encore bien juger les positions.

Avec ce fichier, l'agent utilise les poids deja entraines.

## 8. Commandes utiles

Lancer une partie :

```bash
python pacman.py -p RLMinimaxAgent -l smallClassic
```

Entrainer l'agent :

```bash
python train.py
```

Comparer les agents :

```bash
python compare.py
```

## 9. Ce qu'il faut savoir expliquer a l'oral

Tu dois surtout savoir dire :

1. Pac-Man utilise Alpha-Beta pour chercher une bonne action.
2. La fonction d'evaluation donne une note a chaque situation.
3. Les features decrivent la situation avec des nombres.
4. Les poids disent quelles features sont importantes.
5. Le Reinforcement Learning sert a apprendre ces poids.
6. `compare.py` prouve si l'agent marche mieux que les autres.

## 10. Resume ultra simple

```text
Pac-Man regarde le plateau.
Il imagine plusieurs coups.
Il evite les fantomes.
Il cherche la nourriture.
Il utilise des poids appris pour choisir.
```
