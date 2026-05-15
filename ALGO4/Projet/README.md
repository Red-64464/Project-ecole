# Projet Pac-Man ALG4

Ce dossier contient le projet final RLMinimax.

Les fichiers Python importants restent a la racine pour que les commandes fonctionnent simplement.
Les fichiers pedagogiques sont ranges dans `Explication/`.

## Fichiers principaux

| Fichier | Role |
|---|---|
| `features.py` | Calcule les facteurs `x_i(s)` de la fonction d'evaluation |
| `rlMinimaxAgents.py` | Contient l'agent final `RLMinimaxAgent` |
| `train.py` | Apprend les poids par renforcement |
| `compare.py` | Compare RLMinimax avec Minimax et AlphaBeta |
| `weights.json` | Stocke les poids appris |
| `Rapport Final.pdf` | Rapport final officiel a remettre au professeur (3 pages, complet) |
| `output/pdf/RAPPORT_FINAL_RLMINIMAX_PROF_FINAL_PRO.pdf` | Version sauvegardee du rapport final |
| `RAPPORT_POIDS_ET_RESULTATS.md` | Rapport professionnel des poids et resultats |
| `CONTROLE_FINAL_CONFORMITE.md` | Checklist finale par rapport au sujet et a la grille |

## Commandes utiles

Verifier la syntaxe :

```bash
python -m py_compile compare.py game.py pacman.py layout.py train.py rlMinimaxAgents.py features.py multiAgents.py
```

Comparer les agents :

```bash
python compare.py --num-games 100 --layout smallClassic --depth 2 --baseline both
```

Lancer Pac-Man avec RLMinimax :

```bash
python pacman.py -p RLMinimaxAgent -l smallClassic -a depth=2
```

Attention : `python train.py` relance un entrainement et reecrit `weights.json`.
Pour la remise, les poids finaux actuels sont deja dans `weights.json`.

Les scripts `run.ps1` et `run.sh` utilisent maintenant les poids existants par
defaut. Pour relancer volontairement l'entrainement :

```powershell
.\run.ps1 -Train
```

```bash
RUN_TRAIN=1 ./run.sh
```

## Documents a lire

| Document | Utilite |
|---|---|
| `Rapport Final.pdf` | PDF final a remettre : objectifs, formule affine, poids, resultats, limites, conclusion |
| `output/pdf/RAPPORT_FINAL_RLMINIMAX_PROF_FINAL_PRO.pdf` | Copie propre du rapport final en style defense orale |
| `RAPPORT_POIDS_ET_RESULTATS.md` | Poids, features, resultats, commandes reproductibles |
| `CONTROLE_FINAL_CONFORMITE.md` | Verification finale contre `projet.pdf` et la grille d'evaluation (`evaluation_screenshots/`) |
| `RESULTATS_FINAUX_RL_MINIMAX.md` | Tableau brut des comparaisons finales |
| `Explication/README.md` | Guide des fichiers d'explication |
| `Explication/EXPLICATION_PROJET_ALGO4_PACMAN.md` | Explication simple du projet |
| `Explication/ORDRE_COMPRENDRE_CLASSES.md` | Ordre conseille pour comprendre les classes |
| `Explication/LABO/LABO1_NOTIONS_MINIMAX_ALPHABETA.md` | Rappel Minimax / AlphaBeta |
| `Explication/LABO/LABO2_NOTIONS_REINFORCEMENT_LEARNING.md` | Rappel Reinforcement Learning |

## Idee du projet

`RLMinimaxAgent` utilise Alpha-Beta avec une fonction d'evaluation affine :

```text
f(s) = a1*x1(s) + a2*x2(s) + ... + a7*x7(s) + C
```

Les `x_i(s)` viennent de `features.py`.
Les poids `a_i` et la constante `C` sont appris puis stockes dans `weights.json`.
