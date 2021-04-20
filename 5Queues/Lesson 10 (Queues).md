## Queues

### What is a Queue?

> A queue is a linear data structure which models real world queues by having two primary operations: **enqueue** (*offers* element) and **dequeue** (*polls* element).

### When and where is a Queue used?

* Any waiting line models a queue, for example a lineup at a movie theatre;
* Can be used to efficiently keep track of the **N** most recently added elements;
* Web server request management where you want "first come – first serve” principle;
* Can be used to do a Breadth First Search (BFS).

### Complexity

|   Operation   | Complexity |
| :-----------: | :--------: |
|    Enqueue    |    O(1)    |
|    Dequeue    |    O(1)    |
|   Searching   |    O(n)    |
|   Removing    |    O(n)    |
| Size Checking |    O(1)    |

### Queue Example – BFS

```python
q = []
q.append(starting_node)
starting_node["visited"] = True

while not q.empty():
  node = q.pop(0)
  for neighbour in neighbours(node):
    if not neighbour["visited"]:
      neighbour["visited"] = True
      q.append(neighbour)
```

