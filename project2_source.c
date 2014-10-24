// Fundamental Data Structures Proj2, Lytze
// Richest in the World

# include <stdio.h>

# define	MAX_RECORD	100000				// Maxium length for a list of records
# define	MAX_QUERY	1000 				// Maxium query length
# define	MAX_ANSWER 	100 				// Maxium answer list length
# define	MAX_NAME	8					// Maxium length for a name

typedef struct record *Record;
struct record {
	char	name[MAX_NAME + 1];
	int		age;
	long	worth;
};
typedef struct record_list RecordList;
struct record_list {
	Record 	list[MAX_RECORD + 1];			// -> list of data records
	long	len;
};
/* RecordList -> (list[index] -> data; len = length) */

typedef struct query *Query;
struct query
{
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


int				compare_rec(Record r1, Record r2);
/* Compare two records based on worth -> age -> name */
int 			heap_is_full(RecordList store);
void			heap_insert(RecordList store, Record current);
Record 			heap_rmtop(RecordList);
void			heap_pdown(RecordList store, long index);
void 			heap_pup(RecordList store, long index);
/* Heap routines */
RecordList 		build_records(void);
QueryList 	 	build_queries(void);
/* Build up record list and query list */
AnswerList		feed_cases(RecordList store, QueryList quel);
void			feed_back(AnswerList ansl);
/* Build answer list and print answsr */

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