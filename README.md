# CacheSim
Simulation of simplified cache

1 Cache Simulation

A cache is a collection of cache lines, each of which may store a block of memory along with some
additional information about the block (for example, which addresses it contains). Each block
contains the same number of bytes, known as the block size. The block size will always be a power of
two. The cache size is the block size multiplied by the number of cache lines (that is, the additional
information is not counted in the cache size).
Consider a system with 48-bit addresses and a block size of 16 bytes. Each block will begin with
an address divisible by 16. Because 16 = 24
, the last 4 bits of an address will determine its offset
within a particular block. For example, the address ffff0000abcd (hexadecimal) will have offset d.
The remaining 44 bits of the address may be considered a block identifier.
If the block size were 8 bytes, then the last 3 bits of the address will be its block offset. The last
three bits of ffff000abcd are 101 (binary) or 5 (hexadecimal). The most-significant 45 bits will be
the block identifier. (Exercise: write the block identifier in hexadecimal

.1)
11ffe0001579
1
For a cache with a single cache line, that cache line will store the 16 bytes of the block along
with the block identifier. Each memory access will first check whether the address is part of the
block currently in the cache (if any). Since we are only simulating memory reads and writes, you
cache will not need to store the actual blocks.

1.1 Memory operations
Your simulator will simulate two memory operations: reading and writing individual bytes. Your
program will read a trace file describing addresses to read or write from, and will keep track of
which blocks are stored in which cache lines in order to determine when these memory operations
result in actual reads and writes to memory.
Note that your program only keeps enough information to simulate the behavior of the cache. It
does not need to know the actual contents of memory and the blocks stored in the cache lines; this
information is, in fact, not available.
Your program will simulate a write-through cache. The operations supported are reading and
writing from an address A.
read A Read the byte at address A. If the block containing A is already loaded in the cache, this
is a cache hit. Otherwise, this is a cache miss: note a memory read and load the block into a
chosen cache line (see sections 1.2 and 1.4).
write A Write a byte at address A. If the block containing A is already loaded in the cache, this is
a cache hit: note a memory write. Otherwise, this is a cache miss: note a memory read, load
the block into a chosen cache line, and then note a memory write.
Your program will track the number of cache hits, cache misses, memory reads, and memory
writes.
Note that loading an address or block into the cache means changing the information about
a particular cache line to indicate that it holds a particular block. Since your simulator does not
simulate the contents of memory, no data will be loaded or stored from the address itself.

1.1.1 Prefetching
Prefetching is a technique used to increase spatial locality in a cache. In the operations described
above, blocks are read from memory only after a cache miss. If a cache uses prefetching, each cache
miss will also ensure that the next block is loaded into the cache.
That is, if an attempt to access A results to a cache miss, the simulator will load the block
containing A into the cache. A prefetching cache will also check whether the block containing A + B
(where B is the block size) is present in the cache and will load it if not.
This pseudo-code illustrates how to simulate a memory read when prefetching:
read(A):
B <- block id for A
if B in cache:
increment cache_hits
else:
increment cache_misses
increment memory_reads
2
store B in cache
C <- block id for A + block_size
if C not in cache:
increment memory_reads
store C in cache
Note that the prefetching step is not considered a cache hit or a cache miss.
The behavior is similar for write operations, except that the prefetched block is not written back
to memory.
Your program will simulate the behavior of a cache with or without prefetching.

1.2 Mapping
For caches with multiple cache lines, there are several ways of determining which cache lines will
store which blocks. Generally, the cache lines will be grouped into one or more sets. Whenever a
block is loaded into a cache line, it will always be stored in a cache line belonging to its set. The
number of sets will always be a power of two.
Let B be the block size. The least significant log2 B = b bits of an address will be its block offset.
If there are S sets, then the next least significant log2 S = s bits of the address will identify its set.
The remaining 48 − s − b bits are known as the tag and are used to identify the block stored in a
particular cache line.
For example, for a cache with 16 sets and 16-byte blocks, the address ffff0000abcd is in set 12
(c). The first 40 bits, ffff000ab, are the tag.
Your program will simulate three forms of cache:
Direct-mapped This is the simplest form of cache, where each set contains one cache line and no
decisions need to be made about where a particular block will be stored.
n-way associative In this form of cache, each set contains n cache lines, where n is a power of
two. The particular block stored in each line is identified by its tag.
Fully associative This form of cache puts all the cache lines in a single set.
For direct and n-way associative caches, your program will need to derive the number of sets
(S) from the associativity A (the number of cache lines per set), the block size B, and the size of
the cache C using the relation C = SAB. For fully associative caches, S = 1, but you will need to
determine A using the same relation.2

1.3 Calculating block, set, and tag
We will simulate a cache in a system with 48-bit addresses. Since int has 32 bits on the iLab
machines, you will want to use a long or unsigned long to represent the addresses. Using some
other type, such as a string containing hexadecimal digits, is not recommended for efficiency reasons.
Your program will need to extract sequences of bits from the addresses it works with. When
simulating a cache with block size B = 2b
, the least significant b bits of an address (the block offset)
may be ignored. Code such as X >> b will effectively remove the block offset, leaving only the
block identifier.
2Can direct-mapped and fully associative caches be considered special cases of n-way associativity?
3
The least significant s bits of the block identifier identify the set. To obtain these, recall that
2
k − 1, represented in binary, is all zeros except for the last k bits, which are ones. That is, 2
2 − 1 is
112, 2
4 − 1 is 11112, and so forth. Recall that 2
k may be easily computed by 1 << k.
The tag is the portion of the address remaining after removing the block offset and set identifier.

1.4 Replacement policies
Each cache line is either valid, meaning it contains a copy of a block from memory, or invalid,
meaning it does not. Initially, all cache lines are invalid. As cache misses occur and blocks are
loaded into the cache, those lines will become valid. Eventually, the cache will need to load a block
into a set which has no invalid lines. To load this new block, it will select a line in the set according
to its replacement policy and put the new block in that line, replacing the block that was already
present.3
We consider two replacement policies in this assignment:
fifo (“First-in, first-out”) In this policy, the cache line that was loaded least recently will be replaced.
lru (“Least-recently used”) In this policy, the cache line that was accessed least recently will be
replaced.4
Your simulator will implement fifo. Implementing lru is left for extra credit.
Note that each set contains a fixed, finite number of cache lines. To determine the oldest block
in the cache, it is sufficient to store the relative age of each cache line. That is, the first time a block
is loaded into a cache line, its relative age is set to 0 and the ages of all other valid cache lines are
increased by 1. This ensures that every cache line will have a unique age. Once all cache lines are
valid (i.e., contain data), a sequential search is sufficient to determine the oldest cache line.5
2 Program
You will write a program cachesim that reads a trace of memory accesses and simulates the behavior
of a cache for that trace. The program will be awarded up to 300 points by the auto-grader. (The
version of the auto-grader included for your testing gives up to 150 points.) An optional second part
may be completed for extra credit.
Your program will take six arguments. These are:
1. The cache size, in bytes. This will be a power of two.
2. The associativity, which will be “direct” for a direct-mapped cache, “assoc:n” for an n-way
associative cache, or “assoc” for a fully associative cache (see section 1.2)
3. The prefetching policy, which will be “p0” (no prefetching) or “p1” (prefetch one block ahead).
4. The replacement policy, which will be “fifo” or “lru” (see section 1.4).
5. The block size, in bytes. This will be a power of two.
3For direct-mapped caches, no decision needs to be made, because each set only contains one cache line.
4For a prefetching cache, the prefetched block is considered to be accessed if it is loaded from memory, but is not
accessed if it is already in the cache.
5Sequential search is O(n), but for our purposes n will always be fairly small.
4
6. The trace file.
Your program will use the first four arguments to configure a cache. It will read the memory
accesses in the trace file and simulate the behavior of that cache. When it is complete, it will print
the number of cache hits, cache misses, memory reads, and memory writes.
Usage
./cachesim 512 direct p0 fifo 8 trace1.txt
Your program SHOULD confirm that the block size and cache size are powers of two. If the
associativity is “assoc:n”, your program SHOULD confirm that n is a power of two.
Your program MUST implement the “fifo” replacement policy. Your program MAY implement
“lru” for extra credit.
Input The input is a memory access trace produced by excuting real programs. Each line describes
a memory access, which may be a read or write operation. The lines contain three fields, separated
by spaces. The first is the program counter (PC) at the time of the access, followed by a colon.
The second is “R” or “W”, indicating a read or write, respectively. The third is the 48-bit memory
address which was accessed. Additionally, the last line of the file contains only the string “#eof”.
Here is a sample trace file:
0x804ae19: R 0x9cb3d40
0x804ae19: W 0x9cb3d40
0x804ae1c: R 0x9cb3d44
0x804ae1c: W 0x9cb3d44
0x804ae10: R 0xbf8ef498
#eof
You MAY assume that the trace file is correctly formatted. Note that the PC is not used by
your simulator, and may be ignored.
Recall that the format code for hexadecimal integers is x. To read and discard an arbitrarily
large hex int, you may use %*x. To read a hex int and store in a long int, you should use %lx.
Output Your program will print the number of cache hits, cache misses, memory reads, and
memory writes observed when simulating the memory access trace.
The output must have this form:
Memory reads: 3499
Memory writes: 2861
Cache hits: 6501
Cache misses: 3499
Note that spacing and capitalization must be correct 
