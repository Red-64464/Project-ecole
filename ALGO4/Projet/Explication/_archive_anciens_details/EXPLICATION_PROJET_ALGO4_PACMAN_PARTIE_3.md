## 11. C’est quoi Q-Learning ?

Q-Learning est une méthode où l'agent apprend la valeur d'une action dans un état.

On note :

```text
Q(s, a)
```

Cela veut dire :

```text
"Si je suis dans l'état s et que je fais l'action a,
à quel point est-ce une bonne idée ?"
```

Formule simplifiée :

```text
nouvelle valeur =
ancienne valeur + alpha * erreur
```

L'erreur regarde :

- la récompense immédiate
- la valeur future estimée
- ce que l'agent croyait avant

## 12. C’est quoi une Q-value ?

Une Q-value est une note pour un couple :

```text
état + action
```

Exemple :

```text
Q(position devant nourriture, East) = très bon
Q(position devant fantôme, East) = très mauvais
```

Dans le Lab 2, les Q-values sont stockées directement dans un dictionnaire.  
Dans ton projet final, on ne stocke pas toutes les Q-values. On apprend plutôt des poids pour une fonction d'évaluation.

## 13. C’est quoi getQValue ?

Dans un Q-agent classique, `getQValue(state, action)` retourne :

```text
la valeur Q(s, a)
```

Si l'agent n'a jamais vu ce couple état-action, on retourne souvent `0.0`.

## 14. C’est quoi computeValueFromQValues ?

Cette fonction répond à la question :

```text
Depuis cet état, quelle est la meilleure valeur possible ?
```

Formule :

```text
V(s) = max_a Q(s, a)
```

Si aucune action n'est possible, c'est un état terminal, donc la valeur est `0.0`.

## 15. C’est quoi computeActionFromQValues ?

Cette fonction choisit la meilleure action selon les Q-values.

```text
meilleure action = action avec la plus grande Q-value
```

S'il y a égalité, on peut choisir au hasard parmi les meilleures.

## 16. C’est quoi getAction ?

`getAction` choisit l'action réellement jouée.

En apprentissage, il y a deux comportements :

- exploration : tester une action au hasard
- exploitation : choisir la meilleure action connue

Avec epsilon-greedy :

```text
avec probabilité epsilon -> action aléatoire
sinon -> meilleure action connue
```

Dans ton `train.py`, l'option `--epsilon` sert à explorer pendant l'entraînement.

## 17. C’est quoi update ?

`update` est la fonction qui apprend.

Elle prend :

- l'état actuel
- l'action choisie
- l'état suivant
- la récompense obtenue

Puis elle corrige les valeurs internes de l'agent.

Dans ton projet, la correction se fait sur les poids :

```text
w_i = w_i + alpha * gradient_i
```
