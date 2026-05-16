<!-- Affiche les notifications globales (succès, erreur, info) sous forme
     de toasts en bas à droite de l'écran. Monté une seule fois dans App.vue. -->
<script setup>
import { useNotifications } from "@/composables/useNotifications";

const { notifications, dismiss } = useNotifications();
</script>

<template>
  <div class="toast-container" aria-live="polite">
    <div
      v-for="n in notifications"
      :key="n.id"
      :class="['toast', `toast-${n.type}`]"
      role="status"
      @click="dismiss(n.id)"
    >
      <span class="toast-message">{{ n.message }}</span>
      <button class="toast-close" aria-label="Fermer">×</button>
    </div>
  </div>
</template>

<style scoped>
.toast-container {
  position: fixed;
  right: 1rem;
  bottom: 1rem;
  z-index: 1000;
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
  pointer-events: none;
}

.toast {
  display: flex;
  align-items: center;
  gap: 0.6rem;
  min-width: 14rem;
  max-width: 22rem;
  padding: 0.6rem 0.9rem;
  border-radius: 4px;
  color: #fff;
  font-size: 0.9rem;
  font-weight: 600;
  box-shadow: 0 4px 10px rgba(0, 0, 0, 0.25);
  pointer-events: auto;
  cursor: pointer;
}

.toast-success { background-color: #1f8a3b; }
.toast-error   { background-color: #b33; }
.toast-info    { background-color: #2b2d78; }

.toast-message { flex: 1; }
.toast-close {
  border: 0;
  background: transparent;
  color: inherit;
  font-size: 1.1rem;
  cursor: pointer;
}
</style>
