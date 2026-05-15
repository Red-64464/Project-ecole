# README principal du projet - partie 1

# Projet ALG4 - Pac-Man RLMinimax ðŸŽ®ðŸ¤–

Ce README est le rapport principal du projet.
Il rÃ©sume ce qui a Ã©tÃ© fait, comment lancer le code, comment lire les rÃ©sultats, et pourquoi le projet respecte la grille d'Ã©valuation.

---

## 1. But du projet ðŸŽ¯

Le but est de construire un agent Pac-Man appelÃ© `RLMinimaxAgent`.

Cet agent combine :

```text
Minimax + Ã©lagage alpha-bÃªta
+
fonction d'Ã©valuation affine
+
poids appris par reinforcement learning
```

Formule utilisÃ©e :

```text
f(s) = a1*x1(s) + a2*x2(s) + ... + a7*x7(s) + C
```

Explication simple :

```text
x_i = une feature, donc une information sur le jeu
a_i = un poids appris
C = le biais
f(s) = la note donnÃ©e Ã  une situation
```

---

## 2. Fichiers importants ðŸ“¦

| Fichier | RÃ´le |
|---|---|
| `features.py` | Calcule les 7 features utilisÃ©es dans la fonction affine |
| `rlMinimaxAgents.py` | Contient l'agent final `RLMinimaxAgent` |
| `train.py` | EntraÃ®ne les poids par reinforcement learning |
| `compare.py` | Compare `RLMinimaxAgent` avec `AlphaBetaAgent` |
| `multiAgents.py` | Contient les agents du TD : `MinimaxAgent`, `AlphaBetaAgent`, etc. |
| `weights.json` | Stocke les poids appris |
| `LABO1_NOTIONS_MINIMAX_ALPHABETA.md` | Explique Minimax et Alpha-Beta simplement |
| `LABO2_NOTIONS_REINFORCEMENT_LEARNING.md` | Explique le reinforcement learning simplement |
| `EXPLICATION_PROJET_ALGO4_PACMAN.md` | Explication complÃ¨te du projet |
| `ordre.md` | Ordre conseillÃ© pour comprendre le projet |

---

## 3. Features utilisÃ©es ðŸ§©

Les 7 features sont :

| Feature | Sens simple | Pourquoi elle est utile |
|---|---|---|
| `x1` | nourriture proche | Pac-Man doit chercher les gommes |
| `x2` | fantÃ´mes dangereux proches | Pac-Man doit Ã©viter de mourir |
| `x3` | fantÃ´mes effrayÃ©s proches | Pac-Man peut les manger |
| `x4` | nourriture restante | Pac-Man doit finir la carte |
| `x5` | capsule proche | Les capsules aident contre les fantÃ´mes |
| `x6` | score actuel | Le score rÃ©sume dÃ©jÃ  une partie de la rÃ©ussite |
| `x7` | fantÃ´me dangereux le plus proche | Pac-Man doit rÃ©agir au danger immÃ©diat |

Les distances importantes utilisent un BFS.
Cela permet de tenir compte des murs du labyrinthe.

---

## 4. Poids finaux retenus âš–ï¸

Les poids finaux sont dans `weights.json` :

```json
{
  "weights": [0.6891, -1.4093, 1.4061, 0.4036, 0.0252, 1.8572, -1.2624],
  "bias": -0.0834
}
```

Lecture simple :

| Poids | InterprÃ©tation |
|---|---|
| `0.6891` | nourriture proche utile |
| `-1.4093` | fantÃ´mes dangereux proches mauvais |
| `1.4061` | fantÃ´mes effrayÃ©s proches utiles |
| `0.4036` | rÃ©duire la nourriture restante aide Ã  finir |
| `0.0252` | capsules utiles mais moins dominantes |
| `1.8572` | score actuel trÃ¨s important |
| `-1.2624` | fantÃ´me dangereux proche trÃ¨s mauvais |
| `-0.0834` | biais lÃ©ger |

---

## 5. EntraÃ®nement rÃ©alisÃ© ðŸŽ“

L'entraÃ®nement apprend les poids de la fonction d'Ã©valuation.
Il ne remplace pas Minimax.
Il amÃ©liore seulement la maniÃ¨re de noter les Ã©tats.

MÃ©thode utilisÃ©e :

```text
1. jouer des parties
2. enregistrer les Ã©tats vus
3. calculer reward = score_aprÃ¨s - score_avant
4. calculer l'erreur TD
5. ajuster les poids
6. garder les meilleurs poids
```

ParamÃ¨tres utilisÃ©s pendant les tests :

| ParamÃ¨tre | RÃ´le |
|---|---|
| `alpha` | vitesse d'apprentissage |
| `epsilon` | exploration pendant l'entraÃ®nement |
| `gamma` | importance du futur |
| `num-games` | nombre de parties d'entraÃ®nement |
| `eval-games` | parties d'Ã©valuation sans apprentissage |

Exemples de blocs testÃ©s :

```bash
python train.py --resume --num-games 1000 --layout originalClassic --depth 1 --alpha 0.0003 --epsilon 0.03 --eval-games 100
python train.py --resume --num-games 1000 --layout capsuleClassic --depth 2 --alpha 0.00025 --epsilon 0.04 --eval-games 100
python train.py --resume --num-games 500 --layout smallClassic --depth 2 --alpha 0.0002 --epsilon 0.03 --eval-games 80
```

RÃ©sultat de l'analyse :

```text
Certains entraÃ®nements amÃ©lioraient une carte,
mais baissaient les performances sur d'autres cartes.
Les poids finaux actuels ont donc Ã©tÃ© gardÃ©s car ils sont plus stables globalement.
```

---

## 6. Pourquoi amÃ©liorer une carte peut casser les autres ? ðŸ—ºï¸

Un poids est comme une habitude.

Exemple :

```text
Si on renforce trop les capsules,
Pac-Man peut devenir meilleur sur capsuleClassic,
mais perdre du temps sur une carte oÃ¹ les capsules sont moins importantes.
```

Donc on ne garde pas seulement les poids qui gagnent une carte.
On garde les poids qui restent bons sur plusieurs layouts.

