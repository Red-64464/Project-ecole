# Verification finale stricte - Projet Pac-Man RLMinimax

Date de verification : 14 mai 2026.

## 1. Etat global

Statut : presque termine et conforme pour la partie technique.

Le projet respecte l'objectif principal du PDF : creer un agent `RLMinimaxAgent` qui combine Alpha-Beta avec une fonction d'evaluation affine dont les poids sont appris par renforcement.

Point a verifier hors code avant remise : le vrai depot Git/Gitesi, car le dossier local analyse ici n'est pas un depot Git complet avec historique.

## 2. Table de conformite PDF / grille / projet

| Point demande par le PDF / grille | Statut | Preuve dans le projet | Remarque |
|---|---|---|---|
| Combiner Minimax Alpha-Beta et apprentissage par renforcement | OK | `rlMinimaxAgents.py`, classe `RLMinimaxAgent`, ligne 133 | L'agent utilise Alpha-Beta et une evaluation apprise. |
| Utiliser une fonction affine `sum(ai*xi)+C` | OK | `rlMinimaxAgents.py`, `rl_evaluation_function`, ligne 115 | Pas de termes croises, pas de carres, pas de division par feature. |
| Avoir des facteurs `xi(s)` lies a Pac-Man | OK | `features.py`, `FEATURE_FUNCTIONS`, ligne 209 | Nourriture, fantomes, capsules, score et nourriture restante. |
| Certains facteurs font de la recherche | OK | `features.py`, `_bfs_distance`, puis features BFS | BFS calcule des distances reelles dans le labyrinthe. |
| Poids independants de l'etat | OK | `weights.json` | 7 poids + un biais `C`, separes du code. |
| Apprendre les poids par renforcement | OK | `train.py`, `_td_update`, ligne 139 ; `train`, ligne 184 | L'apprentissage ajuste les poids, pas une politique directe. |
| Ne pas apprendre une strategie directe | OK | `train.py`, classe `TrainingRLMinimaxAgent`, ligne 19 | L'historique sert a corriger les coefficients de l'evaluation. |
| Agent final appele RLMinimax | OK | `rlMinimaxAgents.py`, classe `RLMinimaxAgent`, ligne 133 | Le nom correspond a l'attente du sujet. |
| Comparer RLMinimax a Minimax | OK | `compare.py`, `run_games`, ligne 28 ; resultats dans `RESULTATS_FINAUX_RL_MINIMAX.md` | C'est l'exigence principale du PDF. |
| Comparer aussi a AlphaBeta | OK bonus | `compare.py`, option `--baseline both` | Plus complet que le minimum demande. |
| Produire des statistiques | OK | `compare.py`, `summarize`, ligne 84 | Score moyen, winrate, temps, coups et Stop moyen. |
| Resultats reproductibles | OK | `RESULTATS_FINAUX_RL_MINIMAX.md` | Les commandes et nombres de parties sont listes. |
| Gain de RLMinimax prouve par les chiffres | OK | `output/verification/comparaison_finale_2026-05-14.json` | Diff positive sur les 10 layouts testes. |
| Rapport clair et defendable | OK | `Rapport Final.pdf` | Rapport final 3 pages : conformite sujet, fonction d'evaluation, resultats sur 10 layouts, limites, conclusion. |
| README present | OK | `README.md` | Corrige pour pointer vers le bon rapport conseille. |
| Scripts de lancement | OK apres correction | `run.ps1`, `run.sh` | Ils n'ecrasent plus les poids par defaut. |
| Code clair/commentaires | OK apres correction | `features.py`, `rlMinimaxAgents.py`, `train.py`, `multiAgents.py`, `compare.py` | Docstrings ajoutees ou completees dans les fichiers importants. |
| Distinction Berkeley / ajouts personnels | OK | Comparaison avec `Projet_BASE_GITLAB` | Ajouts : `features.py`, `rlMinimaxAgents.py`, `train.py`, `compare.py`, `multiAgents.py`, rapports. |
| Ne pas modifier Berkeley inutilement | OK avec reserve | Seul `game.py` differe de la base | Difference limitee a des `print(..., file=sys.stderr)` Python 3. |
| Compilation Python | OK | Commande `python -m py_compile ...` | Aucune erreur de syntaxe. |
| Lancement de RLMinimax | OK | `python pacman.py -p RLMinimaxAgent -l testClassic -a depth=1 -q -n 1 -f` | Pac-Man gagne, score 554. |
| Comparaison officielle smallClassic | OK | `python compare.py --num-games 100 --layout smallClassic --depth 2 --baseline both` | Resultat reproduit : RLMinimax 196.6 contre -149.8. |
| Scripts `run.ps1` et `run.sh` | OK | Lances apres correction | Les scripts sautent l'entrainement par defaut et gardent `weights.json`. |
| Gestion Git / commits hebdomadaires | A verifier | Non visible dans ce dossier local | Verifier sur Gitesi avant remise. |

## 3. Fichiers analyses

- Sujet : `projet.pdf` + captures `evaluation_screenshots/projet/projet_page_01.png` a `projet_page_03.png`.
- Grille : `evaluation_screenshots/evaluation_extracted_text.txt` + captures `evaluation_screenshots/evaluation/evaluation_page_01.png` a `evaluation_page_06.png`.
- Explications : `Explication/README.md`, `Explication/ordre.md`, `Explication/EXPLICATION_PROJET_ALGO4_PACMAN.md`, `Explication/ORDRE_COMPRENDRE_CLASSES.md`, `Explication/ANALYSE_GRILLE_EVALUATION.md`.
- Labos : `Explication/LABO/LABO1_NOTIONS_MINIMAX_ALPHABETA.md`, `Explication/LABO/LABO2_NOTIONS_REINFORCEMENT_LEARNING.md`.
- Code important : `features.py`, `rlMinimaxAgents.py`, `train.py`, `compare.py`, `multiAgents.py`, `weights.json`.
- Base Berkeley : `game.py`, `pacman.py`, `layout.py`, `util.py`, `ghostAgents.py`, `textDisplay.py`, `pacmanAgents.py`, `keyboardAgents.py`.
- Resultats : `RESULTATS_FINAUX_RL_MINIMAX.md`, `RAPPORT_POIDS_ET_RESULTATS.md`, `output/verification/comparaison_finale_2026-05-14.json`, `output/verification/comparaison_finale_2026-05-14.csv`.
- Rapports PDF : `Rapport Final.pdf` (rapport final officiel), versions dans `output/pdf/`.

## 4. Fichiers modifies pendant cette verification

- `features.py` : docstrings simples ajoutees pour expliquer les features et le BFS.
- `rlMinimaxAgents.py` : docstrings ajoutees pour les poids, l'evaluation affine et Alpha-Beta.
- `train.py` : docstrings ajoutees pour l'entrainement et la mise a jour TD.
- `multiAgents.py` : docstrings ajoutees pour Minimax, AlphaBeta et les baselines.
- `compare.py` : docstring ajoutee pour le point d'entree.
- `README.md` : noms de rapport corriges et avertissement ajoute sur `train.py`.
- `CONTROLE_FINAL_CONFORMITE.md` : chemins locaux corriges et test officiel smallClassic ajoute.
- `run.ps1` et `run.sh` : entrainement rendu optionnel pour proteger les poids finaux.
- `weights.json` : restaure aux poids finaux apres un test d'entrainement rapide.

## 5. Erreurs trouvees et corrigees

1. Le README pointait vers `Rapport Final - version defense.pdf`, mais ce fichier n'existait pas a la racine. Correction : le README indique maintenant `Rapport Final.pdf`, qui est le PDF reellement present.
2. Plusieurs fonctions importantes n'avaient pas de docstring. Correction : docstrings ajoutees dans les fichiers personnels.
3. Un test de `train.py` a reecrit `weights.json`. Correction : les poids finaux du rapport ont ete restaures.
4. `CONTROLE_FINAL_CONFORMITE.md` citait des chemins `Downloads`. Correction : chemins remplaces par les fichiers reels du projet.
5. Aucun PDF supplementaire n'est cree : `Rapport Final.pdf` reste le rapport officiel.
6. Les scripts `run.ps1` et `run.sh` relancaient l'entrainement par defaut. Correction : ils utilisent maintenant `weights.json` par defaut, et l'entrainement est volontaire (`-Train` ou `RUN_TRAIN=1`).

## 6. Risques restants

- Git/Gitesi : non verifiable ici. Il faut confirmer les commits/pushs et la structure du vrai depot.
- `openClassic` : resultat favorable a RLMinimax, mais les baselines utilisent un timeout de 1 seconde. A presenter comme cas particulier.
- Winrate : RLMinimax a un meilleur score moyen partout, mais pas un taux de victoire parfait sur les cartes difficiles.
- Entrainement : les poids sont appris, mais il n'y a pas de courbe de convergence ni de variance entre plusieurs runs. Suffisant pour le projet, moins fort pour viser "excellente maitrise".
- Anti-Stop : RLMinimax retire `Stop` quand un mouvement existe. C'est defendable comme anti-blocage, mais il faut l'expliquer si le professeur demande pourquoi toutes les actions ne sont pas gardees.
- Plusieurs PDF existent dans `output/pdf/`. Pour eviter la confusion, utiliser `Rapport Final.pdf` (a la racine) comme rapport final officiel.

## 7. Conclusion

Tu peux rendre le projet pour la partie technique apres verification du depot Git/Gitesi.

Le rapport conseille est `Rapport Final.pdf` a la racine. Il couvre la conformite au sujet, la fonction d'evaluation, les resultats sur 10 layouts et les limites.

Les resultats sont suffisants pour montrer que RLMinimax bat Minimax en score moyen sur les tests documentes. La comparaison avec AlphaBeta est un bonus utile.

Le projet respecte le PDF : fonction affine, facteurs Pac-Man, BFS, Alpha-Beta, poids appris par renforcement, comparaison statistique.

Tu peux defendre le projet a l'oral si tu sais expliquer simplement : features, poids, Alpha-Beta, apprentissage, resultats et limites.

## 8. Questions probables a l'oral

1. Pourquoi le projet n'apprend pas directement une strategie ?
   Reponse : parce que le sujet demande d'apprendre les poids d'une fonction d'evaluation. Alpha-Beta reste la methode qui choisit l'action.

2. C'est quoi une fonction affine ?
   Reponse : c'est une somme de poids fois des features, plus une constante : `a1*x1 + ... + C`.

3. Pourquoi utiliser BFS ?
   Reponse : BFS donne une vraie distance dans le labyrinthe en tenant compte des murs, donc c'est plus fiable que Manhattan.

4. Que representent les poids ?
   Reponse : ils disent l'importance de chaque information. Un poids positif valorise, un poids negatif penalise.

5. Pourquoi le poids des fantomes dangereux est negatif ?
   Reponse : car quand un fantome dangereux est proche, l'etat doit etre moins bon pour Pac-Man.

6. Pourquoi Alpha-Beta et pas seulement Minimax ?
   Reponse : Alpha-Beta donne la meme logique que Minimax, mais coupe des branches inutiles pour gagner du temps.

7. Pourquoi comparer avec Minimax ?
   Reponse : le PDF le demande. Le but est de voir si l'evaluation apprise ameliore l'agent par rapport a la baseline.

8. Pourquoi AlphaBeta et Minimax ont souvent les memes scores ?
   Reponse : car AlphaBeta optimise le calcul, mais ne change pas la decision a profondeur identique.

9. Pourquoi le winrate n'est pas toujours haut ?
   Reponse : certaines cartes sont difficiles et les fantomes restent aleatoires. Le score moyen montre quand meme une meilleure qualite de jeu.

10. Quelle limite principale tu reconnais ?
    Reponse : les poids sont bons sur les tests faits, mais je ne prouve pas qu'ils sont optimaux sur toutes les cartes possibles.
