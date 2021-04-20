## Stacks

### What is a Stack?

> A stack is a one-ended linear data structure which models a real world stack by having two primary operations: **push** and **pop**.

### When and where is a Stack used?

* Used by undo mechanisms in text editors;
* Used in compiler lexical analysis for matching brackets and braces;
* Can be used to model a pile of books or plates;
* Used behind the scenes to support recursion by keeping track of previous function calls (call stack);
* Can be used to do a Depth First Search (DFS).

### Complexity

|   Operation   | Complexity |
| :-----------: | :--------: |
|    Pushing    |    O(1)    |
|    Popping    |    O(1)    |
|   Searching   |    O(n)    |
|   Removing    |    O(n)    |
| Size Checking |    O(1)    |

### Example – Brackets

```python
def validBraces(string):
    stack = []
    closingToOpeningMap = {
        "}": "{",
        ")": "(",
        "]": "["
    }

    for c in string:
        if c in "({[":
            stack.append(c)

        elif c in ")}]":
            if len(stack) == 0 or closingToOpeningMap[c] != stack.pop():
                return False

    return len(stack) == 0
```

