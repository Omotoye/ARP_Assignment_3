
# Libraries

## Compiling

All headers are conveniently referenced by the *arpnet.h* library, which resides in the root folder of the "ARP_headers" directory.
From within the source code it's sufficient to write
```c
	#include "arpnet.h"
```
to reference it.

Similarly, all libraries objects (the ones expresssed in the different subdirectories) are linked together in the static library
*libarpnet.a*, once again found in the root of the "ARP_headers" dir.

To correctly reference them when compiling it's sufficient to tell the compiler where to look (-I option for the header, 
-L for the library), following the pattern
```bash
	$ gcc -L<path_>/ARP_headers -I<path>/ARP_headers -o <source_name> <source_name>.c -larpnet

```
Where "path" referes to the path that make "ARP_headers" reachable from the current directory at the time of calling the
compiler. As a further example, supposing to have a structure with 

```bash 
root/
	|
	ARP_headers/
		|
		arpnet.h
		libarpnet.a
		<stuff>
	src/
		|
		hello_node.c
```
We can simply compile our "hello_node.c" into the executable "hello_node" by going into the
directory src/ and writing
```bash 
	$ gcc -L../ARP_headers -I../ARP_headers -o hello_node hello_node.c -larpnet
```

# First handshake

The first handshake is the moment before the definition of the first ever Turn Leader,
where the net should check if all nodes are reachable and if all are up-to-date with
the last versione of the header file. This is simply done by sending - once again -
a message around the ring of nodes. Such message contains a bitvector 
("available_nodes") and numeric field ("version").
> The actual implementation of the versioning depends on how deep we want things to be.
	We could easily set for a mere integer, incrementing each time a new version is
	released. Or we could do even more and properly generate an hash string from the
	headers content at run time, and check if the one computed locally by each node is
	equal to the one passed rfom ZERO (and assumed to be up-to-date).
	At the moment we decided to use simply a random generated string, as a placeholder

The check would go as follows:

- as always the node ZERO would kick the ball, by first sending a message to the node with
	id '1', initializing to 0's the bitvector "available_nodes" (all nodes assumed non-reachable)
	and "version" to the ID of its header's version;
- each time a node (including ZERO) tries to send a message to the next one in the "ring" it 
	will first check if the connection failed (connect() returned a negative value): in that
	case it will try to send the message to the
	next one in the table, repeating until a node is reachable (or it comes back to itself, in
	which case it throws an error): once a successful connection is established it marks that
	next node as "reachable", and proceeds to send the message;
- each time a node receives a message in this phase it first checks if its version corresponds
	to the one of ZERO. If that's not the case it marks itself as "unreachable", updating the
	bitvector contained in the message. It then tries to send the updated message to the 
	following node in the ring;

Note that once such message comes back to the ZERO node it should be sent once again along the
ring, since the complete state of the net will be expresssed in the "reachable_nodes" field of the
message only after all nodes had a chance to record their status/ the status of their neighbors.
For this second trip
- each time a node receives a message it updates its *local* ip_table by copying the "reachable_nodes"
	field contained in the received message, in order to have a copy of which nodes are actually
	active part of the net at the moment.
- it then simply sends this message to the next *reachable* node from the table just updated.
