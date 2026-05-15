# Data

Schéma et données pour le projet *Repartitor*

## Schéma

Le schéma est défini avec toutes ses politiques d'accès dans le fichier `repartitor-tables.sql` fourni.
Vous pouvez définir des **vues** et des **fonctions** supplémentaires, un exemple de chaque est fourni dans le fichier `repartitor-views.sql`.

## Données

Les données sont fournies dans les fichiers `.csv`.
Puisqu'il existe des contraintes de clef étrangère, il faut réfléchir à l'ordre d'importation.

### Sources

 * Les noms (fictifs) des traducteurs et éditeurs proviennent des données du projet [Attendo (2025) par Sébastien Drobisz](https://git.esi-bru.be/sdr/public/4web3/attendo-supabase).
 * Les noms des textes sont des textes disponibles sur [WikiSource anglophone](https://en.wikisource.org/wiki/Main_Page), à savoir les constitutions des états et les textes votés par le parlement anglais entre 1952 et 1991.
 * Le reste a été généré, soit par une formule (par exemple les trigrammes sont de la forme &lt;première lettre du prénom&gt;&lt;deux premières lettres du nom&gt;, et les dates sont aléatoires.
