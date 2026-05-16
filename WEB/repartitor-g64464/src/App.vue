<script setup>
import AppFooter from "@/components/AppFooter.vue";
import AppHeader from "@/components/AppHeader.vue";
import NotificationToast from "@/components/NotificationToast.vue";
</script>

<template>
  <AppHeader />

  <main class="app-main">
    <!-- Le composant affiché ici dépend de la route courante. -->
    <RouterView />
  </main>

  <AppFooter />

  <!-- Toasts globaux (succès / erreur) affichés au-dessus de tout le reste. -->
  <NotificationToast />
</template>

<style scoped>
.app-main {
  min-height: 70vh;
  padding: 2rem;
}
</style>