#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
/*
	by @ozgur-taylan
*/
//--------------------------------------------------------------Prioity Queue--------------------------------------------------------------------
/*Structure of Node.*/
struct node{
         int key;
         char* text;
         char* doc_name;
         int degree;
         struct node* parent;
         struct node* child;
         struct node* sib;
		};
//Structure of Heap.
struct heap{
	struct node* head;
};
/*Method that creates new node.
key = frequancy of given input in the given 42 text files.
text = the original text.
doc_name = name of the document(content_123456...).*/
struct node* create_node(int key,char* text,char* doc_name) {
	struct node* node = (struct node*)malloc(sizeof(struct node));
		
	node->key = key;
	node->text = text;
	node->doc_name = (char*)malloc(sizeof(char)*strlen(doc_name));
	strcpy(node->doc_name, doc_name);
	
	node->degree = 0;
	node->child = NULL;
	node->parent = NULL;
	node->sib = NULL;
	
	return node;
};
/*This method basically links the given node with given heap without checking any conditions. (degreee, great, small).
  If given heap is NULL then method allocates a new heap.*/
struct heap* link_node(struct node* newNode,struct heap* heap){
	struct heap* temp = heap;
	
	if(temp == NULL){
		temp = (struct heap*)malloc(sizeof(struct heap));
		temp->head = newNode;
		return temp;
	}
	newNode->sib = temp->head;
	temp->head = newNode;
	heap = temp;
	return heap;	
}
/*This method arranges the given heap according to degree and max heap properties(greater key value). The given heap is the returned heap which is linked before in "link_node" method above.
  Method runs as recursively, First it checks the degrees between head and its sibling if the checking process is true than it checks the values of them.After that processes
  due to there can not be 2 node with same degree in the root's line, method arranges child and sibiling pointers according to key value and it repeats recursively until there are no nodes with same degree in root's line. */
void arrange_heap(struct heap* heap){
		
		struct  heap* temp_heap = heap;
	
	if(temp_heap->head->sib != NULL){
			
		struct node* current = temp_heap->head;
		struct node* next = temp_heap->head->sib;
			
		if(current->degree == next->degree){
				
			if(current->key > next->key){
					
				if(next->sib != NULL){
					current->sib = next->sib;
					next->sib = NULL;
				}else{
					current->sib = NULL;
				}
				
				if(current->child != NULL){
					next->sib = current->child;
				}	
				current->child = next;
				next->parent = current;
				current->degree++;
				temp_heap->head = current;
			
				arrange_heap(temp_heap);
			}
			if(next->key >= current->key){
										
				if(next->child != NULL)
					current->sib = next->child;
				else
					current->sib = NULL;
								
					next->child = current;
					current->parent = next;
					next->degree++;
					temp_heap->head = next;
					arrange_heap(temp_heap);
				}
					heap = temp_heap;			
			}
					
		}else{
			return;
		}	
}
/*This method is basically insert method.
  Firstly,"create_node" method creates the new node.
  Secondly, "link_node" method links the freshly created new node to the given heap.
  Thirdly, after "link_node" method, heap is not the proper heap so arrange_heap method runs.
  Finally, heap is arranged and method returns the proper heap.*/
struct heap* enqueue(int key,char* text,char* doc_name, struct heap* heap){
	struct heap* temp = heap;
	struct node* node_inserted = create_node(key,text,doc_name);
	temp = link_node(node_inserted,temp);
	arrange_heap(temp);
	heap = temp;
	return heap;
}
/*This method helps to extracting max node in binomial heap.
print_node = the node parameter that keeps the information about deleted max node.
According to degrees of the given heap method deletes the node and free it from memory.*/
void delete_head(struct heap* heap,struct node* print_node){
	
		struct heap* temp_heap = heap;	
		struct node* head_delete = temp_heap->head;
		
		print_node->key = head_delete->key;
		print_node->text = head_delete->text;
		print_node->doc_name = head_delete->doc_name;
			
		if(head_delete->sib == NULL){					
		
			if(head_delete->degree == 0){
				free(head_delete);
				temp_heap->head = NULL;
				temp_heap = NULL;
				heap = temp_heap;
			}
			else if(head_delete->degree == 1){
				temp_heap->head = head_delete->child;
				temp_heap->head->parent = NULL;
				free(head_delete);
				heap = temp_heap;
			}		
			else if(head_delete->degree > 1){
				temp_heap->head = head_delete->child;
				free(head_delete);	
			
				struct node* current = temp_heap->head;
				struct node* next = current->sib;
				temp_heap->head->sib = NULL;
			
				while(next != NULL){
				
				current = next;
				if(next->sib != NULL)
						next = next->sib;
				else
					next = NULL;
					
				current->sib = temp_heap->head;	
				temp_heap->head = current;	
			}
		  	 	heap = temp_heap;	
		}
			 	
		}else{
				temp_heap->head = head_delete->sib;
				free(head_delete);
				heap = temp_heap;
		}
}
/*
  When the max node extracted , to obtain a binomial heap structure again,	
  This method adds the nodes of given heap to another heap and arranges them to otain a proper binomial heap structure.
  temp_heap_2 parameter is the heap which has been extracted max node in "delete_head" method. 
  This methods uses the link_node method for node adding proccess and it uses the arrange_heap method for editing degrees , child pointers etc. */
void edit_heap(struct heap* temp_heap_1 , struct heap* temp_heap_2){
	
		struct node* current = temp_heap_2->head;
		
		while(current != NULL){
			
			if(current->sib != NULL)
				temp_heap_2->head = current->sib;
			else
				temp_heap_2->head = NULL;
				
			current->sib = NULL;
			temp_heap_1 = link_node(current,temp_heap_1);
			arrange_heap(temp_heap_1);
			current = temp_heap_2->head;			
		}	
}
/*When extracting max node from binomial heap there are many factors to be careful with.
  For instance, if the node which going to delete has sibling or if there is a node which has sibling to deleted node etc.
  First , this method walks between the head and its siblings than it finds the node which is going to be extracted and which has maximum key value.
  Second , method keeps the information about extracted node (sibiling of it , previous of it etc.) temporarily. According to that information several cases occurs.
  With checking theese cases ,
  Method basically splits one heap to two heaps , one of the heaps has a node which is going to be extracted.By using methods above, The node has been extracted than
  Two binomial heap has been merged and arranged by using methods which are used to create binomial heap.*/
struct heap* dequeue(struct heap* heap,struct node* print_node){		
			
		struct heap* temp_heap_1 = heap;
	
		if(temp_heap_1 != NULL && temp_heap_1->head != NULL){
				
			struct heap* temp_heap_2 = (struct heap*)malloc(sizeof(struct heap));
			temp_heap_2->head = NULL;
		
			if(temp_heap_1->head->sib == NULL){
				delete_head(temp_heap_1 ,print_node);
				heap = temp_heap_1;
				return heap;
			}
			else{
				struct node* node_delete = temp_heap_1->head;		
				struct node* previous = NULL;
				struct node* current = temp_heap_1->head;
				struct node* next = current->sib;
			
				while(next != NULL){
				
				if(next->key > node_delete->key){
					node_delete = next;
					previous = current;
				}
					current = next;
				
					if(next->sib == NULL)
						next = NULL;
					else 
						next = next->sib;	
				}
							
				if(previous != NULL){
					if(node_delete->sib != NULL){
						previous->sib = node_delete->sib;
						node_delete->sib = NULL;
						temp_heap_2->head = node_delete;				
						delete_head(temp_heap_2,print_node);
						edit_heap(temp_heap_1 , temp_heap_2);
						heap = temp_heap_1;	
						return heap;
										
					}else{
						previous->sib = NULL;
						temp_heap_2->head = node_delete;
						delete_head(temp_heap_2,print_node);				
						edit_heap(temp_heap_1 , temp_heap_2);
						heap = temp_heap_1;	
						return heap;
						
						}
				}
				else{
					
					if(node_delete->degree == 0){
						
						delete_head(temp_heap_1,print_node);
						heap = temp_heap_1;	
						return heap;
						
					}else{
						temp_heap_1->head = temp_heap_1->head->sib;
						node_delete->sib = NULL;
						temp_heap_2->head = node_delete;
						delete_head(temp_heap_2,print_node);	
						edit_heap(temp_heap_1 , temp_heap_2);
						heap = temp_heap_1;	
						return heap;						
						}	
					}
			}
			free(temp_heap_2);
	}else{
		return NULL;
	}
}
//------------------------------------------------------------------INPUT AND TEXT-----------------------------------------------------------------------------

/*To get a method with dynamically memory allocation I use the "fseek" and "ftell" in this method.
  "fseek" and "ftell" helps to find what size the text has been allocated.
  After allocation process, method reads the given file with "fread" and it returns char pointer.*/
char* read_text(FILE* file){
	char* text = NULL;
		if(file != NULL){
			int lenght;
	   		fseek(file , 0, SEEK_END);
	   		lenght = ftell(file);
	   		fseek(file,0,SEEK_SET);	
	  		text = (char*)malloc(sizeof(char) * (lenght+1) );
	  		
	  		int a = 0;
			for(a = 0 ; a<lenght+1 ; a++) text[a] = '\0';
		
			if(text != NULL){
				fread(text,sizeof(char),lenght,file);		
				return text;
			}else{
				return;
			}
			
		}else{
			printf("\n\nCan not find any documant to read ! \n\n");
			return;
		}
}
/*This method works to remove specific punctuation marks in the text.
 The rule is : if the previous and next pointer of punctuation mark pointer are letters or numbers, don't delete the punctuation mark.
 After some specific editing, text becomes the punctual marks removed text. */
void clear_text(char* text1){
	char* temp = text1;
	
	if(temp != NULL){
		int a = 0;
		for(a = 0 ; a < strlen(temp) ; a++){
			if(isalpha(temp[a]) == 0){
				if(isdigit(temp[a]) == 0){
					if( (isalpha(temp[a-1]) !=0 || isdigit(temp[a-1]) !=0 ) && (isalpha(temp[a+1]) !=0 || isdigit(temp[a+1]) !=0 ) ){
						if(temp[a] == ':' || temp[a] == 47 || temp[a] == 92 || temp[a] == '&')
							temp[a] = ' ';
					}else{
					temp[a] = ' ';
					}
				}
			}
		}			
		text1 = temp;
		
	}else{
		printf("Can not find any documant to clear ! \n\n");
		return ;
	}
}
/*Method calculates frequency by splitting given text to words and  by comparing with given input and these words.*/
int calculate_freq(char* text,char input[] , int number){
								
	if(text != NULL){
		char * split;
 		split = strtok (text," ");
 		while (split != NULL){	   
			if(strcmp(split,input) == 0) 
				number++;
				
  		    split = strtok (NULL, " ");
 		 }
 		return number;
	}else{
		printf("Can not find any documant to calculate ! \n\n");
		return 0;
	}
}

//----------------------------------------------------------------------MAIN------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
			int number_of_relevant = 0;
			struct heap* myheap = NULL; // Creating binomial heap for priority queue
		
			printf("Please enter the word want to search = "); // Asking user input.
			char input[50];
	   		scanf("%s",input);
	   	 
			
	        int number = 0;
		    struct dirent* entry;  // Using File Directory for accessing to all contents from file.
    	    DIR* dir;
		    dir = opendir("files");
    		if (dir == NULL) return;
   			 
    		while ((entry = readdir(dir)) != NULL) {
        		if ( !strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") ){
        			continue;
				}else{
					
						char location[] = "files/";
     					char* file_name = entry->d_name; 		
     					FILE* file = fopen(strcat(location,entry->d_name),"r");// opening file
     					
     					char* text_orijin = read_text(file); // keeping the original text to print it after
	 										
	 					if(text_orijin == NULL){// Condition that if there is no document in files.
	 						return 0;
						 }
						else{
							char* text_cleaned = read_text(file);
							clear_text(text_cleaned); // cleaning text to get best result.
						 	number = calculate_freq(text_cleaned,input,0); // Calculating the frequency of given input in 42 text files.
						
							// Just insert new node in queue when it matched to get number of relevant documents.
							if(number != 0){
								myheap = enqueue(number,text_orijin,file_name,myheap);
								number_of_relevant++;
							}														
							number = 0;
							fclose(file);
						}
     					
					}
			}
			printf("\n\n--------------------------------\n\n");
		
//---------------------------------------------------This part written for satisfying the asking output format-----------------------------------------------------		
		
			struct node* relevance_texts[5]; // Keeping 5 text as a node in array.	
			if(myheap == NULL){
				printf("There is no relevance between input and the texts so number of relevant documents : %d\n\n",number_of_relevant);
				free(myheap);
			}else{
				printf("Number of Relevant Document : %d \n\n",number_of_relevant);
				printf("The relevance order is: ");					
					int a = 0; 
					for(a = 0 ; a<5 ; a++){
						struct node* temp = (struct node*)malloc(sizeof(struct node));
						temp->child = NULL;
						temp->parent = NULL;
						temp->sib = NULL;
						temp->degree = -1;
						temp->key = -1;
						temp->doc_name = NULL;
						temp->text = NULL;	
						relevance_texts[a] = temp;
				
						dequeue(myheap,relevance_texts[a]);
						if(relevance_texts[a]->key != -1){
						printf("%s(%d) ",relevance_texts[a]->doc_name,relevance_texts[a]->key);
						}
					}
					printf("\n\n");
					for(a =0 ; a<5 ;a++){
					 	if(relevance_texts[a]->key != -1){
						printf("%s(%d): %s \n\n",relevance_texts[a]->doc_name,relevance_texts[a]->key,relevance_texts[a]->text);
						}
					}
				free(myheap);
			}			
		return 0;
}

