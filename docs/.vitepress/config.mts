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
      { text: "Queue", link: "/queue/intro" },
      { text: "Binary Tree", link: "/binary-tree/intro" },
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
              {
                text: "Priority Queue",
                link: "/linked-list/problems/priority-queue",
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
              {
                text: "Min Stack",
                link: "/stack/problems/min-stack",
              },
              {
                text: "Implement Queue using Stacks",
                link: "/stack/problems/queue-using-stacks",
              },
            ],
          },
        ],
      },
      {
        text: "Queue",
        items: [
          { text: "What is a Queue?", link: "/queue/intro" },
          { text: "Creating a Queue", link: "/queue/create" },
          { text: "Enqueuing an Element", link: "/queue/enqueue" },
          { text: "Dequeuing an Element", link: "/queue/dequeue" },
          {
            text: "Peeking and Checking if Empty",
            link: "/queue/peek-isempty",
          },
        ],
      },
      {
        text: "Binary Tree",
        items: [
          { text: "What is a Binary Tree?", link: "/binary-tree/intro" },
          { text: "Types of Binary Trees", link: "/binary-tree/types" },
          { text: "What is a BST?", link: "/binary-tree/bst" },
          { text: "Creating a BST", link: "/binary-tree/create" },
          {
            text: "Operations on a BST",
            items: [
              {
                text: "Inserting a Node",
                link: "/binary-tree/operations/insert",
              },
              {
                text: "Searching for a Node",
                link: "/binary-tree/operations/search",
              },
              {
                text: "Deleting a Node",
                link: "/binary-tree/operations/delete",
              },
            ],
          },
          {
            text: "Traversing a Binary Tree",
            items: [
              {
                text: "Preorder Traversal",
                link: "/binary-tree/traversal/preorder",
              },
              {
                text: "Inorder Traversal",
                link: "/binary-tree/traversal/inorder",
              },
              {
                text: "Postorder Traversal",
                link: "/binary-tree/traversal/postorder",
              },
              {
                text: "Level Order Traversal (BFS)",
                link: "/binary-tree/traversal/level-order",
              },
            ],
          },
          {
            text: "Common Problems and Solutions",
            items: [
              {
                text: "Finding the Height of a Binary Tree",
                link: "/binary-tree/problems/height",
              },
              {
                text: "Checking if a Tree is Balanced",
                link: "/binary-tree/problems/balanced",
              },
              {
                text: "Lowest Common Ancestor (LCA)",
                link: "/binary-tree/problems/lca",
              },
              {
                text: "Checking if a Tree is a Valid BST",
                link: "/binary-tree/problems/valid-bst",
              },
            ],
          },
          {
            text: "Heap",
            items: [
              { text: "What is a Heap?", link: "/heap/intro" },
              { text: "Types of Heaps", link: "/heap/types" },
              { text: "Heap as an Array", link: "/heap/array-representation" },
              { text: "Creating a Heap", link: "/heap/create" },
              { text: "Inserting an Element", link: "/heap/insert" },
              { text: "Deleting an Element", link: "/heap/delete" },
              {
                text: "Peek and Checking if Empty",
                link: "/heap/peek-isempty",
              },
              {
                text: "Common Problems and Solutions",
                items: [
                  {
                    text: "Kth Largest Element",
                    link: "/heap/problems/kth-largest",
                  },
                  {
                    text: "Merge K Sorted Lists",
                    link: "/heap/problems/merge-k-sorted",
                  },
                  {
                    text: "Top K Frequent Elements",
                    link: "/heap/problems/top-k-frequent",
                  },
                ],
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
