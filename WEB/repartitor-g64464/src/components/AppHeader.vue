<script setup>
/**
 * En-tete principal de l'application Repartitor.
 *
 * Ce composant correspond a la version minimale demandee dans les consignes :
 * il affiche le titre du projet, le menu de navigation et un bouton
 * "Connexion avec Google" qui ne fait encore aucune action.
 *
 * @component
 */

  import { useAuthStore } from "@/stores/auth";
  import { useRouter } from "vue-router";
  import { useRoleStore } from "@/stores/role";

  const roleStore = useRoleStore();
  const authStore = useAuthStore();
  const router = useRouter();

  async function handleLogout() {
    await authStore.logout();
    roleStore.reset();
    router.push({ name: "home" });
  }

</script>
<template>
  <header class="app-header">
    <h1 class="site-title">REPARTITOR</h1>

    <div class="header-bottom">
      <nav  v-if="authStore.isLoggedIn" class="main-nav">
        <RouterLink to="/">Accueil</RouterLink>
        <RouterLink to="/roles">Changer de role</RouterLink>
        <RouterLink to="/texts">Gerer les textes</RouterLink>
        <RouterLink to="/stats">Statistiques</RouterLink>
      </nav>
      
      <div class="auth-area">
        <template v-if="authStore.isLoggedIn">
      

        <div class="user-info">
          <span class="span-email">{{ authStore.user.user_metadata.full_name }}</span>
          <!-- On utilise user_metadata car c'est la que Supabase stocke le nom complet de l'utilisateur et d'autres info  -->
          <span class="span-email">{{ authStore.userEmail }}</span>
          <p>--------------</p>
          <div class="role">
            <span>Rôle actuel : {{ roleStore.role }}</span>
            <p v-if="roleStore.identity">
               {{ roleStore.identity.firstname }} {{ roleStore.identity.lastname }}
            </p>
          </div>

        </div>
        <button  class="login-button" @click="handleLogout">Deconnexion</button>
        </template>

        <button v-else class="login-button" @click="authStore.loginWithGoogle()">
          Connexion avec Google
        </button>
      </div>
    </div>
  
  </header>
</template>

<style scoped>
.app-header {
  background-color: #2b2d78;
  color: #ffa500;
  padding: 1.5rem;
}

.span-email{
  margin-right : 1rem;
}

.user-info {
  display: flex;
  flex-direction: column;
  margin-right: 1rem;
  gap: 5px;
  margin-bottom: 20px;
}

.role{
  font-weight: bold;
}

.site-title {
  margin: 0 0 1rem;
  color: #ffa500;
  font-size: 3rem;
  text-align: center;
}

.header-bottom {
  display: flex;
  align-items: flex-end;
  justify-content: space-between;
  gap: 1rem;
}

.main-nav {
  display: flex;
  gap: 1rem;
  flex-wrap: wrap;
}

.main-nav a {
  color: #ffa500;
  text-decoration: none;
  font-weight: bold;
}

.main-nav a.router-link-active {
  text-decoration: underline;
}

.login-button {
  background-color: #ffa500;
  border: 0;
  border-radius: 4px;
  padding: 0.6rem 1rem;
  cursor: pointer;
}

@media (max-width: 700px) {
  .header-bottom {
    align-items: flex-start;
    flex-direction: column;
  }
}
</style>
