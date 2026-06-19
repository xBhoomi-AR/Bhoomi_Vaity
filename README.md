### My Approach

I would like to explain the working of each function which i have used in my program. 
The erase function is used whenever a memory block needs to be promoted from a lower cache level to a higher cache level. It removes the specified memory block from both the deque and the corresponding hash set, ensuring that duplicate copies of the same block do not exist within the cache hierarchy.

The insertL1 function is responsible for inserting memory blocks into the L1 cache. Before insertion it checks whether the block is already present. If the cache is full, the oldest block is removed and transferred to the L2 cache. This helps maintain the limited size of the L1 cache while preserving recently accessed data.

The insertL2 function inserts memory blocks into the L2 cache. Similar to L1 it first checks for duplicates and if the cache is full the oldest block is removed and moved to the L3 cache. This creates a hierarchical type movement of data between cache levels.

The insertL3 function handles insertion into the L3 cache. If L3 becomes full the oldest memory block is permanently removed from the cache hierarchy. 

Now, the access function simulates memory access. Whenever a task requests a memory block the function checks whether it exists in L1, L2, or L3 cache. If found, the corresponding cache hit latency is returned. If the block is not found in any cache level or  a RAM miss occurs, the RAM access counter is incremented, and the block is inserted into L1 cache. This function adresses cache hit and miss behavior and also promotes frequently accessed data to higher cache levels.

I have used a Round Robin Scheduler because it provides fair CPU allocation among all tasks. Every task receives a fixed time quantum of 3 units, ensuring that no task can occupy the processor for a very long time. If a task is not completed within its quantum, it is placed into the ready queue, allowing other waiting tasks to execute. This ensures balanced CPU utilization.

I have used a Multicore Architecture consisting of 3 CPU cores for parallel task execution. Instead of executing one task at a time, multiple tasks can run simultaneously on different cores, reducing overall execution time and reducing no. of cycles thereby. This is better because in modern processors, multiple cores work together to handle several tasks parallely and improve performance.
