// Fundamental Data Structures Proj2, Lytze
// Richest in the World

# include <stdio.h>
# include <stdlib.h>

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
typedef struct record_list *RecordList;
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
	Query 	list[MAX_QUERY + 1];			// -> list of query cases, start from index 0
	int		len;
};
/* QueryList -> (list[index] -> cases; len = length) */

typedef struct answer_record_list *ARList;
struct answer_record_list {
	Record 	anslist[MAX_ANSWER + 1];		// -> answer records
	int 	anslen;							// -> length of this answer
};
typedef struct answer *AnswerList;
struct answer {
	ARList 	list[MAX_QUERY + 1];			// -> list of answer record lists
	int 	len;
};
/* AnswerList -> (list[index] -> (anslist[index] -> data; -> anslen = length); len = length) */
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
RecordList 		build_records(long n);
QueryList 	 	build_queries(long k);
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
			while (r1 -> name[name_p] && r2 -> name[name_p]) {
				if (r1 -> name[name_p] < r2 -> name[name_p]) {
					rt = 1;
					break;
				}
				else if (r1 -> name[name_p] > r2 -> name[name_p]) {
					rt = 0;
					break;
				}
				else name_p++;
			}
			if (r1 -> name[name_p] && !r2 -> name[name_p]) rt = 1;
			else if (!r1 -> name[name_p] && r2 -> name[name_p]) rt = 0;
			else if (!r1 -> name[name_p] && !r2 -> name[name_p]) {
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
	store -> list[store -> len] = current;	// Append new record to the list
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
	if (store -> len != 1) {
		store -> list[1] = store -> list[store -> len];	// Move the bottom to the top
		store -> len--;						// Shorten the heap
		heap_pdown(store, 1);				// Percolate down the fit the heap
	}
	else store -> len--;

	return rt;
}

void heap_pup(RecordList store, long index) {
	// Percolate up funtion on a guard-free max-heap
	// Heap: RecordList store -> list of length -> len
	Record current = store -> list[index];
	while (index != 1 && compare_rec(current, store -> list[index / 2])) {
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

	next = store -> list[index * 2];
	if (store -> len >= 3) {
		// If the heap is of length 2, next is the only child
		// If the length is >= 3, go on
		if (compare_rec(store -> list[index * 2 + 1], next)) {
			next = store -> list[index * 2 + 1];
			r = 1;							// The right child is selected
		}									// Set next Record to the prior most children
	}

	while (index * 2 <= store -> len && compare_rec(next, current)) {
		// While the next Record is prior and the current Record has any child
		store -> list[index] = next;
		index = index * 2 + r;				// If the right child is selected, r is 1

		r = 0;								// Initialize r
		if (index * 2 <= store -> len) {
			next = store -> list[index * 2];
			if (index * 2 + 1 <= store -> len && 
				compare_rec(store -> list[index * 2 + 1], next)) {
				next = store -> list[index * 2 + 1];
				r = 1;
			}
		}									// Find proper valid next Record
	}
	store -> list[index] = current;			// When the shuffle is down, set the current Record
	
	return;
}

RecordList build_records(long n) {
	// Read from stdin the data to be processed
	// 1st line: number of records - n, number of queries - k
	// Next n lines: name, age and worth of the nth record
	// Next k lines: number of output, and the age range of the kth query
	RecordList rt;
	rt = (RecordList)malloc(sizeof(struct record_list));
	if (!rt) {
		perror("In build_records(n):\n\tout of memory\n");
	}
	// rt -> list[0] -> worth = MAX_WORTH + 1;			// Not required
	rt -> len = 0;

	while (rt -> len < n) {
		// While current length not full
		Record newrec;
		newrec = (Record)malloc(sizeof(struct record));
		if (!newrec) {
			perror("In build_records(n):\n\tout of memory\n");
		}
		scanf("%s%d%ld", newrec -> name, &newrec -> age, &newrec -> worth);
		heap_insert(rt, newrec);			// -> len ++ in heap_insert()
	}

	return rt;
}

QueryList build_queries(long k) {
	// Read from stdin the data to be processed
	// 1st line: number of records - n, number of queries - k
	// Next n lines: name, age and worth of the nth record
	// Next k lines: number of output, and the age range of the kth query
	QueryList rt;
	rt = (QueryList)malloc(sizeof(struct query_list));
	if (!rt) {
		perror("In build_records(n):\n\tout of memory\n");
	}
	rt -> len = 0;
	while (rt -> len < k) {
		// Whille cuurent length not full
		Query newq;
		newq = (Query)malloc(sizeof(struct query));
		if (!newq) {
			perror("In build_records(n):\n\tout of memory\n");
		}
		scanf("%d%d%d", &newq -> resid, &newq -> lower, &newq -> upper);
		rt -> list[rt -> len] = newq;
		rt -> len++;
	}

	return rt;
}

AnswerList feed_cases(RecordList store, QueryList quel) {
	// Based on built heap, build answer list responsible to the query list
	// Return the AnswerList
	AnswerList rt;
	rt = (AnswerList)malloc(sizeof(struct answer));
	rt -> len = quel -> len;					// Length of answers == length of queries

	int alindex = 0;							// Answerlist index
	while (alindex < rt -> len) {
		rt -> list[alindex] = (ARList)malloc(sizeof(struct answer_record_list));
		rt -> list[alindex] -> anslen = 0;		// Each answer now let Record number to be 0
		alindex++;
	}											// Initialize AnswerList

	int dorep = 0;								// Check mark for going on shearching
	int qlindex = 0;							// Querylist index
	while (store -> len && qlindex < quel -> len) {
		// While in querylist
		if (quel -> list[qlindex] -> resid != 0) {
			dorep = 1;
			break;								// If found residual question, dorepeat
		}
		qlindex++;
	}

	while (dorep) {
		Record current;
		current = heap_rmtop(store);			// Get the prior most record

		qlindex = 0;							// Travel through querylist
		while (qlindex < quel -> len) {
			if (quel -> list[qlindex] -> resid) {
				// If the required number of answer is not reached
				if (current -> age >= quel -> list[qlindex] -> lower &&
					current -> age <= quel -> list[qlindex] -> upper) {
					// Put current Record into the AnswerList if the age is in the range
					int i = rt -> list[qlindex] -> anslen++; // i is the value before increase
					rt -> list[qlindex] -> anslist[i] = current;
					quel -> list[qlindex] -> resid--;
				}
			}
			qlindex++;
		}

		dorep = 0;
		qlindex = 0;
		while (store -> len && qlindex < quel -> len) {
			if (quel -> list[qlindex] -> resid != 0) {
				dorep = 1;
				break;							// If found residual question, dorepeat
			}
			qlindex++;
		}
	}

	return rt;
}

void feed_back(AnswerList ansl) {
	// Viva! The last function!
	// Print out the AnswerList
	int i = 0;
	while (i < ansl -> len) {
		printf("Case #%d:\n", i + 1);			// Answer for query #(i + 1)
		if (ansl -> list[i] -> anslen) {
			int j = 0;
			while (j < ansl -> list[i] -> anslen) {
				printf("%s %d %ld\n", ansl -> list[i] -> anslist[j] -> name,
										  ansl -> list[i] -> anslist[j] -> age,
										  ansl -> list[i] -> anslist[j] -> worth);
				j++;
			}	
		}
		else printf("None\n");

		i++;
	}
	return;
}
/////////////////////////////////////////////////////////////////////////////////////// END OF FUNTIONS

/////////////////////////////////////////////////////////////////////////////////////// BODY
int main(void) {
	RecordList 	store;
	QueryList 	quel;
	AnswerList 	ansl;
	long n;
	int k;
	scanf("%ld%d", &n, &k);

	store = build_records(n);				// Read Records from stdin
	quel = build_queries(k);				// Read Queries from stdin
	ansl = feed_cases(store, quel);			// Make answer list
	feed_back(ansl);						// Put the answer list Records into stdout

	long index = 0;
	while (index < store -> len) {
		index++;
		free(store -> list[index]);			// Free RecordList Records
	}
	free(store);							// Free RecordList

	index = 0;
	while (index < quel -> len) {
		free(quel -> list[index]);			// Free QueryList Queries
		index++;
	}
	free(quel);								// Free Queries

	index = 0;
	while (index < ansl -> len) {
		free(ansl -> list[index]);			// Donot need to free Records, which is all freed previously
		index++;
	}
	free(ansl);

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////// END OF BODY