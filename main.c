// PREPROCESSOR DEFINITIONS:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_LENGTH 300
#define LINE_LENGTH 1500
#define MOVIES_SIZE 200
#define ACTORS_SIZE 5000
#define QUEUE_SIZE 5000

// ************************************************


// STRUCT DEFINITIONS:

// Neighbor node of vertices in adjacency list:
struct neighbor{
	int actorNo;				// Keeps the actor's index in the actors array which stored in related node.
	int movieNo;				// Keeps the index of common movie between neighbor and it's vertex node.
	struct neighbor * next;		// Keeps pointer to next neighbor.
};
typedef struct neighbor n;

// Vertex node of adjacency list: 
struct vertex{
	int index;					// Keeps actor's index in the actors array.
	char name[NAME_LENGTH];		// Keeps actor's name.
	struct neighbor * next;		// Keeps pointer to it's neighbor.
	struct vertex * parent; 	// Keeps pointer to parent node which caused this node added to queue.
};
typedef struct vertex v;

// Queue struct (will be used in BFS):
struct queue{
	v * array[QUEUE_SIZE];		// Keeps items in queue.
	int front;					// Keeps the front index of queue.
	int rear;					// Keeps the rear index of queue.
	int capacity;				// Keeps the maximum capacity of queue.
	int size;					// Keeps current number of items in queue.
};
typedef struct queue q;

// ************************************************


// FUNCTION PROTOTYPES:

// Returns the actor's index with given name in the actors array which has the size actorCount. If actor not found, returns -1:
int findActor(v * actors[], int actorCount, char name[]);

// Adds the actor with given name to actors array which has the size actorCount:
void addActor(v * actors[], int actorCount, char name[]);

// Create adjacency list of the actor in actors[index] using currentLine string and related movieNo:
void addConnections(v * actors[], int actorCount, int index, char currentLine[], int movieNo);

// Creates a queue and returns it's pointer.
q * createQueue();

// If queue is full returns 1, else returns 0:
int isFull(q * queue);

// If queue is empty returns 1, else returns 0:
int isEmpty(q * queue);

// Enqueues item to rear of queue:
void enqueue(q * queue, v * item);

// Dequeues item from front of queue and returns dequeued item's pointer. If queue is empty, returns NULL.
v * dequeue(q * queue);

// Returns front item of queue:
v * getFront(q * queue);

// Returns rear item of queue:
v * getRear(q * queue);

// Returns index of the movie which actors[index] and actors[index2] played both.
int findCommonMovie(v * actors[], int index1, int index2);

// Returns Kevin Bacon Number of the actor with given index in actors array with the size actorCount using baconsIndex.
int findBaconNumber(v * actors[], int actorCount, int index, int baconsIndex);

// Returns Kevin Bacon Number of the actor with given index in actors array with the size actorCount using baconsIndex, also prints the path.
int findBaconNumberWithPrint(v * actors[], char movie[MOVIES_SIZE][NAME_LENGTH], int actorCount, int index, int baconsIndex);

// ************************************************


// MAIN FUNCTION:

int main(){
	
	// Declarations:
	v * actors[ACTORS_SIZE];				// Array of actors
	char buff[LINE_LENGTH];					// Buffer which keeps the current line as a char array
	char movie[MOVIES_SIZE][NAME_LENGTH];	// Array of movie names
	char name[NAME_LENGTH];					// Buffer which keeps the name temporarily
	char userIn[NAME_LENGTH];				// User's input (actor name to find Kevin Bacon number)
	char fileName[NAME_LENGTH];				// File name that given by user
	int baconNumbers[ACTORS_SIZE];			// Array which keeps actors Kevin Bacon numbers
	int baconsIndex;						// Keeps the index of Kevin Bacon in actors array
	int i;									// Iterator
	int j;									// Iterator
	int index;								// Keeps the index of current actor temporarily
	int actorCount;							// Keeps the current number of actors
	int lineNo;								// Keeps the current line number
	int counted;							// Number of actors that Kevin Bacon number has found
	int counter;							// Counter keeps the number of actors with a Kevin Bacon number
	FILE * fp;								// File pointer for reading
	
	// Ask user for file name:
	fp = NULL;
	while(fp == NULL){
		printf("Please enter the name of the file which you wish to read the data.\n");
		gets(fileName);
		printf("\n************************************************\n\n");
		fp = fopen(fileName,"r");
		if(fp == NULL)
			printf("Invalid file name. ");
	}
	
	// Initializations:
	actorCount = 0;
	lineNo = 0;
	
	
	// 1) Read the file and create array of actors:
	
	// Loops once for each line until end of file:
	while(!feof(fp)){
		
		i = 0;							// Reset i to 0 for each line, thus it starts from the first char of the current line.
		fgets(buff,LINE_LENGTH,fp);		// Load the current line to the buff string.
		
		// Save movie name to array of movie names:
		while(buff[i] != '/'){
			movie[lineNo][i] = buff[i];
			i++;
		}
		movie[lineNo][i] = '\0';
		///////////////////////
		
		// Loops once for each actor until end of the line:
		while(buff[i] != '\n'){
			
			i++;	// Current char is '/', skip it by increasing i.
			
			// Keep the last name in the lastName string:
			j = 0;
			while(buff[i] != '/' && buff[i] != '\n'){
				name[j] = buff[i];
				i++;
				j++;
			}
			name[j] = '\0';
			///////////////////
			
			// If actor does not exist in actors array, then add it to the array and get it's index:
			if(findActor(actors, actorCount, name) == -1){
				addActor(actors, actorCount, name);
				index = actorCount;
				actorCount++;
			}
		}
		lineNo++;
	}
	
	fseek(fp, 0, SEEK_SET);		// Reset file pointer to start of the file.
	lineNo = 0;					// Reset lineNo to 0.
	
	
	// 2) Create the graph by adjacency list:
	
	// Loops once for each line until end of file:
	while(!feof(fp)){
		
		i = 0;						// Reset i to 0 for each line, thus it starts from the first char of the current line.
		fgets(buff,LINE_LENGTH,fp);	// Load the current line to the buff string.
		
		// Skip movie name to read actor names:
		while(buff[i] != '/'){
			i++;
		}
		///////////////////////
		
		// Loops once for each actor until end of the line:
		while(buff[i] != '\n'){
			
			i++;	// Current char is '/', skip it by increasing i.
			
			// Keep the last name in the lastName string:
			j = 0;
			while(buff[i] != '/' && buff[i] != '\n'){
				name[j] = buff[i];
				i++;
				j++;
			}
			name[j] = '\0';
			///////////////////
			
			// Get the index of actor in actors array and create adjacency list for the actor:
			index = findActor(actors, actorCount, name);	
			addConnections(actors, actorCount, index, buff, lineNo);
		}
		lineNo++;
	}
	
	baconsIndex = findActor(actors,actorCount,"Bacon, Kevin");
	
	
	// 3) Calculate Kevin Bacon numbers of all actors:
	for(i=0; i<actorCount; i++){
		baconNumbers[i] = findBaconNumber(actors, actorCount, i, baconsIndex);
	}
	
	
	// 4) Calculate number of actors with each Kevin Bacon number and print:
	counted = 0;
	i=-1;
	while(counted < actorCount){
		counter = 0;
		for(j=0; j<actorCount; j++){
			if(baconNumbers[j] == i){	
				counter++;
				counted++;
			}	
		}
		if(i == -1 && counter != 0)
			printf("Number of actors with Kevin Bacon number Infinite: %d\n",counter);
		else if (counter != 0)
			printf("Number of actors with Kevin Bacon number %d: %d\n",i,counter);
		i++;
	}
	
	fclose(fp);
	
	
	// 5) Find the actor's Kevin Bacon number which entered by user:
	while(strcmp(userIn,"E") != 0){
		printf("\n********************************\n\n");
		printf("Please enter the actor you wish to find Kevin Bacon number (Enter 'E' to exit):\n");
		gets(userIn);
		printf("\n");
		i = findActor(actors, actorCount, userIn);
		if(strcmp(userIn, "E") == 0){
			return 0;
		}
		else if(i >= 0){
			j = findBaconNumberWithPrint(actors, movie, actorCount, i, baconsIndex);
			if(j > 0)
				printf("\nKevin Bacon number of %s: %d\n",actors[i]->name,j);
			else if(j == 0)
				printf("Kevin Bacon number of %s: %d\n",actors[i]->name,j);
			else
				printf("Kevin Bacon number of %s: Infinite\n",actors[i]->name);
		}
		else{
			printf("Actor not found.\n");
		}	
	}	
	return 0;
}
// ************************************************


// FUNCTION DEFINITIONS:

int findActor(v * actors[], int actorCount, char name[]){
	int i;
	for(i=0; i<actorCount; i++){
		if(strcmp(actors[i]->name,name) == 0)
			return i;	
	}
	return -1;
}

void addActor(v * actors[], int actorCount, char name[]){
	actors[actorCount] = (v *)malloc(sizeof(v));
	strcpy(actors[actorCount]->name, name);
	actors[actorCount]->index = actorCount;
	actors[actorCount]->next = NULL;
	actors[actorCount]->parent = NULL;
}

void addConnections(v * actors[], int actorCount, int index, char currentLine[], int movieNo){
	n * tmp = NULL;
	char name[NAME_LENGTH];
	int connection;
	int i = 0;
	int j;
	
	// Skip movie name:
	while(currentLine[i] != '/'){
		i++;
	}
	///////////////////////
	
	// Go to last node of current adjacency list:
	if(actors[index]->next != NULL){
		tmp = actors[index]->next;
		while(tmp->next != NULL)
			tmp = tmp->next;
	}
	///////////////////
	
	
	// Loops once for each actor until end of the string:
	while(currentLine[i] != '\n'){
		
		i++;	// Current char is '/', skip it by increasing i.
		
		// Keep the name in the name string:
		j = 0;
		while(currentLine[i] != '/' && currentLine[i] != '\n'){
			name[j] = currentLine[i];
			i++;
			j++;
		}
		name[j] = '\0';
		///////////////////
		
		// Get the index of actor to be added to adjacency list in actors array:
		connection = findActor(actors, actorCount, name);
		
		// Add the actor to the adjacency list:
		if(connection != index){
			if(actors[index]->next == NULL){
				actors[index]->next = (n *)malloc(sizeof(n));
				tmp = actors[index]->next;
			}
			else{
				tmp->next = (n*)malloc(sizeof(n));
				tmp = tmp->next;
			}
			tmp->actorNo = connection;
			tmp->movieNo = movieNo;
			tmp->next = NULL;
		}
	}
}

int isFull(q * queue){
	if(queue->size == queue->capacity)
		return 1;
	else
		return 0;
}

int isEmpty(q * queue){
	if(queue->size == 0)
		return 1;
	else
		return 0;
}

void enqueue(q * queue, v * item){
	if(isFull(queue) == 1)
		return;
	queue->rear = (queue->rear + 1) % queue->capacity;
	queue->size = queue->size + 1;
	queue->array[queue->rear] = item;
}

v * dequeue(q * queue){
	if(isEmpty(queue) == 1)
		return NULL;
	v * item = queue->array[queue->front];
	queue->front = (queue->front + 1) % queue->capacity;
	queue->size = queue->size - 1;
	return item;
}

q * createQueue(){
	q * queue = (q *)malloc(sizeof(q));
	queue->capacity = QUEUE_SIZE;
	queue->size = 0;
	queue->front = 0;
	queue->rear = queue->capacity - 1;
	return queue;
}

v * getFront(q * queue){
	if(isEmpty(queue) == 1)
		return NULL;
	return queue->array[queue->front];
}

v * getRear(q * queue){
	if(isEmpty(queue) == 1)
		return NULL;
	return queue->array[queue->rear];
}

int findCommonMovie(v * actors[], int index1, int index2){
	if(actors[index1]->next == NULL)
		return -1;

	n * tmp = actors[index1]->next;
		
	// Loops until finding common movie or come end of the list:
	while(tmp != NULL){
		// Find actor with index2 in the adjacency list of actor with index1:
		if(tmp->actorNo == index2)
			return tmp->movieNo;	// Return common movie's number.
		tmp = tmp->next;
	}
	return -1;
}

int findBaconNumber(v * actors[], int actorCount, int index, int baconsIndex){
	
	// Bacon number is 0 (Bacon, himself):
	if(index == baconsIndex)
		return 0;
	
	// Bacon number is infinite (Not connected to any actor):
	else if(actors[index]->next == NULL)
		return -1;
	
	// Otherwise, calculate:
	q * queue = createQueue();
	v * visited[ACTORS_SIZE] = {};
	v * tmp = actors[index];
	v * found;
	n * tmp2;
	int baconNumber = 0;
	int empty = 0;
	int full = 0;
	
	// Add first item to queue:
	enqueue(queue, tmp);
	visited[tmp->index] = tmp;
	tmp->parent = NULL;
	
	// Until queue is empty, queue is full or kevin bacon found:
	while(tmp->index != baconsIndex && isEmpty(queue) != 1 && isFull(queue) != 1){

		// Get neighbors:
		if(tmp->next != NULL)
			tmp2 = tmp->next;
		else
			tmp2 = NULL;
		
		// Add neighbors to queue:
		while(tmp2 != NULL){
			// If not visited:
			if(visited[tmp2->actorNo] == NULL){
				enqueue(queue, actors[tmp2->actorNo]);
				visited[tmp2->actorNo] = actors[tmp2->actorNo];
				actors[tmp2->actorNo]->parent = tmp;
			}
			tmp2 = tmp2->next;
		}
		
		// Dequeue current item and skip to next item:
		dequeue(queue);
		
		// Get next item in queue:
		if(getFront(queue) != NULL)
			tmp = getFront(queue);
	}
	
	// No connection to Kevin Bacon:
	if(tmp->index != baconsIndex)
		return -1;
	
	// Traverse back from the path to calculate Kevin Bacon number:
	while(tmp->parent != NULL && tmp != NULL){
		baconNumber++;
		tmp = tmp->parent;
	}
	free(queue);
	return baconNumber;
}

int findBaconNumberWithPrint(v * actors[], char movie[MOVIES_SIZE][NAME_LENGTH], int actorCount, int index, int baconsIndex){
	
	// Bacon number is 0 (Bacon, himself):
	if(index == baconsIndex)
		return 0;
	
	// Bacon number is infinite (Not connected to any actor):
	else if(actors[index]->next == NULL)
		return -1;
	
	// Otherwise, calculate:
	q * queue = createQueue();
	v * visited[ACTORS_SIZE] = {};
	v * tmp = actors[index];
	v * found;
	n * tmp2;
	int baconNumber = 0;
	int empty = 0;
	int full = 0;
	int movieNo;
	
	// Add first item to queue:
	enqueue(queue, tmp);
	visited[tmp->index] = tmp;
	tmp->parent = NULL;
	
	// Until queue is empty, queue is full or kevin bacon found:
	while(tmp->index != baconsIndex && isEmpty(queue) != 1 && isFull(queue) != 1){

		// Get neighbors:
		if(tmp->next != NULL)
			tmp2 = tmp->next;
		else
			tmp2 = NULL;
		
		// Add neighbors to queue:
		while(tmp2 != NULL){
			// If not visited:
			if(visited[tmp2->actorNo] == NULL){
				enqueue(queue, actors[tmp2->actorNo]);
				visited[tmp2->actorNo] = actors[tmp2->actorNo];
				actors[tmp2->actorNo]->parent = tmp;
			}
			tmp2 = tmp2->next;
		}
		
		// Dequeue current item and skip to next item:
		dequeue(queue);
		
		// Get next item in queue:
		if(getFront(queue) != NULL)
			tmp = getFront(queue);
	}
	
	// No connection to Kevin Bacon:
	if(tmp->index != baconsIndex)
		return -1;
	
	// Traverse back from the path to calculate Kevin Bacon number with printing the path:
	while(tmp->parent != NULL && tmp != NULL){
		baconNumber++;
		movieNo = findCommonMovie(actors,tmp->index,tmp->parent->index);
		printf("%s - %s: %s.\n",tmp->name,tmp->parent->name,movie + movieNo);
		tmp = tmp->parent;
	}
	free(queue);
	return baconNumber;
}
