# Projet_BASE_GITLAB 🙂

Ce dossier est une base propre pour faire un premier push GitLab progressif.

Il contient seulement le squelette Berkeley minimal :

```text
moteur Pac-Man
règles du jeu
fantômes
affichage
layouts
PDF du projet
```

Il ne contient pas encore le travail final du projet :

```text
pas de RLMinimaxAgent
pas de features.py
pas de train.py
pas de compare.py
pas de weights.json
pas de fiches Markdown pédagogiques
pas de multiAgents.py complété
```

Idée de push progressif :

```text
Commit 1 : base Berkeley minimale
Commit 2 : ajout multiAgents.py avec Minimax/AlphaBeta du TD
Commit 3 : ajout features.py
Commit 4 : ajout rlMinimaxAgents.py
Commit 5 : ajout train.py
Commit 6 : ajout compare.py
Commit 7 : ajout weights.json + documentation
```

Test rapide possible dans cette base :

```bash
python pacman.py -p GreedyAgent -l testClassic -n 1 -q
```
