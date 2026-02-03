# Social Network Simulator

## Introduction

This project is a command-line social network simulator that manages users, friendships, and posts. The core data structures (AVL Tree for posts and Graph for user connections) have been implemented from scratch using only standard C++ libraries for hashmaps.

---

## System Architecture

The social network manages users and their relationships using the following data structures:

* **Graph**: Represents the social net where each vertex is a user and edges represent bidirectional friendships. Implemented using adjacency lists with vectors.
* **AVL Tree**: Stores posts for each user sorted by creation time, allowing efficient insertion (O(log n)) and retrieval in reverse chronological order. Self-balancing ensures optimal performance.
* **HashMap**: Uses C++ `unordered_map` for O(1) average-time user lookup by username. All usernames are stored in lowercase for case-insensitive operations while preserving original case for display.

All data structures except HashMap have been created from scratch as per assignment requirements.

---

## Instructions to Run

### Linux/Mac:
```bash
chmod +x run.sh
./run.sh
```

This will compile the project and run the executable.

---

## Command Reference

The program provides a command-line interface for managing the social network.

* **ADD_USER `<username>`**
    * Creates a new user with no friends and no posts.
    * **Success Message**: "User `<username>` added."
    * **Error Messages**: 
        * "ERROR: User `<username>` already exists."
        * "ERROR: Missing username."

* **ADD_FRIEND `<username1>` `<username2>`**
    * Establishes a bidirectional friendship between two users.
    * **Success Message**: "Friend added."
    * **Error Messages**:
        * "ERROR: Cannot friend yourself."
        * "ERROR: User `<username>` does not exist."
        * "ERROR: Users `<username1>` and `<username2>` do not exist."
        * "ERROR: Friend already exists."
        * "ERROR: Missing username(s)."

* **LIST_FRIENDS `<username>`**
    * Lists all friends in alphabetical order.
    * **Error Messages**:
        * "ERROR: User `<username>` does not exist."
        * "ERROR: Missing username."

* **SUGGEST_FRIENDS `<username>` `<N>`**
    * Recommends up to N friends based on mutual connections, ranked by mutual friend count (descending) then alphabetically.
    * **Error Messages**:
        * "ERROR: User `<username>` does not exist."
        * "ERROR: No mutual friends."
        * "ERROR: Missing username or number."
        * "ERROR: Invalid number."

* **DEGREES_OF_SEPARATION `<username1>` `<username2>`**
    * Calculates shortest friendship path using BFS. Returns 0 for same user, -1 if no path exists.
    * **Error Messages**:
        * "ERROR: User `<username>` does not exist."
        * "ERROR: Users `<username1>` and `<username2>` do not exist."
        * "ERROR: Missing username(s)."

* **ADD_POST `<username>` `<content>`**
    * Adds a post for the user. Content is everything after the username (assumed that the quotation marks in the problem statement are only for representation).
    * **Success Message**: "Post `<content>` added for `<username>`."
    * **Error Messages**:
        * "ERROR: User `<username>` does not exist."
        * "ERROR: Post content cannot be empty."
        * "ERROR: Missing post content."
        * "ERROR: Missing username."

* **OUTPUT_POSTS `<username>` `<N>`**
    * Outputs N most recent posts in reverse chronological order. Use -1 for all posts.
    * **Error Messages**:
        * "ERROR: User `<username>` does not exist."
        * "ERROR: Missing username or number."
        * "ERROR: Invalid number."

* **EXIT**
    * Exits the program.
    * **Message**: "Exiting"

---

## Key Semantics and Assumptions

* **Case Insensitivity**: Usernames are case-insensitive for all operations but original case is preserved for display.
* **Bidirectional Friendships**: All friendships are mutual and stored in both users' friend lists.
* **Post Ordering**: Posts are stored in AVL tree by timestamp, retrieved in reverse chronological order (most recent first).
* **Error Handling**: Comprehensive validation for duplicate users, self-friending, missing parameters, and invalid inputs.
* **BFS for Path Finding**: Degrees of separation uses breadth-first search to find shortest path.
* **Friend Suggestions**: Based on mutual friends, excluding already-connected users and self.

---

## Time Complexities

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| ADD_USER | O(1) | HashMap insertion |
| ADD_FRIEND | O(F) | F = checking existing friends |
| LIST_FRIENDS | O(F log F) | Sorting friends alphabetically |
| SUGGEST_FRIENDS | O(F²) | Counting mutual friends |
| DEGREES_OF_SEPARATION | O(V + E) | BFS traversal |
| ADD_POST | O(log P) | AVL tree insertion |
| OUTPUT_POSTS | O(P) | Tree traversal |

*F = friends per user, V = total users, E = total friendships, P = posts per user*

---

## File Structure

* `socialnet.cpp`: Main source file containing all implementations
* `run.sh`: Shell script to compile and run the program.
