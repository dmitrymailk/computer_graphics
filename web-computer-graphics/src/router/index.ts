import { createRouter, createWebHistory } from "vue-router";
import HomeView from "../views/HomeView.vue";
import Lab1 from "../components/Lab1/Lab1.vue";
const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: "/",
      name: "home",
      component: HomeView,
    },
    {
      path: "/lab_1",
      component: Lab1,
    },
  ],
});

export default router;
