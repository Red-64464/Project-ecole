<script setup>
import AppFooter from "@/components/AppFooter.vue";
import AppHeader from "@/components/AppHeader.vue";
</script>

<template>
  <AppHeader />

  <main class="app-main">
    <!-- Cette partie affichera le contenu des différentes composants  en fonction de la route via le routeur -->
    <RouterView />
  </main>

  <AppFooter />
</template>

<style scoped>
.app-main {
  min-height: 70vh;
  padding: 2rem;
}
</style>