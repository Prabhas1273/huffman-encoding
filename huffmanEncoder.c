/*
File Name: huffmanEncoder.c
Description: Program to generate/ print huffman codes for each distinct character in a data stream
Author: Prabhas Reddy M.
Date: Tuesday 02 July 2024 03:15:12 PM 
*/

// Include the necessary headers
#include <stdio.h>
#include <stdlib.h>

// struct MinHeapNode contains the following fields:
// data: character
// freq: frequency of character
// left, right: pointers to left and right nodes of a <MinHeapNode> node 
struct MinHeapNode {
	char data;
	int freq;
	struct MinHeapNode *left, *right;
};

// struct MinHeap contains the following fields:
// size: number of root nodes present in Huffman Tree at a given point
// capacity: maximum number of root nodes possible
// nodeArray: array of pointers to <MinHeapNode> nodes
struct MinHeap {
	int size;
	int capacity;
	struct MinHeapNode** nodeArray;
};

// newMinHeapNode(): returns pointer to memory, allocated to min heap node 
struct MinHeapNode* newMinHeapNode(char data, int freq) {
	struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
	temp->data = data;
	temp->freq = freq;
	temp->left = temp->right = NULL;
	
	return temp;
}

// createMinHeap(): allocates memory to <MinHeap> structure instance
struct MinHeap* createMinHeap(int size) {
	struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
	minHeap->size = 0;
	minHeap->capacity = size;
	// array of pointers to min heap nodes of size capacity
	minHeap->nodeArray = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
	
	return minHeap;	
}

// swapMinHeapNodes(): utility function to swap two nodes using double pointers
void swapMinHeapNodes(struct MinHeapNode** node1, struct MinHeapNode** node2) {
	struct MinHeapNode* temp = *node1;
	*node1 = *node2;
	*node2 = temp;
}

// minHeapify(): ensures that the property of min heap is satisfied (heapify)
void minHeapify(struct MinHeap* minHeap, int idx) {
	int min = idx;
	/*
				 (i)
				parent
			   /      \
			  /        \
		left child  right child
		 (2*i+1)      (2*i+2)
	*/
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;
	
	if(left < minHeap->size 
	&& minHeap->nodeArray[left]->freq < minHeap->nodeArray[min]->freq) {
		min = left;	
	}
	
	if(right < minHeap->size 
	&& minHeap->nodeArray[right]->freq < minHeap->nodeArray[min]->freq) {
		min = right;	
	}
	
	// swap the parent node with child node having minimum frequency 
	if(min != idx) {
		swapMinHeapNodes(&minHeap->nodeArray[min], &minHeap->nodeArray[idx]);
		// repeat the process until leaf node is reached
		minHeapify(minHeap, min);
	}
}

// extractMin(): returns the peek of min heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
	// minimum at top(at 0- index)
	struct MinHeapNode* temp = minHeap->nodeArray[0];
	// replace peek with last node followed by heapifying
	minHeap->nodeArray[0] = minHeap->nodeArray[minHeap->size - 1];
	minHeap->size -= 1;
	minHeapify(minHeap, 0);	
	
	return temp;
}

// insertMinHeap(): places new node into the heap at correct position
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
	minHeap->size += 1;
	int i = minHeap->size - 1;
	
	// child node @ i => parent node @ (i-1)/2
	while(i && minHeap->nodeArray[(i-1)/2]->freq > minHeapNode->freq) {
		minHeap->nodeArray[i] = minHeap->nodeArray[(i-1)/2];
		i = (i-1)/2;
	}
	minHeap->nodeArray[i] = minHeapNode;
}

// buildMinHeap(): applies heapify process
void buildMinHeap(struct MinHeap* minHeap) {
	int n = minHeap->size - 1;
	
	for(int i = (n-1)/2; i >=0; i--) {
		minHeapify(minHeap, i);
	}
}

// createAndBuildMinHeap(): allocates memory and builds min heap from given data
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
	struct MinHeap* minHeap = createMinHeap(size);
	
	// create a node for each distinct character
	for(int i = 0; i < size; i++) {
		minHeap->nodeArray[i] = newMinHeapNode(data[i], freq[i]);
	}
	
	minHeap->size = size;
	// min heapify the nodes 
	buildMinHeap(minHeap);
	
	return minHeap;
}

// buildHuffmanTree(): creates a min heap from the given data
//                     and performs Huffman Encoding to generate Huffman Tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
	struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
	
	struct MinHeapNode *left, *right, *temp;
	
	// Iterate until the min heap contains a single root
	while(minHeap->size != 1) {
		// Extract the lowest two frequencies from the min heap
		left = extractMin(minHeap);
		right = extractMin(minHeap);
		
		// Create a new min heap node with frequency equal to 
		// sum of lowest two frequencies extracted in the above step 
		temp = newMinHeapNode('#', left->freq + right->freq);
		
		// Make the first and second lowest frequency it's left and right child respectively 
		temp->left = left;
		temp->right = right;
		
		// Insert this new node into the min heap
		insertMinHeap(minHeap, temp);
	}
	
	return extractMin(minHeap);
}

// isLeaf(): utility function that returns true if the node is a leaf node, false otherwise
bool isLeaf(struct MinHeapNode* root) {
	// A node is called leaf node if it doesn't have any children (neither left nor right)
	if(root->left == NULL && root->right == NULL) {
		return true;
	}
	return false;
}

// max(): returns maximum of two numbers
int max(int a, int b) {
	return a > b ? a : b;
}

// huffmanTreeHeight(): recursive function that returns height of Huffman Tree
int huffmanTreeHeight(struct MinHeapNode* root) {
	if(root == NULL) {
		return 0;
	}
	
	int leftSubTreeHeight = huffmanTreeHeight(root->left);
	int rightSubTreeHeight = huffmanTreeHeight(root->right);
	
	return 1 + max(leftSubTreeHeight, rightSubTreeHeight);
}

// printCodes(): generates Huffman Codes from the Huffman Tree
void printCodes(struct MinHeapNode* root, int code[], int idx) {
	// x = 0 or 1
	// assign 'x' to left branch
	if(root->left) {
		code[idx] = 0;
		printCodes(root->left, code, idx+1);
	}
	// assign '!x' to right branch
	if(root->right) {
		code[idx] = 1;
		printCodes(root->right, code, idx+1);
	}
	if(isLeaf(root)) {
		printf("%c -> ", root->data);
		for(int i=0; i<idx; i++) {
			printf("%d ", code[i]);
		}
		printf("\n");
	}
}

// getHuffmanCodes(): builds Huffman Tree and prints Huffman Codes
void getHuffmanCodes(char data[], int freq[], int size) {
	struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
	
	int treeHeight = huffmanTreeHeight(root);
	
	// container to store Huffman Code for each character
	int code[treeHeight-1];
	
	printCodes(root, code, 0);
}

// Driver code
int main() {
	// Array of distinct characters 
	char data[] = {'a', 'b', 'c', 'd', 'e', 'f'};
	// Array of frequencies corresponding to each distinct character
	int freq[] = {5, 9, 12, 13, 16, 45};
	int size = sizeof(data) / sizeof(data[0]);
	
	// Function to build Huffman Tree and print the Huffman Codes
	getHuffmanCodes(data, freq, size);
	
	return 0;
}
