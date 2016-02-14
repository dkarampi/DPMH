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

void traverse_list(struct listelement *head)
{
	for (struct listelement *p = head; p != NULL; p = p->next)
		;
}

#ifdef DEBUG
void print_list(struct listelement *head)
{
	int i = 0;
	for (struct listelement *p = head; p != NULL; p = p->next, ++i)
		printf("%ld ", p->padding[0]);
	printf("(%d nodes)\n", i);
}
#endif

struct listelement * create_list_fwd_pointers(int num_of_nodes)
{
	struct listelement *head = 
		malloc(num_of_nodes * sizeof(struct listelement));

	struct listelement *p = head;
	for (int i = 0; i < num_of_nodes-1; i++, p++) {
#ifdef DEBUG
		p->padding[0] = (long int) i;
#endif
		p->next = (p+1);
	}
#ifdef DEBUG
	p->padding[0] = (long int) num_of_nodes-1;
#endif
	p->next = NULL;

	return head;
}

/*
 * We adapt Knuth's shuffling algorithm, yet there might be a couple of
 * weeknesses: 1) head is always the element at the first position of the array
 * and 2) I'm not sure if we handle the corner cases (see if statement within
 * loop) correctly. Nevertheless, for a large number of nodes the following
 * function should be sufficient.
 */
struct listelement * create_list_rnd_pointers(int num_of_nodes)
{
	struct listelement *list_array = create_list_fwd_pointers(num_of_nodes);

	for (int i = num_of_nodes-1; i > 1; i--) {
		int j = rand() % i;

		/* 
		 * We cannot swap with i with j itself becuase we need a pointer to the
		 * previous element of j for that. Instead, we will swap with j->next.
		 */
		struct listelement *p = list_array[j].next;
		if (p == NULL || p == &list_array[i]) continue;
		list_array[j].next = p->next;
		struct listelement *q = list_array[i].next;
		list_array[i].next = p;
		p->next = q;
	}
	
	return &list_array[0];
}

int main(void)
{
	/* TODO: provide basic unit tests and configure options */
	/* TODO: implement reverse linked list */

	struct timeval start, end;

	struct listelement * head = create_list_fwd_pointers(10 * 1024 * 1024 / 64);
	/* Warm up the caches */
	traverse_list(head);
	/* Run experiment */
	gettimeofday(&start, NULL);
	for (int i = 0; i < RUNS; i++)
		traverse_list(head);
	gettimeofday(&end, NULL);
	printf("%ld μs\n", ((end.tv_sec * 1000000 + end.tv_usec)
			- (start.tv_sec * 1000000 + start.tv_usec)));
	free(head);

	head = create_list_rnd_pointers(10 * 1024 * 1024 / 64);
	/* Warm up the caches */
	traverse_list(head);
	/* Run experiment */
	gettimeofday(&start, NULL);
	for (int i = 0; i < RUNS; i++)
		traverse_list(head);
	gettimeofday(&end, NULL);
	printf("%ld μs\n", ((end.tv_sec * 1000000 + end.tv_usec)
			- (start.tv_sec * 1000000 + start.tv_usec)));
	free(head);

	return 0;
}
