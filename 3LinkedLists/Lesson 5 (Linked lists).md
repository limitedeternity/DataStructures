 ## Linked lists

### Where are linked lists used?

* Used in many List, Queue and Stack implementations
* Great for creating circular lists
* Can easily model real world objects such as trains
* Used in separate chaining, which is present in certain Hashtable implementations to deal with hashing collisions
* Often used in the implementation of adjacency lists for graphs

### Terminology

**Head**: The first node in a linked list

**Tail**: The last node in a linked list

**Pointer**: Reference to another node

**Node**: An object containing data and pointer(s)

### Singly vs Doubly Linked lists

> Singly linked lists only hold a reference to the next node. In the implementation you always maintain a reference to the **head** of the linked list for quick additions/removals and – optionally – a reference to the **tail** for maintaining a circular link.

> In a doubly linked list each node holds a reference to the next and previous nodes. In the implementation you always maintain a reference to the **head** and the **tail** of the doubly linked list to do quick additions/removals from *both ends* of your list.

### Pros and cons

|      |                Singly Linked                 |       Doubly Linked        |
| :--: | :------------------------------------------: | :------------------------: |
| Pros | Uses less memory<br />Simpler implementation | Can be traversed backwards |
| Cons |    Cannot easily access previous elements    |      Takes 2x memory       |

### Complexity

|                  | Singly Linked | Doubly Linked |
| :--------------: | :-----------: | :-----------: |
|      Search      |     O(n)      |     O(n)      |
|   Insert head    |     O(1)      |     O(1)      |
|   Insert tail    |     O(1)      |     O(1)      |
|   Remove head    |     O(1)      |     O(1)      |
|   Remove tail    |     O(n)      |     O(1)      |
| Remove in middle |     O(n)      |     O(n)      |

