Grading
The whole implementation grading can be broadly divided into 4 parts:

User Functionalities - 150 marks
System Requirements - 40 marks
Specifications - 10 marks
Bonus Functionalities - 50 marks (Optional)
So, the total marks add up to 250. The further breakdown of marks for each functionality is mentioned in the respective sections above.

TA Section
How do I start?
Define the calls. For example, define the format of the functionality of the read call for the client as READ path and then divide the work amongst yourselves with each team member implementing either the client, the naming server or the storage server code.
Identify things that can be decoupled. For example, the specification where an SS can join the NM at any given moment of execution doesn’t depend on neither the client or the SS (Assuming you have figured out how SSs attach themselves at the beginning of the execution). One team member can implement this while the other thinks of ways to implement caching (You probably won’t get merge conflicts as you’ll be changing different parts of the naming server code)
Remember Figuring things out is just as important as coding the implementation in this project. Not everyone needs to be programming at once. One could design the redundancy attribute and start coding it up later.

Some pointers:
Use TCP sockets
You may use any POSIX C library (opengroup link)
Use wireshark for debugging TCP communications by inspecting your packets when required.
You can use netcat to create client/server stubs so that you can start testing your components even if some of it hasn’t been coded up.
As always, decompose the problem and write modular code
Cite your resources if you take any ideas or code
Make necessary assumptions
Resources
A few slides on the topic
CMU slides on Distrbuted File Systems
Rutgers University’s resources
Least Recently Used (LRU) Caching
Handling multiple clients
Handling multiple clients without multithreading (This section will be updated on specific requests in the doubts document)
The Apache Hadoop DFS
Tips from the TAs :)
Reference Code to get started
Hints in the doc are merely for providing a direction to one of the solutions. You may choose to ignore them and come up with your own solutions.
Communicate with your teammates if you cannot fulfill your deliverables on time. That is the entire point of team-work.
Last, but not the least, START EARLY. This is a big project and will take time and coordination to complete.
Why are the requirements so trash?
The requirements might feel very unideal for the usecase at times. However, we would like to highlight that this is a learning exercise (through a usecase) and not a production system. The requirements are designed to make you think about the various aspects of distributed systems and file systems in particular.
We would also like to ensure that all the requirements and implementables were throughly discussed in TA meets before reaching this document and finally you. To give a few examples (with hopes of igniting that system thinking spark and no thought of flex, okay maybe a bit)
Using period as delimiter might seem like a bad idea, especially when it is in the middle of a word.
You might think that use of newline character would be better. However, this would mean that every time a user wants to write a new sentence, they would have to add a newline character at the end. NOT USER_FRIENDLY DESIGN
Another approach could be using fixed character lengths for sentences. But, this would require the system to pad sentences with spaces to reach the fixed length (or even worse, splitting words mid-way). NOT SPACE_EFFICIENT DESIGN
The point is, there is no perfect solution. Every design decision has its pros and cons. The requirements are designed to make you think about these trade-offs and come up with a solution that balances them effectively.
Anyways in the industry (and DASS next sem), you will be working with, probably, worse requirements. So, this is a good practice run.
ALL THE BEST! BUT MOST IMPORTANTLY, HAVE FUN!