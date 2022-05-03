import { createRouter, createWebHistory } from "vue-router";
import HomeView from "../views/HomeView.vue";
import Lab1 from "../components/Lab1/Lab1.vue";
import Lab2 from "../components/Lab2/Lab2.vue";
import Lab3 from "../components/Lab3/Lab3.vue";
import Lab4 from "../components/Lab4/Lab4.vue";
import Lab5 from "../components/Lab5/Lab5.vue";
import Lab6 from "../components/Lab6/Lab6.vue";

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
    {
      path: "/lab_2",
      component: Lab2,
    },
    {
      path: "/lab_3",
      component: Lab3,
    },
    {
      path: "/lab_4",
      component: Lab4,
    },
    {
      path: "/lab_5",
      component: Lab5,
    },
    {
      path: "/lab_6",
      component: Lab6,
    },
  ],
});

export default router;
