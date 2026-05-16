/**
 * Petit système de notifications "toast" partagé par toute l'application.
 *
 * Utilisation :
 *   import { useNotifications } from "@/composables/useNotifications";
 *   const { notify } = useNotifications();
 *   notify("Texte attribué", "success");
 *
 * Le composant <NotificationToast /> (monté dans App.vue) lit la liste
 * partagée et affiche chaque message pendant quelques secondes.
 */

import { ref } from "vue";

// État partagé (en dehors de la fonction pour être un singleton).
const notifications = ref([]);
let nextId = 1;

export function useNotifications() {
  /**
   * Ajoute une notification.
   * @param {string} message - Texte à afficher.
   * @param {"info"|"success"|"error"} [type="info"] - Style visuel.
   * @param {number} [duration=4000] - Durée d'affichage en millisecondes.
   */
  function notify(message, type = "info", duration = 4000) {
    const id = nextId++;
    notifications.value.push({ id, message, type });
    setTimeout(() => dismiss(id), duration);
  }

  /** Retire manuellement une notification. */
  function dismiss(id) {
    notifications.value = notifications.value.filter((n) => n.id !== id);
  }

  return { notifications, notify, dismiss };
}
