	/* Mohammad Abu-Shelbaia 1200198 */
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>				
	#include <limits.h>														// INT_MAX
	#include <stdbool.h>													// To use bool, true, and false.


	/*      Defentions       */
	#define MAX_SIZE 20
	#define INF INT_MAX
	typedef char String[100];


	/*      Globals          */
	int size = 0;															// Size variable.
	int source = 0;															// Source Variable.
	int destination = 0;													// Destination Variable.
	String cities[MAX_SIZE];												// Hash Table :)
	int distances[MAX_SIZE];												// Distances from source.
	bool visited[MAX_SIZE] = {false};										// Visited Flags
	int adjacencyMatrix[MAX_SIZE][MAX_SIZE];								// Adjacenecy Matrix
	int path[MAX_SIZE];														// Path parents.


	/* 		Functions 		*/
	int getIndex(char * input); 											// Function to convert strings into indices.
	void loadFile();														// Load text file into an adjacency adjacencyMatrix.
	void dijkstra();														// DIJKSTRA.
	int getMin();															// Getting the index of the path with minimum weight which is not visited.
	void print();															// Prinitng The generated table.
	void printWithpath();											// Printing source destination with distance and path.
	void getPath(int p);													// Recursive function to get the parent.
	void save_file();														// Function to save output of function 3.


	int main() {
		bool loadFlag, sourceFlag, destinationFlag;
		char * menu = "1. Load cities.\n"
					"2. Enter source city.\n"
					"3. Enter destination city.\n"
					"4. Exit.\n";
		while(true) {
			printf("%s", menu);
			int input = 4;
			scanf("%d", &input);
			if (input == 1) {
				loadFile();
				loadFlag = true;
			}
			else if (input == 2) {
				if (loadFlag) {
					String in;
					scanf(" %[^\n]", in);
					source = getIndex(in);
					dijkstra();
					print();
					sourceFlag = true;
				}
				else printf("LoadFile First!\n");
			}
			else if (input == 3) {
				if (sourceFlag) {
					String in;
					scanf(" %[^\n]", in);
					destination = getIndex(in);
					printWithpath(destination);
					destinationFlag = true;
				}
				else printf("Enter destination First!\n");
			}
			else if (input == 4) {
				if(!destinationFlag)
					printf("Exiting without saving since you didn't choose a destination!\n");
				else save_file();
				break;
			}
			else {
				printf("Enter a valid option!\n");
			}


		}
		return 0;
	}


	void dijkstra() {
		path[source] = -1;													// source parent.
		for ( int i = 0 ; i < size ; ++i ) {								// INITIALIZING DISTANCE ARRAY.
			if ( i != source) 
				distances[i] = INF;											// ALL NODES ARE INTIALY INFINITE UNLESS THE SOURCE NODE.
		}
		int vertices = size - 1;
		while (vertices--) {
			int index = getMin();											// FIND WHICH NODE WE NEED TO WORK WITH NEXT.
			visited[index] = true;											// SET VISITED FLAG.
			for ( int vertix = 0;  vertix < size ; vertix++ ) {
				int new_distance = distances[index] + adjacencyMatrix[index][vertix];
				if (!visited[vertix] && adjacencyMatrix[index][vertix])   	// Check if the node is visted or not and check if there is a path.
					if (new_distance < distances[vertix]) {					// Check wether the new path has less distance or not
						distances[vertix] = new_distance;					// Update distance
						path[vertix] = index;								// Update parent.
					}
			}
		}
	}


	int getIndex(char * input) {
		for(int i=0; i < size; i++) {
			if(!strcasecmp(input, cities[i])) { 							// Found a matching string.
				return i;
			}
		}
		strcpy(cities[size++], input);          							// Inserting the new string.
		return size - 1;
	}


	void loadFile() {
		FILE *F = fopen("cities.txt", "r");
		String temp;
		while(fgets(temp, 100, F)){
			int i = getIndex(strtok(temp, "\t"));   						// From
			int j = getIndex(strtok(NULL, "\t"));   						// To
			char * temp3 = strtok(NULL, "\n");      						// Distance
			temp3[strlen(temp3) - 2] = '\0';        						// Remosizeing KM
			adjacencyMatrix[i][j] = atoi(temp3);              
		}
		fclose(F);
	}


	int getMin() {															// FINDING THE INDEX OF THE MINIMUM VERTICIES WEIGHT
		int min = INF, index;
		for (int i = 0; i < size; ++i) {
			if ( !visited[i] && distances[i] <= min){
				min = distances[i];
				index = i;
			}
		}
		return index;
	}


	void print() {
		printf("%-12s -> %-12s : Distance\n", "From", "To");
		for ( int i = 0 ; i < size ; ++i ) {
			printf("%-12s -> %-12s : %4d\n", cities[source], cities[i], distances[i]);
		}
	}


	void printWithpath() {
		printf("\n%-12s -> %-12s : Distance | PATH\n", "From", "To");
		printf("%-12s -> %-12s : %5d%3s | %12s%5s", cities[source], cities[destination], distances[destination], " ", cities[source], " ");
		getPath(destination);
		printf("\n");
	}


	void getPath(int p) {
		if (path[p] == -1) return;											// Reached Source.
		getPath(path[p]);													// Getting Back To Source.
		printf(" -> %6d%4s -> ", distances[p] - distances[path[p]]," ");	// Prining Source and ditance.
		printf("%12s%5s", cities[p]," ");									// Printing next city.
	}


	void getPathFile(int p, FILE *f) {
		if (path[p] == -1) return;											// Reached Source.
		getPathFile(path[p], f);											// Getting Back To Source.
		fprintf(f," -> %6d%4s -> ", distances[p] - distances[path[p]]," ");	// Prining Source and ditance.
		fprintf(f,"%12s%5s", cities[p]," ");								// Printing next city.
	}


	void printWithpathFile(FILE *f) {
		fprintf(f,"\n%-12s -> %-12s : Distance | PATH\n", "From", "To");
		fprintf(f,"%-12s -> %-12s : %5d%3s | %12s%5s", cities[source], cities[destination], distances[destination], " ", cities[source], " ");
		getPathFile(destination, f);
		fprintf(f,"\n");
	}


	void save_file() {
		FILE *F = fopen("shortest_path.txt", "w");
		printWithpathFile(F);
		fclose(F);
	}