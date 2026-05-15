-- Il faut savoir que on crée des vues car Supabase utilise une API appelé PostgREST 
-- qui ne supporte pas les fonctions d'agrégation, et qui ne supporte pas les jointures complexes.
-- Donc on crée des vues pour pouvoir utiliser les fonctions d'agrégation et les jointures

-- View to be able to use the GROUP BY keyword.-
-- On crée une vue sur la table translation qui permet de faire des agrégations sur le nombre de mots traduits par chaque traducteur, le nombre de jours de travail occupés par chaque traducteur, et le nombre de textes en cours de traduction pour chaque traducteur.
-- security_invoker = true permet de faire en sorte que les permissions de la vue soient celles de l'utilisateur qui l'appelle, et non pas celles de l'utilisateur qui a créé la vue. Cela permets que si un utilisateur n'as pas les droits de voir quelque cose il ne les verras pas via la vue
-- elle sert a savoir pour chaque traducteur combien de mots il a en cours de traduction, combien de jours de travail il a occupé, et combien de textes il a en cours de traduction. Cela permet de répartir les traductions de manière équitable entre les traducteurs en fonction de leur charge de travail actuelle.
CREATE VIEW public.translator_workload WITH (security_invoker = true) AS
  SELECT translator, SUM(wordcount) AS words, CAST(SUM(wordcount) AS float)/expectedthroughput AS busydays, COUNT(*) AS texts
    FROM translator
      LEFT JOIN translation ON translator.id = translation.translator
      JOIN text ON text.id =  translation.text
    WHERE finished IS NULL
    GROUP BY translator,expectedthroughput;

-- Le soucis c'est que dans la table text on a une colonne precedent qui est une clé étrangère vers la même table, et que Supabase ne supporte pas les fonctions d'agrégation sur les clés étrangères. Donc on crée une fonction qui permet de faire le lien entre la table text et elle même via la colonne precedent
CREATE OR REPLACE FUNCTION prec(public.text) returns setof public.text ROWS 1 AS $$
  SELECT * FROM text WHERE id = $1.precedent
$$ stable language sql;
