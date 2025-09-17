# Pharmacy Management 

**A pharmacy management system implemented in C++ using tree data structures (BST + Trie).**
+ [Project Assignment File](https://github.com/AshkanRN/ds-project2-gu/releases/download/dl/ds_project2-assignment.pdf)
---

## Overview

This project is an interactive console application that models a pharmacy inventory system. It demonstrates core data-structure concepts from the course assignment: storing and managing drugs using binary search trees and providing name autocompletion with a Trie.

Key goals: practice tree-based storage, learn how to keep multiple trees in sync (main drug tree + category tree) and implement a prefix tree for fast name suggestions.

---

## Features

- Add / Delete drugs (each drug has `ID`, `Name`, `Price`, `Inventory`, `Category`).
- Main drug storage: **BST ordered by numeric `ID`** (unique ID required for each drug).
- Category index: **BST of categories**, where each category node stores a separate BST of drugs belonging to that category.
- Autocomplete / suggestions for drug names using a **Trie** (prefix tree).
- Search drugs by:
    - ID (fast lookup in main BST)
    - Exact name (search across tree)
    - Price range (in-order traversal + range check)
- Display all drugs (in-order traversal — sorted by ID).
- Display category-wise lists (in-order traversal of categories; each category shows its drugs).
- Track and show statistics: total number of drug entries, total inventory count, cheapest & most expensive drugs (maintained during insert/delete), and tree heights/balance check.

---

## Data structures & design

- **Main BST (drugs)**: stores every `drugs` node keyed by `ID`. Used for primary lookup, deletion, and global operations (min/max price search, inventory totals).
- **Category BST**: each node contains a `categoryName` and a root pointer to another BST of `drugs` (category-specific).
- **Trie**: supports autocomplete suggestions for drug names entered by the user. Stores lowercase names and space as an allowed character.

---

## Menu (what each option does)

- `0` Exit (frees allocated memory)
- `1` Insert — add a new drug (ID must be unique)
- `2` Delete — delete a drug by ID (automatically removes it from category tree and removes category node if empty)
- `3` Search — sub-menu: by ID, by name (with autocomplete fallback), or by price range
- `4` Display — print all drugs (in-order by ID)
- `5` Check Inventory — show counts (#drugs, total inventory, #categories)
- `6` Display cheapest and most expensive drugs
- `7` Heights & balance checks for both main and category trees
- `8` Auto Complete — provide suggestions using Trie
- `9` Display Category — list products grouped by category

---


