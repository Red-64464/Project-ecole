# Plan de creation des fichiers pour le depot GitLab

Ce fichier sert si je pars du dossier `Projet_BASE_GITLAB` pour reconstruire mon projet final proprement dans le depot GitLab.

## 1. Etat du dossier de base

Le dossier `Projet_BASE_GITLAB` est bon comme base Berkeley minimale.

Verifications faites :

```bash
python -m py_compile game.py pacman.py layout.py ghostAgents.py textDisplay.py util.py pacmanAgents.py keyboardAgents.py graphicsDisplay.py graphicsUtils.py
python pacman.py -p GreedyAgent -l testClassic -q -n 1 -f
```

Resultat :

```text
Compilation OK.
Pac-Man se lance correctement.
GreedyAgent gagne une partie test avec un score de 434.
```

Important : cette base ne contient pas encore mon projet final RLMinimax. Elle contient surtout le moteur Pac-Man Berkeley, les layouts et le PDF du sujet.

## 2. Fichiers a creer / ajouter pour obtenir le projet final

| Ordre | Fichier a creer ou copier | Pourquoi il est utile | Message de commit conseille |
|---:|---|---|---|
| 1 | `.gitignore` | Eviter de pousser les fichiers inutiles comme `__pycache__` | `chore: add project gitignore` |
| 2 | `multiAgents.py` | Ajouter les agents de reference Minimax et AlphaBeta | `feat: add minimax and alphabeta baseline agents` |
| 3 | `features.py` | Ajouter les facteurs `x_i(s)` de la fonction affine | `feat: add pacman feature extraction module` |
| 4 | `rlMinimaxAgents.py` | Ajouter la classe finale `RLMinimaxAgent` | `feat: add RLMinimax agent with alpha beta search` |
| 5 | `train.py` | Ajouter l'apprentissage des poids par renforcement | `feat: add reinforcement training for evaluation weights` |
| 6 | `weights.json` | Ajouter les poids finaux appris | `chore: add learned RLMinimax weights` |
| 7 | `compare.py` | Ajouter les comparaisons Minimax / AlphaBeta / RLMinimax | `feat: add agent comparison script` |
| 8 | `run.ps1` | Ajouter un script de lancement Windows | `chore: add PowerShell project runner` |
| 9 | `run.sh` | Ajouter un script de lancement Linux/Mac | `chore: add shell project runner` |
| 10 | `README.md` | Expliquer le projet, les commandes et les fichiers | `docs: add project README and usage commands` |
| 11 | `RESULTATS_FINAUX_RL_MINIMAX.md` | Documenter les resultats finaux | `docs: add final RLMinimax results` |
| 12 | `RAPPORT_POIDS_ET_RESULTATS.md` | Expliquer les poids et les resultats | `docs: add weights and results report` |
| 13 | `TEST_POIDS_ACTUELS_COMPARAISON.md` | Garder une trace courte des tests des poids actuels | `docs: add current weights comparison note` |
| 14 | `COMMANDES_RAPPORT_FINAL.txt` | Donner les commandes utilisees dans le rapport | `docs: add final report commands` |
| 15 | `CONTROLE_FINAL_CONFORMITE.md` | Montrer la conformite avec le PDF et la grille | `docs: add final conformity checklist` |
| 16 | `VERIFICATION_FINALE_STRICTE.md` | Garder la verification stricte avant remise | `docs: add strict final verification report` |
| 17 | `Rapport Final.pdf` | Ajouter le rapport PDF final | `docs: add final project report PDF` |
| 18 | `Explication/README.md` | Expliquer comment lire les fichiers d'explication | `docs: add explanation guide` |
| 19 | `Explication/ordre.md` | Donner l'ordre rapide de revision | `docs: add quick revision order` |
| 20 | `Explication/ORDRE_COMPRENDRE_CLASSES.md` | Donner l'ordre complet pour comprendre les classes | `docs: add class understanding order` |
| 21 | `Explication/EXPLICATION_PROJET_ALGO4_PACMAN.md` | Expliquer le projet simplement | `docs: add simple project explanation` |
| 22 | `Explication/ANALYSE_GRILLE_EVALUATION.md` | Relier le projet a la grille d'evaluation | `docs: add evaluation grid analysis` |
| 23 | `Explication/LABO/LABO1_NOTIONS_MINIMAX_ALPHABETA.md` | Rappel Minimax et AlphaBeta | `docs: add minimax alphabeta lab notes` |
| 24 | `Explication/LABO/LABO2_NOTIONS_REINFORCEMENT_LEARNING.md` | Rappel reinforcement learning | `docs: add reinforcement learning lab notes` |

## 3. Modification a faire dans un fichier Berkeley

Normalement, il ne faut pas modifier les fichiers Berkeley.

Dans mon projet final, un seul fichier Berkeley est different :

| Fichier | Pourquoi il change | Message de commit conseille |
|---|---|---|
| `game.py` | Correction de quelques `print >> sys.stderr` vers `print(..., file=sys.stderr)` pour Python 3 | `fix: update game stderr prints for Python 3` |

Cette modification est petite et defendable : elle ne change pas l'algorithme de Pac-Man, elle corrige seulement l'affichage d'erreurs en Python 3.

## 4. Fichiers a ne pas pousser

Je ne dois normalement pas pousser :

```text
__pycache__/
*.pyc
output/audit_render_Rapport_Final/
anciens rendus PDF temporaires
fichiers de test temporaires
```

Les captures `evaluation_screenshots/` peuvent rester hors remise, sauf si je veux garder les preuves visuelles de verification. Elles ne sont pas necessaires pour lancer le projet.

## 5. Ordre de push conseille

Voici un ordre propre et professionnel :

```text
1. chore: add project gitignore
2. feat: add minimax and alphabeta baseline agents
3. feat: add pacman feature extraction module
4. feat: add RLMinimax agent with alpha beta search
5. feat: add reinforcement training for evaluation weights
6. chore: add learned RLMinimax weights
7. feat: add agent comparison script
8. chore: add project runner scripts
9. docs: add project README and usage commands
10. docs: add final RLMinimax results
11. docs: add weights and results report
12. docs: add final conformity checklist
13. docs: add final project report PDF
14. docs: add explanation and lab notes
```

## 6. Verification apres avoir tout ajoute

Apres avoir ajoute les fichiers dans le depot GitLab, je dois relancer :

```bash
python -m py_compile compare.py game.py pacman.py layout.py train.py rlMinimaxAgents.py features.py multiAgents.py ghostAgents.py textDisplay.py util.py
python pacman.py -p RLMinimaxAgent -l testClassic -a depth=1 -q -n 1 -f
python compare.py --num-games 100 --layout smallClassic --depth 2 --baseline both
```

Resultats attendus :

```text
Compilation sans erreur.
RLMinimax se lance correctement.
Sur smallClassic avec 100 parties, RLMinimax doit retrouver environ 196.6 de score moyen.
```

## 7. Phrase simple a retenir

```text
Je pars de la base Berkeley, puis j'ajoute progressivement mes fichiers :
baselines, features, RLMinimax, entrainement, comparaison, poids et documentation.
```
