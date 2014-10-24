// Fundamental Data Structures Proj2, Lytze
// Richest in the World

# include <stdio.h>

# define	MAX_RECORD	100000				// Maxium length for a list of records
# define	MAX_QUERY	1000 				// Maxium query length
# define	MAX_ANSWER 	100 				// Maxium answer list length
# define	MAX_NAME	8					// Maxium length for a name
# define 	MAX_WORTH	1000000				// Maxium worth value


/////////////////////////////////////////////////////////////////////////////////////// DEFINE DATA TYPES
typedef struct record *Record;
struct record {
	char	name[MAX_NAME + 1];
	int		age;
	long	worth;
};
typedef struct record_list RecordList;
struct record_list {
	Record 	list[MAX_RECORD + 1];			// -> list of data records, heap, start from index 1
	long	len;
};
/* RecordList -> (list[index] -> data; len = length) */

typedef struct query *Query;
struct query {
	int		resid;							// -> rest required output number
	int		lower;							// -> lower age limit
	int		upper;							// -> upper age limit
};
typedef struct query_list *QueryList;
struct query_list {
	Query 	list[MAX_QUERY + 1];			// -> list of query cases
	int		len;
};
/* QueryList -> (list[index] -> cases; len = length) */

typedef struct answer *AnswerList;
struct answer {
	Record 	list[MAX_ANSWER + 1];
	int 	len;
};
/* AnswerList -> (list[index] -> data; len = length) */
/////////////////////////////////////////////////////////////////////////////////////// END OF DEFINE DATA TYPES

/////////////////////////////////////////////////////////////////////////////////////// FUNCTION DECLARATIONS
int				compare_rec(Record r1, Record r2);
/* Compare two records based on worth -> age -> name */
int 			heap_is_full(RecordList store);
void			heap_insert(RecordList store, Record current);
Record 			heap_rmtop(RecordList store);
void 			heap_pup(RecordList store, long index);
void			heap_pdown(RecordList store, long index);
/* Heap routines */
RecordList 		build_records(void);
QueryList 	 	build_queries(void);
/* Build up record list and query list */
AnswerList		feed_cases(RecordList store, QueryList quel);
void			feed_back(AnswerList ansl);
/* Build answer list and print answsr */
/////////////////////////////////////////////////////////////////////////////////////// END OF FUNCTION DECLARATIONS

/////////////////////////////////////////////////////////////////////////////////////// FUNCTIONs
int compare_rec(Record r1, Record r2) {
	// Compare 2 record according to worth > age > name
	// returen 1 if r1 is prior to r2, otherwise return 0
	int rt;

	if (!r1 || !r2) {
		perror("In compare_rec(r1, r2):\n\tinput unintialized record structure\n");
	}										// Error if any of the inputs is empty

	if (r1 -> worth > r2 -> worth) rt = 1;	// Check the worths
	else if (r1 -> worth == r2 -> worth) {
		if (r1 -> age < r2 -> age) rt = 1;	// Check the ages
		else if (r1 -> age == r2 -> age) {
			int name_p = 0;					// Check the names
			while (r1 -> name[name_p] || r2 -> name[name_p]) {
				if (!r2 -> name[name_p]) rt = 1;
				else if (!r2 -> name[name_p]) rt = 0;
				else {
					if (r1 -> name[name_p] < r2 -> name[name_p]) rt = 1;
					else if (r1 -> name[name_p] < r2 -> name[name_p]) rt = 0;
					else name_p++;
				}
			}
			if (!r1 -> name[name_p] && !r2 -> name[name_p]) {
				perror("In compare_rec(r1, r2):\n\tidentical entries found within input records\n");
			}								// End of check the name, error if identical, prior = alphabetic
		}
		else rt = 0;						// End of heck the age, prior = younger
	}
	else rt = 0;							// End of check the worth, prior = richer

	return rt;
}

int heap_is_full(RecordList store) {
	// Check if the given heap (RecordList store -> list) is full
	// return 1 if the heap is full
	int rt = 0;

	if (store -> len >= MAX_RECORD) {
		rt = 1;
	}										// RecordList store -> list is heap, index start from 1

	return rt;
}

void heap_insert(RecordList store, Record current) {
	// Insert a new record in a RecordList
	// Call heap_pup(store, index)
	if (heap_is_full(store)) {
		perror("In heap_insert(store, current):\n\theap flow over\n");
	}										// Cast error if the storage heap is full

	store -> len++;							// Lengthen the heap
	store -> list[len] = current;			// Append new record to the list
	heap_pup(store, store -> len);			// Percolate up the fit the heap

	return;
}

Record heap_rmtop(RecordList store) {
	// Remove the top member of the heap (RecordList store -> list)
	// Call heap_pdown(store, index)
	// Returen the removed entry if not empty
	Record rt;
	if (store -> len == 0) {
		perror("In heap_rmtop(store):\n\theap is empty, cannot remove the top element\n");
	}										// Error if heap is empty

	rt = store -> list[1];					// Let return the top element
	store -> list[1] = store -> list[store -> len];	// Move the bottom to the top
	store -> len--;							// Shorten the heap
	heap_pdown(store, 1);					// Percolate down the fit the heap

	return rt;
}

void heap_pup(RecordList store, long index) {
	// Percolate up funtion on a guard-free max-heap
	// Heap: RecordList store -> list of length -> len
	Record current = store -> list[index];
	while (compare_rec(current, store -> list[index / 2]) && !index == 1) {
		// While the current Record is prior and the index is not to the top
		store -> list[index] = store ->list[index / 2];
		index /= 2;
	}
	store -> list[index] = current;			// When the shuffle is down, set the current Record

	return;
}

void heap_pdown(RecordList store, long index) {
	// Percolate down function on guard-free max-heap
	// Heap: RecordList store -> list of length -> len
	Record current = store -> list[index];
	Record next;							// The selected chiled
	int r = 0; 								// If the right child is selected

	if (store -> len != 1) {
		// If the heap is singular, just return
		next = store -> list[index * 2];
		if (store -> len >= 3) {
			// If the heap is of length 2, next is the only child
			// If the length is >= 3, go on
			if (compare_rec(store -> list[index * 2 + 1], next)) {
				next = store -> list[index * 2 + 1];
				r = 1;						// The right child is selected
			}								// Set next Record to the prior most children
		}

		while (compare_rec(next, current) && index * 2 <= store -> len) {
			// While the next Record is prior and the current Record has any child
			store -> list[index] = next;
			index = index * 2 + r;			// If the right child is selected, r is 1

			r = 0;							// Initialize r
			if (index * 2 <= store -> len) {
				next = store -> list[index * 2];
				if (index * 2 + 1 <= store -> len && 
					compare_rec(store -> list[index * 2 + 1], next)) {
					next = store -> list[index * 2 + 1];
				}
			}								// Find proper valid next Record
		}
		store -> list[index] = current;		// When the shuffle is down, set the current Record
	} 
	
	return;
}
/////////////////////////////////////////////////////////////////////////////////////// END OF FUNTIONS

/////////////////////////////////////////////////////////////////////////////////////// BODY
int main(void) {
	RecordList 	store;
	QueryList 	quel;
	AnswerList 	ansl;

	store = build_records();
	quel = build_queries();
	ansl = feed_cases();
	feed_back();

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////// END OF BODY