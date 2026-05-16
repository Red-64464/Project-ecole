# Projet Pac-Man ALG4

Ce dossier contient le projet final RLMinimax (Ilias).

Les fichiers Python importants restent a la racine pour que les commandes
fonctionnent simplement. Les fichiers pedagogiques sont dans `Explication/`.

## Fichiers principaux

| Fichier | Role |
|---|---|
| `features.py` | Calcule les facteurs `x_i(s)` de la fonction d'evaluation |
| `rlMinimaxAgents.py` | Agent final `RLMinimaxAgent` (Alpha-Beta + evaluation affine) |
| `train.py` | Apprend les poids par renforcement (TD) |
| `compare.py` | Compare RLMinimax avec Minimax et AlphaBeta |
| `multiAgents.py` | ReflexAgent, Minimax, AlphaBeta, betterEvaluationFunction |
| `weights.json` | Poids finaux appris (7 poids + biais) |
| `plot_comparison.py` | Genere les graphiques de comparaison sur les 10 layouts |
| `generate_rapport.py` | Script source pour regenerer le PDF final |
| `Rapport Final.pdf` | Rapport final officiel a remettre au professeur |
| `RESULTATS_FINAUX_RL_MINIMAX.md` | Tableau brut des resultats avec commandes reproductibles |

## Commandes utiles

Verifier la syntaxe :

```bash
python -m py_compile compare.py train.py rlMinimaxAgents.py features.py multiAgents.py
```

Comparer les agents :

```bash
python compare.py --num-games 100 --layout smallClassic --depth 2 --baseline both
```

Lancer Pac-Man avec RLMinimax :

```bash
python pacman.py -p RLMinimaxAgent -l smallClassic -a depth=2
```

Generer les graphiques de comparaison (sortie dans `output/`) :

```bash
python plot_comparison.py
```

Regenerer le PDF du rapport final :

```bash
python generate_rapport.py
```

Attention : `python train.py` relance un entrainement et reecrit `weights.json`.
Pour la remise, les poids finaux sont deja dans `weights.json`.

Les scripts `run.ps1` et `run.sh` utilisent les poids existants par defaut.
Pour relancer volontairement l'entrainement :

```powershell
.\run.ps1 -Train
```

```bash
RUN_TRAIN=1 ./run.sh
```

## Documents a lire

| Document | Utilite |
|---|---|
| `Rapport Final.pdf` | PDF final a remettre : objectifs, formule, poids, resultats, limites, conclusion |
| `RESULTATS_FINAUX_RL_MINIMAX.md` | Tableau brut des comparaisons avec commandes |
| `Explication/README.md` | Guide des fichiers d'explication |
| `Explication/EXPLICATION_PROJET_ALGO4_PACMAN.md` | Explication simple du projet |
| `Explication/ORDRE_COMPRENDRE_CLASSES.md` | Ordre conseille pour comprendre les classes |
| `Explication/ANALYSE_GRILLE_EVALUATION.md` | Analyse de la grille d'evaluation |
| `Explication/LABO/LABO1_NOTIONS_MINIMAX_ALPHABETA.md` | Rappel Minimax / AlphaBeta |
| `Explication/LABO/LABO2_NOTIONS_REINFORCEMENT_LEARNING.md` | Rappel Reinforcement Learning |

## Idee du projet

`RLMinimaxAgent` utilise Alpha-Beta avec une fonction d'evaluation affine :

```text
f(s) = a1*x1(s) + a2*x2(s) + ... + a7*x7(s) + C
```

Les `x_i(s)` viennent de `features.py`.
Les poids `a_i` et la constante `C` sont appris par renforcement
puis stockes dans `weights.json`.
