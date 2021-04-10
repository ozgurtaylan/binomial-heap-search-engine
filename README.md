# Data Structures Project

Applying Data Structure by using Priority Queue structured with Binomial Heap to observe simple search engines in C.

### Explanation

In WWW, when you enter a “keyword” search engines try to rank the documents
according to the similarities between your keyword and the documents lying on
WWW . The most famous search engines are Google, Bing, Yahoo... Various
machine learning and deep learning algorithms run behind these search engines. On
the other hand, the common logic behind them is to rank the documents according to
relevance.

In this project,binomial heaps are used to implement a priority queue for the
task of ranking documents. In other word, documents are ranked according to
given keyword with binomial heaps in this project.

### How it Works ? 

There are 42 documents in "DataStrcutersProject\files".

In this documents there are some comments about technological devices and brands.

After command shell opened, program wants an input key from user to find most relevant documents in files.

When user entered the input, program starts to rank all documents in file by calculating frequency level of the input in text datasets which are already cleaned and prepared by the program.

After ranking process program starts to build priority queue structured with binomial heaps according to ranked text files calculated before.

Finally program applys simple search engines and shows to user most relevant documents according to input key. 

(to avoid any error "DataStructureProject.c" file and "files" folder have to be in same directory)
