-- Tables

CREATE TABLE public.text (
  id bigserial NOT NULL,
  cote varchar(30),
  title varchar(100) NOT NULL,
  received timestamptz DEFAULT CURRENT_TIMESTAMP NOT NULL,
  deadline timestamptz NOT NULL,
  wordcount integer NOT NULL,
  precedent bigint NULL,
  CONSTRAINT pk_text PRIMARY KEY (id),
  CONSTRAINT u_cote UNIQUE (cote),
  CONSTRAINT fk_precedent FOREIGN KEY (precedent) REFERENCES text(id) ON UPDATE CASCADE ON DELETE SET NULL
) TABLESPACE pg_default;

CREATE TABLE public.translator (
  id char(3) NOT NULL,
  firstname varchar(50) NOT NULL,
  lastname varchar(50) NOT NULL,
  expectedthroughput smallint DEFAULT 1000 NOT NULL,
  CONSTRAINT pk_translator PRIMARY KEY (id)
) TABLESPACE pg_default;

CREATE TABLE public.translation (
  text bigint NOT NULL,
  translator char(3) NOT NULL,
  attributed timestamptz DEFAULT CURRENT_TIMESTAMP NOT NULL,
  deadline timestamptz NOT NULL,
  finished timestamptz NULL,
  CONSTRAINT pk_translation PRIMARY KEY (text,translator),
  CONSTRAINT fk_text FOREIGN KEY (text) REFERENCES text(id) ON UPDATE CASCADE ON DELETE CASCADE,
  CONSTRAINT fk_translator FOREIGN KEY (translator) REFERENCES translator(id) ON UPDATE CASCADE ON DELETE CASCADE
) TABLESPACE pg_default;

CREATE TABLE public.editor (
  id char(3) NOT NULL,
  firstname varchar(50) NOT NULL,
  lastname varchar(50) NOT NULL,
  CONSTRAINT pk_editor PRIMARY KEY (id)
) TABLESPACE pg_default;

CREATE TABLE public.edition (
  text bigint NOT NULL,
  editor char(3) NOT NULL,
  attributed timestamptz DEFAULT CURRENT_TIMESTAMP NOT NULL,
  finished timestamptz NULL,
  translated_by char(3) NULL,
  CONSTRAINT pk_edition PRIMARY KEY (text,editor),
  CONSTRAINT fk_text FOREIGN KEY (text) REFERENCES text(id) ON UPDATE CASCADE ON DELETE CASCADE,
  CONSTRAINT fk_editor FOREIGN KEY (editor) REFERENCES editor(id) ON UPDATE CASCADE ON DELETE CASCADE,
  CONSTRAINT fk_translation FOREIGN KEY (text,translated_by) REFERENCES translation(text,translator) ON UPDATE CASCADE ON DELETE CASCADE
) TABLESPACE pg_default;

-- Policies , on utilise des policies pour restreindre l'accès aux données en fonction de l'authentification de l'utilisateur.

-- On active d'abord la sécurité au niveau des lignes pour chaque table, ce qui permet de définir des règles d'accès spécifiques pour les utilisateurs authentifiés.
ALTER TABLE public.text ENABLE row level security;
ALTER TABLE public.translator ENABLE row level security;
ALTER TABLE public.translation ENABLE row level security;
ALTER TABLE public.editor ENABLE row level security;
ALTER TABLE public.edition ENABLE row level security;

-- Ensuite, on crée des policies pour chaque table qui permettent aux utilisateurs authentifiés d'effectuer des opérations de création, lecture, mise à jour et suppression (CRUD) sur les données. La condition USING (true) signifie que tous les utilisateurs authentifiés ont accès à ces opérations sans restriction supplémentaire.
CREATE POLICY "Enable text CRUD for authenticated users" ON public.text TO authenticated USING (true);
CREATE POLICY "Enable translator CRUD for authenticated users" ON public.translator TO authenticated USING (true);
CREATE POLICY "Enable translation CRUD for authenticated users" ON public.translation TO authenticated USING (true);
CREATE POLICY "Enable editor CRUD for authenticated users" ON public.editor TO authenticated USING (true);
CREATE POLICY "Enable edition CRUD for authenticated users" ON public.edition TO authenticated USING (true);

-- Enable aggregate functions
-- Ce code permet d'activer les fonctions d'agrégation pour les utilisateurs authentifiés 
ALTER ROLE authenticator SET pgrst.db_aggregates_enabled = 'true';
NOTIFY pgrst, 'reload config';
