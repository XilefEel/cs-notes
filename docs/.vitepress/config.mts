import { defineConfig } from "vitepress";

export default defineConfig({
  title: "Data Structures",
  description: "Notes for Data Structures in C and Rust",
  base: "/cs-notes/",
  themeConfig: {
    nav: [
      { text: "Home", link: "/" },
      { text: "Get Started", link: "/get-started" },
      { text: "Linked List", link: "/linked-list/intro" },
      { text: "Stack", link: "/stack/intro" },
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
                text: "Nodes in Doubly Linked List",
                link: "/linked-list/doubly/node",
              },
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
          {
            text: "Common Problems and Solutions",
            items: [
              {
                text: "Reversing a Linked List",
                link: "/linked-list/problems/reverse",
              },
              {
                text: "Detecting a Cycle",
                link: "/linked-list/problems/detect-cycle",
              },
              {
                text: "Merging Two Sorted Linked Lists",
                link: "/linked-list/problems/merge-sorted",
              },
              {
                text: "Removing the Nth Node From the End",
                link: "/linked-list/problems/remove-nth-node",
              },
              {
                text: "Check if Palindrome",
                link: "/linked-list/problems/palindrome",
              },
            ],
          },
        ],
      },
      {
        text: "Stack",
        items: [
          { text: "What is a Stack?", link: "/stack/intro" },
          { text: "Creating a Stack", link: "/stack/create" },
          { text: "Pushing an Element", link: "/stack/push" },
          { text: "Popping an Element", link: "/stack/pop" },
          {
            text: "Peeking and Checking if Empty",
            link: "/stack/peek-isempty",
          },
          {
            text: "Common Problems and Solutions",
            items: [
              {
                text: "Valid Parentheses",
                link: "/stack/problems/valid-parentheses",
              },
            ],
          },
        ],
      },
    ],

    socialLinks: [{ icon: "github", link: "https://github.com/XilefEel" }],

    search: {
      provider: "local",
    },

    editLink: {
      pattern: "https://github.com/XilefEel/cs-notes/tree/main/docs/:path",
      text: "Edit this page on GitHub",
    },

    footer: {
      message: "Personal study notes for learning data structures",
      copyright: "Made by Felix Lee © 2026",
    },
  },

  markdown: {
    theme: {
      light: "catppuccin-latte",
      dark: "one-dark-pro",
    },
  },
});
