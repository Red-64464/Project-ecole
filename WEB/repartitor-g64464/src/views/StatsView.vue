<script setup>
/**
 * Page parente des statistiques.
 *
 * La page concrète (globale, traducteur, éditeur) est rendue dans
 * <RouterView /> via les sous-routes définies dans src/router/index.js.
 */
</script>

<template>
  <section class="stats-page">
    <h2 class="stats-title">Statistiques</h2>
    <RouterView />
  </section>
</template>

<style scoped>
.stats-page {
  min-height: calc(100vh - 194px);
  margin: -2rem;
  padding: 0.65rem 1.5rem 2rem;
  background-color: #eaf6ff;
  color: #000;
}

.stats-title {
  margin: 0 0 1.5rem;
  font-size: clamp(2rem, 4vw, 3rem);
  font-weight: 800;
  text-align: center;
  text-transform: uppercase;
  letter-spacing: 0.05em;
}
</style>
