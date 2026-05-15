# README principal du projet - partie 2

---

## 7. Comparaison finale ðŸ“Š

La grille demande une comparaison entre `RLMinimaxAgent` et l'agent Minimax/AlphaBeta du TD.

Ici, la comparaison est faite avec :

```text
MinimaxAgent local ou AlphaBetaAgent local depuis multiAgents.py
vs
RLMinimaxAgent avec poids appris
```

`compare.py` accepte maintenant :

```bash
--baseline minimax
--baseline alphabeta
--baseline both
```

Pourquoi ?

```text
La grille cite MinimaxAgent.
AlphaBetaAgent est aussi utile car c'est une baseline plus forte et plus rapide.
```

RÃ©sultats finaux principaux :

| Layout | Depth | Parties | AlphaBeta score moyen | RLMinimax score moyen | DiffÃ©rence | AlphaBeta winrate | RLMinimax winrate |
|---|---:|---:|---:|---:|---:|---:|---:|
| `testClassic` | 1 | 3000 | 547.9 | 552.0 | +4.1 | 100% | 100% |
| `smallClassic` | 2 | 1500 | -152.8 | 157.4 | +310.2 | 1% | 29% |
| `capsuleClassic` | 2 | 1000 | -253.7 | -55.3 | +198.4 | 0% | 19% |
| `mediumClassic` | 1 | 500 | 45.0 | 648.6 | +603.5 | 3% | 39% |
| `originalClassic` | 1 | 300 | 345.4 | 815.0 | +469.6 | 0% | 3% |

Conclusion :

```text
RLMinimax bat AlphaBeta en score moyen sur toutes les cartes testÃ©es.
```

AprÃ¨s ajout de la comparaison avec `MinimaxAgent`, les poids actuels ont aussi Ã©tÃ© testÃ©s dans :

```text
TEST_POIDS_ACTUELS_COMPARAISON.md
```

Point honnÃªte mis Ã  jour :

```text
RLMinimax bat Minimax et AlphaBeta sur les layouts utiles testÃ©s
avec une profondeur adaptÃ©e.
minimaxClassic demande depth 3.
trappedClassic demande depth 1.
openClassic fonctionne trÃ¨s bien avec RLMinimax,
mais les baselines Minimax/AlphaBeta sont trop lentes pour une comparaison complÃ¨te.
```

Important :

```text
Le PDF et la grille ne demandent pas un winrate minimum fixe.
Ils demandent une comparaison statistique sÃ©rieuse et commentÃ©e.
```

---

## 8. Robustesse des poids ðŸ’£

Le fichier `weights.json` est validÃ©.

Cas testÃ©s :

| Cas | RÃ©sultat attendu |
|---|---|
| `nan` | refusÃ© |
| `inf` | refusÃ© |
| `1e308` | refusÃ© |
| `1000001` | refusÃ© |
| `true` | refusÃ© |
| `"abc"` | refusÃ© |
| mauvais nombre de poids | refusÃ© |
| JSON cassÃ© | fallback propre |

But :

```text
MÃªme si weights.json est mauvais, le projet ne doit pas exploser.
```

---

## 9. Commandes reproductibles ðŸ§ª

VÃ©rification syntaxe :

```bash
python -m py_compile compare.py pacman.py layout.py train.py rlMinimaxAgents.py features.py multiAgents.py
```

DÃ©monstration rapide :

```bash
python pacman.py -p RLMinimaxAgent -l testClassic -a depth=1 -n 3 -q
```

Comparaison rapide :

```bash
python compare.py --num-games 100 --layout smallClassic --depth 2 --baseline both
```

Comparaison sÃ©rieuse :

```bash
python compare.py --num-games 1500 --layout smallClassic --depth 2 --baseline alphabeta
```

Comparaison qui colle littÃ©ralement Ã  la grille :

```bash
python compare.py --num-games 500 --layout smallClassic --depth 2 --baseline minimax
```

EntraÃ®nement contrÃ´lÃ© :

```bash
python train.py --resume --num-games 1000 --layout smallClassic --depth 2 --alpha 0.0003 --epsilon 0.03 --eval-games 100
```

---

## 10. Limites honnÃªtes du projet âš ï¸

MÃªme si les rÃ©sultats sont bons, il reste des limites :

```text
1. RLMinimax n'a pas 100% de winrate sur les grandes cartes.
2. Un meilleur score moyen ne veut pas toujours dire plus de victoires.
3. L'entraÃ®nement peut amÃ©liorer une carte et en dÃ©grader une autre.
4. La fonction affine reste un modÃ¨le simple.
5. Les features sont fortes mais ne dÃ©crivent pas tout le futur possible.
```

Ces limites sont normales.
Elles sont aussi dÃ©fendables Ã  l'oral.

---

## 11. Lien avec la grille d'Ã©valuation âœ…

| CritÃ¨re de la grille | Ã‰tat du projet |
|---|---|
| Fonction d'Ã©valuation affine | RespectÃ©e avec `f(s)=Î£ ai*xi(s)+C` |
| Facteurs liÃ©s Ã  Pac-Man | Oui : nourriture, fantÃ´mes, capsules, score |
| Facteurs distincts | Oui, chaque feature a un rÃ´le diffÃ©rent |
