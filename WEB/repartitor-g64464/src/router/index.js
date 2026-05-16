/**
 * Routeur principal de Repartitor.
 *
 * Définit l'arbre des routes (avec sous-routes pour /texts et /stats),
 * puis applique un garde global qui :
 *   - bloque les pages protégées si l'utilisateur n'est pas connecté ;
 *   - redirige /texts et /stats vers la bonne sous-route selon le rôle ;
 *   - empêche un traducteur / éditeur de voir les stats d'un autre.
 */

import { createRouter, createWebHistory } from "vue-router";
import { useAuthStore } from "@/stores/auth";
import { useRoleStore } from "@/stores/role";
import HomeView from "@/views/HomeView.vue";

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  scrollBehavior() {
    return { top: 0, left: 0 };
  },
  routes: [
    {
      path: "/",
      name: "home",
      component: HomeView,
    },
    {
      path: "/roles",
      name: "roles",
      // Chargement paresseux : le bundle de RolesView n'est téléchargé
      // que lorsque l'utilisateur ouvre la page.
      component: () => import("@/views/RolesView.vue"),
      meta: { requiresAuth: true },
    },
    {
      path: "/texts",
      name: "texts",
      component: () => import("@/views/TextsView.vue"),
      meta: { requiresAuth: true },
      children: [
        {
          path: "repartition",
          name: "texts-repartition",
          component: () => import("@/views/RepartitionView.vue"),
        },
        {
          path: "traduction",
          name: "texts-traduction",
          component: () => import("@/views/TraductionView.vue"),
        },
        {
          path: "edition",
          name: "texts-edition",
          component: () => import("@/views/EditionTextView.vue"),
        },
      ],
    },
    // Route séparée pour l'attribution (répartiteur uniquement).
    {
      path: "/texts/repartition/text/:id",
      name: "text-attribution",
      component: () => import("@/views/TextAttributionView.vue"),
      meta: { requiresAuth: true, requiresRepartiteur: true },
    },
    {
      path: "/stats",
      name: "stats",
      component: () => import("@/views/StatsView.vue"),
      meta: { requiresAuth: true },
      children: [
        {
          path: "global",
          name: "stats-global",
          component: () => import("@/views/StatsGlobalView.vue"),
        },
        {
          path: "translator/:id",
          name: "stats-translator",
          component: () => import("@/views/StatsTranslatorView.vue"),
        },
        {
          path: "editor/:id",
          name: "stats-editor",
          component: () => import("@/views/StatsEditorView.vue"),
        },
      ],
    },
  ],
});

router.beforeEach((to) => {
  const authStore = useAuthStore();
  const roleStore = useRoleStore();

  // Page protégée + utilisateur déconnecté → accueil.
  if (to.meta.requiresAuth && !authStore.isLoggedIn) {
    return { name: "home" };
  }

  // Page réservée au répartiteur consultée par un autre rôle :
  // on redirige vers la vue adaptée au rôle courant.
  if (to.meta.requiresRepartiteur && !roleStore.isRepartiteur) {
    if (roleStore.isTraducteur) return { name: "texts-traduction" };
    if (roleStore.isEditeur) return { name: "texts-edition" };
  }

  // /texts sans sous-route → on choisit la bonne sous-route.
  if (to.name === "texts") {
    if (roleStore.isRepartiteur) return { name: "texts-repartition" };
    if (roleStore.isTraducteur) return { name: "texts-traduction" };
    if (roleStore.isEditeur) return { name: "texts-edition" };
  }

  // Accès direct à une sous-route /texts/* qui ne correspond pas au rôle.
  if (to.name === "texts-repartition" && !roleStore.isRepartiteur) {
    if (roleStore.isTraducteur) return { name: "texts-traduction" };
    if (roleStore.isEditeur) return { name: "texts-edition" };
  }
  if (to.name === "texts-traduction" && !roleStore.isTraducteur) {
    if (roleStore.isRepartiteur) return { name: "texts-repartition" };
    if (roleStore.isEditeur) return { name: "texts-edition" };
  }
  if (to.name === "texts-edition" && !roleStore.isEditeur) {
    if (roleStore.isRepartiteur) return { name: "texts-repartition" };
    if (roleStore.isTraducteur) return { name: "texts-traduction" };
  }

  // /stats sans sous-route → redirection adaptée au rôle.
  if (to.name === "stats") {
    if (roleStore.isRepartiteur) return { name: "stats-global" };
    if (roleStore.isTraducteur)
      return { name: "stats-translator", params: { id: roleStore.identityId } };
    if (roleStore.isEditeur)
      return { name: "stats-editor", params: { id: roleStore.identityId } };
  }

  // Un traducteur ne peut consulter que ses propres stats : si l'URL
  // contient un autre trigramme, on le ramène sur les siennes.
  if (
    to.name === "stats-translator" &&
    roleStore.isTraducteur &&
    to.params.id !== roleStore.identityId
  ) {
    return { name: "stats-translator", params: { id: roleStore.identityId } };
  }
  if (
    to.name === "stats-editor" &&
    roleStore.isEditeur &&
    to.params.id !== roleStore.identityId
  ) {
    return { name: "stats-editor", params: { id: roleStore.identityId } };
  }

  // Seul le répartiteur peut voir les stats globales.
  if (to.name === "stats-global" && !roleStore.isRepartiteur) {
    if (roleStore.isTraducteur)
      return { name: "stats-translator", params: { id: roleStore.identityId } };
    if (roleStore.isEditeur)
      return { name: "stats-editor", params: { id: roleStore.identityId } };
  }
});

export default router;
