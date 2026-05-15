## 10. Formule TD utilisée dans notre projet 🧾

Dans `train.py`, l'idée est :

```text
target = reward + gamma * V(s')
error = target - V(s)
weight_i = weight_i + alpha * error * x_i(s)
```

Explication très simple :

```text
target = ce qu'on aurait dû prévoir
V(s) = ce qu'on avait prévu
error = l'écart entre les deux
alpha = vitesse de correction
x_i = feature concernée
```

---

## 11. Pseudo-code d'entraînement avec explication ligne par ligne 🎓

Pseudo-code :

```text
initialiser les poids

pour chaque partie:
    jouer une partie avec RLMinimaxAgent
    enregistrer les états visités

    pour chaque transition s -> s':
        reward = score(s') - score(s)
        value_now = V(s)
        value_next = V(s')
        target = reward + gamma * value_next
        error = target - value_now

        pour chaque poids i:
            poids_i = poids_i + alpha * error * feature_i(s)

    sauvegarder les meilleurs poids
```

Explication :

| Ligne | Sens simple |
|---|---|
| `initialiser les poids` | On commence avec des poids existants ou aléatoires |
| `pour chaque partie` | Pac-Man joue beaucoup de parties |
| `jouer une partie` | On observe ce qui arrive |
| `enregistrer les états` | On garde l'historique du jeu |
| `pour chaque transition s -> s'` | On regarde chaque passage d'un état au suivant |
| `reward = score(s') - score(s)` | On mesure si le score a monté ou baissé |
| `value_now = V(s)` | Ce que Pac-Man pensait de l'état actuel |
| `value_next = V(s')` | Ce que Pac-Man pense de l'état suivant |
| `target = reward + gamma * value_next` | Ce qu'il aurait dû viser |
| `error = target - value_now` | Son erreur de jugement |
| `poids_i += ...` | On corrige chaque poids |
| `sauvegarder` | On garde les bons poids dans `weights.json` |

---

## 12. Alpha : vitesse d'apprentissage 🚗

`alpha` dit à quelle vitesse les poids changent.

Si alpha est trop grand :

```text
les poids bougent trop fort
l'apprentissage devient instable 😬
```

Si alpha est trop petit :

```text
les poids changent très lentement 🐢
```

Dans nos entraînements, on utilise des petites valeurs :

```text
0.001
0.0007
0.0005
0.0003
```

Pourquoi ?

```text
Pour apprendre doucement sans casser les bons comportements.
```

---

## 13. Gamma : importance du futur 🔮

`gamma` dit combien Pac-Man tient compte du futur.

Exemple :

```text
gamma = 0
=> je regarde seulement la récompense immédiate

gamma = 0.9
=> je regarde beaucoup le futur
```

Dans Pac-Man, le futur est important.

Pourquoi ?

```text
Une action peut sembler bonne maintenant,
mais mener à un fantôme deux coups plus tard.
```

---
