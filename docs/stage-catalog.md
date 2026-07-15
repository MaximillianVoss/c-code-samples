# Stage catalog

| Date | Original name | Purpose | Disposition |
|---|---|---|---|
| 2016-04-12 | `Lab 5` | Allocate a matrix and sort every row | Preserved; behavior recovered in the C17 module |
| 2016-04-27 | `lab1изменение размера массива` | Grow or shrink a heap array by a signed delta | Preserved; allocation contract rewritten and tested |

The dates come from the original source/project files and are also retained in
the preservation manifest. The two folders are course stages, not duplicate
copies of one project.

The original `Lab 5` passes matrix dimensions to its print routine in reverse,
uses `void main`, omits a required time header, and never frees the allocated
rows. The resize exercise manually copies buffers and has no overflow or
allocation-failure contract. These defects are documented rather than edited
inside the byte-exact legacy snapshots.

