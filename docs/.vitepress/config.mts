import { defineConfig } from "vitepress";

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "Data Structures in C and Rust",
  description: "Notes for Data Structures",
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: "Home", link: "/" },
      { text: "Linked List", link: "/linked-list" },
    ],

    sidebar: [
      {
        text: "Linked List",
        items: [
          { text: "What is a Linked List?", link: "/linked-list/index" },
          { text: "What is a Node?", link: "/linked-list/node" },
          { text: "Creating a Linked List", link: "/linked-list/create" },
          {
            text: "Accessing a Node",
            link: "/linked-list/access",
          },
          {
            text: "Singly Linked List",
            items: [
              {
                text: "Inserting a Node",
                link: "/linked-list/singly/insert",
              },
              {
                text: "Deleting a Node",
                link: "/linked-list/singly/delete",
              },
              {
                text: "Traversing a Linked List",
                link: "/linked-list/singly/traverse",
              },
            ],
          },
          {
            text: "Doubly Linked List",
            items: [
              {
                text: "Inserting a Node",
                link: "/linked-list/doubly/insert",
              },
              {
                text: "Deleting a Node",
                link: "/linked-list/doubly/delete",
              },
              {
                text: "Traversing a Linked List",
                link: "/linked-list/doubly/traverse",
              },
            ],
          },
          { text: "C vs Rust Summary", link: "/linked-list/summary" },
        ],
      },
    ],

    socialLinks: [
      { icon: "github", link: "https://github.com/vuejs/vitepress" },
    ],
  },
});
