import { defineConfig } from "vitepress";

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "Data Structures",
  description: "Notes for Data Structures in C and Rust",
  base: "/cs-notes/",
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: "Home", link: "/" },
      { text: "Get Started", link: "/get-started" },
      { text: "Linked List", link: "/linked-list/intro" },
    ],

    sidebar: [
      {
        text: "Introduction",
        items: [{ text: "Get Started", link: "/get-started" }],
      },
      {
        text: "Linked List",
        items: [
          { text: "What is a Linked List?", link: "/linked-list/intro" },
          { text: "What is a Node?", link: "/linked-list/node" },
          { text: "Creating a Linked List", link: "/linked-list/create" },
          {
            text: "Accessing and Traversing a Linked List",
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
                text: "Bonus: Tail Pointer",
                link: "/linked-list/singly/tail",
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
            ],
          },
          { text: "C vs Rust Summary", link: "/linked-list/summary" },
        ],
      },
    ],

    socialLinks: [{ icon: "github", link: "https://github.com/XilefEel" }],
  },
});
