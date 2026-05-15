import { createRouter, createWebHistory } from 'vue-router'
import { useAuthStore } from "@/stores/auth";
import { useRoleStore } from "@/stores/role";
import HomeView from "@/views/HomeView.vue";
import RolesView from "@/views/RolesView.vue";
import TextsView from "@/views/TextsView.vue";
import StatsView from "@/views/StatsView.vue";


const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  scrollBehavior() {
  return { top: 0, left: 0 };
  },
  routes: [
    {
      path:'/',
      name: 'home',
      component: HomeView,
    },
    {
      path: "/roles",
      name: "roles",
      // Lazy loading , cela évite de tout charger au chargemet de la page en haut dans l'import et de charger le composant que quand on en as besoin 
      component: () => import("@/views/RolesView.vue"),
      meta: { requiresAuth:true }

    },
    {
    path: "/texts",
    name: "texts",
    component: () => import("@/views/TextsView.vue"),
    meta: { requiresAuth: true },
    children: [                                    
      {
        path: "repartition", // si on fait : /texts/repartition
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

  // Route séparée pour l'attribution (répartiteur seulement)
  {
    path: "/texts/repartition/text/:id",
    name: "text-attribution",
    component: () => import("@/views/TextAttributionView.vue"),
    meta: { requiresAuth: true, requiresRepartiteur: true }, // On dois absolument être connecté et être un répartiteur pour accéder à cette page
  },
    {
      path: "/stats",
      name: "stats",
      component: StatsView,
      meta: { requiresAuth:true}
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
})

router.beforeEach((to) => {
  const authStore = useAuthStore();
  const roleStore = useRoleStore(); 

  // Si je demande une page qui nécessite une connexion et que je ne suis pas connecté alors je  redirige vers l'accueil
  if (to.meta.requiresAuth && !authStore.isLoggedIn) {
    return { name: "home" };
  }

  // Si on veut se connecter a une page qui nécessite d'être répartiteur et que je ne le suis pas alors je redirige vers la page qui correspond à mon rôle
  if (to.meta.requiresRepartiteur && !roleStore.isRepartiteur) {
    if (roleStore.isTraducteur) return { name: "texts-traduction" };
    if (roleStore.isEditeur) return { name: "texts-edition" };
  }

  // Si on veut accéder à la page des textes sans préciser de sous-route alors je redirige vers la page qui correspond à mon rôle
  if (to.name === "texts") {
    if (roleStore.isRepartiteur) return { name: "texts-repartition" };
    if (roleStore.isTraducteur) return { name: "texts-traduction" };
    if (roleStore.isEditeur) return { name: "texts-edition" };
  }

   // Si on veut accéder à une page de texte qui ne correspond pas à notre rôle alors je redirige vers la page qui correspond à mon rôle
   
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
    // --- 5. Redirection automatique de /stats vers la bonne sous-route ---
  if (to.name === "stats") {
    if (roleStore.isRepartiteur) return { name: "stats-global" };
    if (roleStore.isTraducteur)
      return { name: "stats-translator", params: { id: roleStore.identityId } };
    if (roleStore.isEditeur)
      return { name: "stats-editor", params: { id: roleStore.identityId } };
  }
  // On empêche un traducteur ou un éditeur d'accéder aux stats d'un autre utilisateur en modifiant l'url
  if (
    to.name === "stats-translator" &&
    roleStore.isTraducteur &&
    to.params.id !== roleStore.identityId // Si l'id dans l'url ne correspond pas à l'id du traducteur connecté 
  ) { // alors on le redirige vers sa propre page de stats
    return { name: "stats-translator", params: { id: roleStore.identityId } };
  }
  if (
    to.name === "stats-editor" &&
    roleStore.isEditeur &&
    to.params.id !== roleStore.identityId
  ) {
    return { name: "stats-editor", params: { id: roleStore.identityId } };
  }
  // si la personne n'est pas un répartiteur et qu'elle essaie d'accéder à la page globale des stats alors je la redirige vers la page de stats qui correspond à son rôle
  if (to.name === "stats-global" && !roleStore.isRepartiteur) {
    if (roleStore.isTraducteur)
      return { name: "stats-translator", params: { id: roleStore.identityId } };
    if (roleStore.isEditeur)
      return { name: "stats-editor", params: { id: roleStore.identityId } };
  }
});

export default router
