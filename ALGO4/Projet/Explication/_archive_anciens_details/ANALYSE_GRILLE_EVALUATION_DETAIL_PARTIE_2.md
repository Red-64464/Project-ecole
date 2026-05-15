# Analyse de la grille evaluation - partie 2


---

## 6. QualitÃ© des tests d'analyse de l'entraÃ®nement ðŸ§ª

### Ce que demande la grille

Pour Ãªtre au niveau maÃ®trisÃ©/excellent :

```text
Tester plusieurs paramÃ¨tres.
Tester plusieurs nombres de parties.
Commenter les rÃ©sultats.
Analyser la robustesse et la stabilitÃ©.
```

### Ce qui est fait

Des entraÃ®nements ont Ã©tÃ© testÃ©s avec variations de :

```text
layout
alpha
epsilon
nombre de parties
profondeur
eval-games
```

Exemples :

```bash
python train.py --resume --num-games 1000 --layout originalClassic --depth 1 --alpha 0.0003 --epsilon 0.03 --eval-games 100
python train.py --resume --num-games 1000 --layout capsuleClassic --depth 2 --alpha 0.00025 --epsilon 0.04 --eval-games 100
python train.py --resume --num-games 500 --layout smallClassic --depth 2 --alpha 0.0002 --epsilon 0.03 --eval-games 80
```

Conclusion :

```text
Les nouveaux poids n'ont pas Ã©tÃ© gardÃ©s quand ils amÃ©lioraient une carte
mais dÃ©gradaient les autres.
```

### Verdict

```text
CritÃ¨re respectÃ©.
Niveau visÃ© : maÃ®trisÃ©.
```

Pour viser encore plus excellent Ã  l'oral :

```text
Expliquer que la stabilitÃ© globale est plus importante qu'un gain isolÃ© sur une seule carte.
```

---

## 7. Projet - Comparaison RLMinimax vs Minimax/AlphaBeta ðŸ“Š

### Ce que demande la grille

Pour Ãªtre au niveau maÃ®trisÃ© :

```text
Comparer les deux agents sur au moins un layout.
Utiliser des statistiques suffisantes : score moyen, taux de victoire.
Documenter et commenter les rÃ©sultats.
```

Pour Ãªtre excellent :

```text
Comparer sur plusieurs layouts et plusieurs mÃ©triques.
Analyser les Ã©carts et les limites.
```

### Ce qui est fait

`compare.py` compare :

```text
MinimaxAgent local ou AlphaBetaAgent local
vs
RLMinimaxAgent
```

Options disponibles :

```bash
python compare.py --baseline minimax
python compare.py --baseline alphabeta
python compare.py --baseline both
```

Cela permet de coller littÃ©ralement Ã  la grille avec `MinimaxAgent`,
et de garder aussi la comparaison plus forte avec `AlphaBetaAgent`.

RÃ©sultats finaux :

| Layout | Depth | Parties | AlphaBeta | RLMinimax | DiffÃ©rence |
|---|---:|---:|---:|---:|---:|
| `testClassic` | 1 | 3000 | 547.9 | 552.0 | +4.1 |
| `smallClassic` | 2 | 1500 | -152.8 | 157.4 | +310.2 |
| `capsuleClassic` | 2 | 1000 | -253.7 | -55.3 | +198.4 |
| `mediumClassic` | 1 | 500 | 45.0 | 648.6 | +603.5 |
| `originalClassic` | 1 | 300 | 345.4 | 815.0 | +469.6 |

Un tableau plus rÃ©cent avec comparaison `MinimaxAgent`, `AlphaBetaAgent` et `RLMinimaxAgent`
est disponible dans :

```text
TEST_POIDS_ACTUELS_COMPARAISON.md
```

Il montre que RLMinimax bat les deux baselines sur les layouts testÃ©s
quand la profondeur est adaptÃ©e.
`minimaxClassic` est meilleur en depth 3.
`trappedClassic` est meilleur en depth 1.
`openClassic` est un cas spÃ©cial : RLMinimax fonctionne vite,
mais les baselines Minimax/AlphaBeta sont trop lentes pour une comparaison complÃ¨te.

### Verdict

```text
CritÃ¨re respectÃ©.
Niveau visÃ© : excellente maÃ®trise.
```

Pourquoi :

```text
Comparaison sur plusieurs layouts.
Plusieurs mÃ©triques utilisÃ©es : score moyen, winrate, Stop, temps, meilleur/pire score.
Les limites sont expliquÃ©es dans README.md.
```

---

## 8. Ce qui a Ã©tÃ© corrigÃ© aprÃ¨s lecture de la grille ðŸ› ï¸

| ProblÃ¨me trouvÃ© | Correction |
|---|---|
| Pas de `README.md` central | CrÃ©ation de `README.md` |
| Pas de `.gitignore` dans le projet | CrÃ©ation de `.gitignore` |
| Besoin de preuves visuelles du PDF | Captures dans `evaluation_screenshots/` |
| Besoin d'analyse claire de la grille | CrÃ©ation de ce fichier |
| La grille cite `MinimaxAgent` prÃ©cisÃ©ment | `compare.py` accepte maintenant `--baseline minimax` |

---

## 9. Ce qui ne se corrige pas dans le code âš ï¸

Certains critÃ¨res dÃ©pendent de GitLab :

```text
commits rÃ©guliers
historique Git lisible
messages de commits clairs
