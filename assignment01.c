#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define RUNS	10
#define NUMPAD	7
#define CACHE_LINE	64


struct listelement {
	struct listelement *next;
	long int padding[NUMPAD];
} __attribute__ ((aligned (CACHE_LINE)));


void destroy_list(struct listelement *head)
{
	// TODO:
}


void traverse_list(struct listelement *head)
{
	while (head != NULL)
		head = head->next;

	return;
}


// for testing
void print_list(struct listelement *head)
{
	for (struct listelement *p = head; p != NULL; p = p->next)
		printf("%ld ", p->padding[0]);
	printf("\n");

	return;
}


struct listelement * create_list_fwd_pointers(int num_of_nodes)
{
	struct listelement *head = malloc(num_of_nodes * sizeof(struct listelement));

	struct listelement *p = head;
	for (int i = 0; i < num_of_nodes-1; i++, p++) {
		p->padding[0] = (long int) i; // use for debugging
		p->next = (p+1);
	}
	p->padding[0] = (long int) num_of_nodes-1; // testing
	p->next = NULL;

	return head;
}


struct listelement * create_list_rnd_pointers(int num_of_nodes)
{
	struct listelement *list_array = create_list_rnd_pointers(num_of_nodes);

	/* Knuth's Shuffle */
	for (int i = num_of_nodes-1; i > 0; i--) {
		int j = rand() % i;

		
		struct listelement *p = list_array[j].next;
		// can p be NULL ?
		list_array[j].next = p->next;	// we essentially remove node p
		struct listelement *q = listelement[i].next;
		listelement[i].next = p;
		p->next = q;
	}


	return head;
}


int main(void)
{
	printf("Size of node: %zu\n", sizeof(struct listelement));
	struct timeval start, end;

	// 64 bytes per node
	// MB: 1024 * 1024
	// struct listelement * head = create_list_fwd_pointers(1024 * 8); // 512 KB
	struct listelement * head = create_list_fwd_pointers(1024 * 16 * 32); // 1024 * 16 = 1MB
	struct listelement * p = head;

	// Testing
	struct listelement *test = create_list_rnd_pointers(7);
	print_list(test);
	return 0;

	// warm up the caches
	traverse_list(p);
	// run experiment
	gettimeofday(&start, NULL);
	for (int i = 0; i < RUNS; i++) {
		p = head;
		traverse_list(p);
	}
	gettimeofday(&end, NULL);

	printf("%ld μs\n", ((end.tv_sec * 1000000 + end.tv_usec)
			- (start.tv_sec * 1000000 + start.tv_usec)));


	return 0;
	// Random pointers
	head = create_list_rnd_pointers(1024 * 16 * 32); // 1024 * 16 = 1MB
	p = head;
	// warm up the caches
	traverse_list(p);
	// run experiment
	gettimeofday(&start, NULL);
	for (int i = 0; i < RUNS; i++) {
		p = head;
		traverse_list(p);
	}
	gettimeofday(&end, NULL);

	printf("%ld μs\n", ((end.tv_sec * 1000000 + end.tv_usec)
			- (start.tv_sec * 1000000 + start.tv_usec)));


	// TODO: free memory

	return 0;
}