/* On crée un store pour gérer l'authentification de l'utilisateur.
 * Le store utilise le client Supabase pour se connecter et se déconnecter.
 * Il expose une fonction `login` pour se connecter, `logout` pour se déconnecter, et une variable `user` qui contient les infos de l'utilisateur connecté.
 */

import { defineStore } from "pinia"; 
import { ensureFreshSession, supabase } from "@/supabase"; 

export const useAuthStore = defineStore("auth", {

    state:()=>({
        session : null, // La session qui contiendra le token d'accès ,  la date d'expiration , etc...
        user : null, // Contiendra les infos de l'utilisateur connecté
        isReady : false, // Indique si le store est prêt (utile pour afficher un loader pendant la connexion)
    }),

    getters : {
        isLoggedIn : (state) => state.session !== null,
        userEmail: (state) => state.user ? state.user.email : "", 
    },

    actions : {
        async init(){
            const session = await ensureFreshSession();
            this.session = session;
            this.user = session ? session.user : null;
            this.isReady = true; // Le store est prêt après avoir récupéré la session

            // Un écouteurs Supabase , il se déclanche chaque fois que l'utilisateur se connecte ou déconnecte 
             supabase.auth.onAuthStateChange((_event, session) => {
            this.session = session;
            this.user = session ? session.user : null;
            });
        },
        async loginWithGoogle() {
            await supabase.auth.signInWithOAuth({
                provider: "google",
            });
        },

        async logout() {
            await supabase.auth.signOut();
            this.session = null;
            this.user = null;
        },

    }
});
