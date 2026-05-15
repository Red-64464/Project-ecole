# Analyse de la grille evaluation - partie 1

# Analyse de la grille d'Ã©valuation âœ…ðŸ“‹

Ce fichier rÃ©sume l'analyse du PDF `EVALUATION.pdf`.
Les captures page par page ont Ã©tÃ© gÃ©nÃ©rÃ©es dans :

```text
evaluation_screenshots/
```

Important :

```text
La grille est une apprÃ©ciation globale.
Elle ne donne pas seulement des points automatiques.
Elle vÃ©rifie surtout la comprÃ©hension, la qualitÃ© du code, les tests et la dÃ©fense orale.
```

---

## 1. Pages capturÃ©es ðŸ“¸

| Page | Capture |
|---|---|
| 1 | `evaluation_screenshots/evaluation_page_1.png` |
| 2 | `evaluation_screenshots/evaluation_page_2.png` |
| 3 | `evaluation_screenshots/evaluation_page_3.png` |
| 4 | `evaluation_screenshots/evaluation_page_4.png` |
| 5 | `evaluation_screenshots/evaluation_page_5.png` |
| 6 | `evaluation_screenshots/evaluation_page_6.png` |

---

## 2. CritÃ¨res gÃ©nÃ©raux utiles ðŸ§±

| CritÃ¨re | Ce que demande la grille | Ã‰tat du projet | Statut |
|---|---|---|---|
| Git | Commits rÃ©guliers, structure claire | Un dossier base GitLab a Ã©tÃ© crÃ©Ã© pour push progressif | Ã€ gÃ©rer dans GitLab |
| README | Rapport avec explications, commandes, scores | `README.md` crÃ©Ã© dans `Projet` | âœ… Fait |
| QualitÃ© du code | Code clair, commentaires utiles, pas de code mort | Code commentÃ© et structurÃ© | âœ… Fait |
| Structure | `lab-1 / lab-2 / project / README.md` dans le dÃ©pÃ´t | Le dossier final est `Projet`; attention Ã  la structure GitLab finale | Ã€ gÃ©rer au moment du push |

Point important :

```text
Le code ne peut pas crÃ©er un historique Git propre tout seul.
Il faudra faire les commits progressivement dans GitLab.
```

---

## 3. CritÃ¨res du projet dans la grille ðŸŽ¯

Les critÃ¨res projet sont surtout en bas des pages 5 et 6.

---

## 4. Projet - ModÃ©lisation de la fonction d'Ã©valuation ðŸ§®

### Ce que demande la grille

Pour Ãªtre au niveau maÃ®trisÃ©/excellent :

```text
La fonction respecte la structure affine.
Les facteurs sont pertinents.
Les facteurs sont distincts.
L'Ã©tudiant sait expliquer le rÃ´le de chaque facteur.
Il sait discuter les limites et interactions.
```

### Ce qui est fait

Dans `rlMinimaxAgents.py` :

```text
f(s) = a1*x1(s) + a2*x2(s) + ... + a7*x7(s) + C
```

Dans `features.py`, les 7 facteurs sont :

```text
x1 : nourriture proche
x2 : fantÃ´mes dangereux proches
x3 : fantÃ´mes effrayÃ©s proches
x4 : nourriture restante
x5 : capsule proche
x6 : score actuel
x7 : fantÃ´me dangereux le plus proche
```

### Verdict

```text
CritÃ¨re respectÃ©.
Niveau visÃ© : excellente maÃ®trise.
```

Pourquoi :

```text
Chaque feature dÃ©crit un aspect diffÃ©rent de Pac-Man.
Le README explique leur rÃ´le.
Le fichier EXPLICATION_PROJET_ALGO4_PACMAN.md donne l'explication dÃ©taillÃ©e.
```

---

## 5. Projet - Apprentissage des poids par renforcement ðŸŽ“

### Ce que demande la grille

Pour Ãªtre au niveau maÃ®trisÃ©/excellent :

```text
Les poids sont appris par renforcement.
L'Ã©tudiant sait distinguer apprendre une fonction d'Ã©valuation et apprendre une politique directe.
Il documente les hyperparamÃ¨tres.
Il analyse la signification des poids.
```

### Ce qui est fait

Dans `train.py` :

```text
reward = score_aprÃ¨s - score_avant
target = reward + gamma * V(s')
error = target - V(s)
poids_i += alpha * error * x_i(s)
```

Les poids sont sauvegardÃ©s dans :

```text
weights.json
```

Poids finaux :

```json
{
  "weights": [0.6891, -1.4093, 1.4061, 0.4036, 0.0252, 1.8572, -1.2624],
  "bias": -0.0834
}
```

### Verdict

```text
CritÃ¨re respectÃ©.
Niveau visÃ© : maÃ®trisÃ© Ã  excellente maÃ®trise.
```

Pourquoi :

```text
Le projet apprend des paramÃ¨tres d'une fonction d'Ã©valuation.
Il n'apprend pas une politique directe.
Cette diffÃ©rence est expliquÃ©e dans README.md et LABO2_NOTIONS_REINFORCEMENT_LEARNING.md.
```
