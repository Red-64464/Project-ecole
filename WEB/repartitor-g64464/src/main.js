import "./assets/main.css";

// Importation des composants et des vues
import { createApp } from "vue";
import { createPinia } from "pinia";

import App from "./App.vue";
import router from "./router";
import { useAuthStore } from "./stores/auth";



const app = createApp(App);
const pinia = createPinia(); // Création de l'instance Pinia

app.use(pinia); // Utilisation de Pinia pour la gestion de l'état

const authStore = useAuthStore(); // On accède au store d'authentification pour initialiser la session de l'utilisateur
await authStore.init(); // on mets await pour que l'application attende la réponse de Supabase pour  éviter a l'applicaton de s'afficher avant que le store soit prêt


app.use(router);
app.mount("#app"); 