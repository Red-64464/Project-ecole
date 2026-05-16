-- Vues et fonctions SQL pour Supabase / PostgREST.
-- Supabase passe par PostgREST, qui ne supporte ni GROUP BY ni les
-- auto-jointures cote API. On contourne ces limitations avec des vues.
--
-- security_invoker = true : la vue herite des permissions de l'utilisateur
-- qui l'appelle (et pas du proprietaire de la vue).

------------------------------------------------------------
-- Charge de travail des traducteurs
------------------------------------------------------------
-- Pour chaque traducteur en activite : nombre de mots en cours,
-- nombre de jours de travail equivalent (mots / expectedthroughput)
-- et nombre de textes encore en cours.
CREATE OR REPLACE VIEW public.translator_workload
  WITH (security_invoker = true) AS
  SELECT translator,
         SUM(wordcount) AS words,
         CAST(SUM(wordcount) AS float) / expectedthroughput AS busydays,
         COUNT(*) AS texts
    FROM translator
    LEFT JOIN translation ON translator.id = translation.translator
    JOIN text ON text.id = translation.text
    WHERE finished IS NULL
    GROUP BY translator, expectedthroughput;

------------------------------------------------------------
-- Fonction prec() : remonter au texte precedent
------------------------------------------------------------
-- Comme `text.precedent` est une cle etrangere auto-referente,
-- PostgREST ne sait pas la suivre. Cette fonction donne l'equivalent.
CREATE OR REPLACE FUNCTION prec(public.text)
  RETURNS SETOF public.text
  ROWS 1
  AS $$
    SELECT * FROM text WHERE id = $1.precedent
  $$ STABLE LANGUAGE sql;

------------------------------------------------------------
-- Statistiques globales courantes (repartiteur)
------------------------------------------------------------
-- Une seule ligne qui regroupe toutes les metriques affichees
-- dans la page "Statistiques / Globales / Actuellement".
CREATE OR REPLACE VIEW public.stats_global_current
  WITH (security_invoker = true) AS
  SELECT
    -- Tous les textes encore vivants (deadline pas encore depassee
    -- ET edition non terminee).
    (SELECT COUNT(*)
       FROM text
       WHERE NOT EXISTS (
         SELECT 1 FROM edition
         WHERE edition.text = text.id AND edition.finished IS NOT NULL
       )) AS in_progress_texts,
    (SELECT COALESCE(SUM(wordcount), 0)
       FROM text
       WHERE NOT EXISTS (
         SELECT 1 FROM edition
         WHERE edition.text = text.id AND edition.finished IS NOT NULL
       )) AS in_progress_words,

    -- Textes pas encore attribues a un traducteur.
    (SELECT COUNT(*)
       FROM text
       WHERE NOT EXISTS (
         SELECT 1 FROM translation WHERE translation.text = text.id
       )) AS unassigned_translation_texts,
    (SELECT COALESCE(SUM(wordcount), 0)
       FROM text
       WHERE NOT EXISTS (
         SELECT 1 FROM translation WHERE translation.text = text.id
       )) AS unassigned_translation_words,

    -- Textes en cours de traduction (attribues, pas finis).
    (SELECT COUNT(DISTINCT text.id)
       FROM text
       JOIN translation ON translation.text = text.id
       WHERE translation.finished IS NULL) AS translating_texts,
    (SELECT COALESCE(SUM(t.wordcount), 0)
       FROM (
         SELECT DISTINCT text.id, text.wordcount
           FROM text
           JOIN translation ON translation.text = text.id
           WHERE translation.finished IS NULL
       ) AS t) AS translating_words,

    -- Textes deja traduits mais pas encore attribues a un editeur.
    (SELECT COUNT(*)
       FROM text
       WHERE EXISTS (
         SELECT 1 FROM translation
         WHERE translation.text = text.id AND translation.finished IS NOT NULL
       )
       AND NOT EXISTS (
         SELECT 1 FROM edition WHERE edition.text = text.id
       )) AS unassigned_edition_texts,
    (SELECT COALESCE(SUM(wordcount), 0)
       FROM text
       WHERE EXISTS (
         SELECT 1 FROM translation
         WHERE translation.text = text.id AND translation.finished IS NOT NULL
       )
       AND NOT EXISTS (
         SELECT 1 FROM edition WHERE edition.text = text.id
       )) AS unassigned_edition_words,

    -- Textes en cours d'edition.
    (SELECT COUNT(DISTINCT text.id)
       FROM text
       JOIN edition ON edition.text = text.id
       WHERE edition.finished IS NULL) AS editing_texts,
    (SELECT COALESCE(SUM(t.wordcount), 0)
       FROM (
         SELECT DISTINCT text.id, text.wordcount
           FROM text
           JOIN edition ON edition.text = text.id
           WHERE edition.finished IS NULL
       ) AS t) AS editing_words;

------------------------------------------------------------
-- Statistiques individuelles passees - traducteur
------------------------------------------------------------
-- Liste chaque attribution de traduction avec :
--   - le traducteur concerne
--   - la date d'attribution
--   - le nombre de mots du texte
--   - si la traduction est terminee
--   - si elle a ete rendue en retard
-- L'application filtre ensuite par traducteur et par intervalle de dates.
CREATE OR REPLACE VIEW public.translator_history
  WITH (security_invoker = true) AS
  SELECT translation.translator,
         translation.text,
         translation.attributed,
         translation.deadline,
         translation.finished,
         text.wordcount,
         (translation.finished IS NOT NULL) AS done,
         (translation.finished IS NOT NULL
          AND translation.finished > translation.deadline) AS late
    FROM translation
    JOIN text ON text.id = translation.text;

------------------------------------------------------------
-- Statistiques individuelles passees - editeur
------------------------------------------------------------
CREATE OR REPLACE VIEW public.editor_history
  WITH (security_invoker = true) AS
  SELECT edition.editor,
         edition.text,
         edition.attributed,
         edition.finished,
         text.deadline,
         text.wordcount,
         (edition.finished IS NOT NULL) AS done,
         (edition.finished IS NOT NULL
          AND edition.finished > text.deadline) AS late
    FROM edition
    JOIN text ON text.id = edition.text;
