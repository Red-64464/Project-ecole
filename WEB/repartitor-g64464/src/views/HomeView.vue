<script setup>
/**
 * Page d'accueil de l'application Repartitor.
 *
 * Cette vue affiche un message simple de bienvenue pour presenter
 * la page principale du site.
 *
 * @component
 */

import { useAuthStore } from "@/stores/auth"; // On importe le store d'auth pour savoir si l'utilisateur est connecté

const authStore = useAuthStore(); // On accède au store : authStore.isLoggedIn = true/false
</script>

<template>
<div class="home-view"> 
   <div class="intro-card">
      <p>
        <strong>Repartitor</strong> est un outil de gestion des textes pour un
        service de traduction. Il permet de répartir les textes entre les
        traducteurs et les éditeurs, en respectant les délais fixés par les
        clients.
      </p>
    </div>
   <div v-if="!authStore.isLoggedIn" class="info-card">
      <h3 class="h3-con">Connexion requise</h3>
      <p>
        Connectez-vous avec votre compte Google pour accéder aux fonctionnalités
        de gestion des textes, des rôles et des statistiques.
      </p>
    </div>
    
     <div class="dev-notes">
      <h3>Notes du développeur</h3>
      <ul>
        <li>
          <strong>Technologies :</strong> VueJS 3, Vue-Router, Pinia, Supabase
        </li>
        <li><strong>Authentification :</strong> Google OAuth via Supabase</li>
        <li><strong>Base de données :</strong> PostgreSQL (Supabase)</li>
        <li>
          <strong>Fonctionnalités :</strong>
          <ul>
            <li>3 rôles : Répartiteur, Traducteur, Éditeur</li>
            <li>Gestion et attribution des textes</li>
            <li>Tableaux avec tri et filtrage</li>
            <li>Statistiques de productivité</li>
          </ul>
        </li>
      </ul>
    </div>
     </div>


</template>

<style scoped>
.h3-con{
  font-weight: bold;
  color: red;
}

.home-view {
  max-width: 800px;
  margin: 0 auto;
}

.intro-card,
.info-card,
.dev-notes {
  background: white;
  border-radius: 8px;
  padding: 1.5rem;
  margin-bottom: 1.5rem;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.3);
}

.info-card {
  border-left: 4px solid #e8a838;
}

.dev-notes h3 {
  margin-top: 0;
}

.dev-notes ul {
  line-height: 1.8;
}
</style>
