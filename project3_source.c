// Fundamental Data Structures Proj3, Lytze
// Reverse hashing

# include <stdio.h>
# include <stdlib.h>

# define MAX_TAB	100000
# define DEBUG 		0

/////////////////////////////////////////////////////////////////////////////////////// DEFINE DATA TYPES
typedef struct tabelement * TabElement;
struct tabelement {
	long ind;	// the original INDex of the element, important for sorting
	long ele;	// the table ELEment
	long dep;	// the farest DEPendancy index of the element
};
typedef struct hashtab * HashTab;
struct hashtab {
	TabElement 	list[MAX_TAB];	// the list of table elements address
	long 		len;			// the table size
};
/////////////////////////////////////////////////////////////////////////////////////// END

/////////////////////////////////////////////////////////////////////////////////////// DECLARE FUNCTIONS
void 	find_dependency(HashTab table);				// find the dependency of each element in the table
int 	dependent(TabElement e1, TabElement e2);	// check if e2 is dependent on e1
int 	greater(TabElement e1, TabElement e2);		// compare the priority of two keys
void 	depsort(HashTab table);						// dependency sort
/////////////////////////////////////////////////////////////////////////////////////// END

/////////////////////////////////////////////////////////////////////////////////////// FUNCTIONS
void 	find_dependency(HashTab table) {
	long rep[MAX_TAB]; // record the replacement of each element in the table
	long i;
	long len = table -> len;

	// first scan the table and record the replacements
	i = 0;
	while (i < len) {
		if (table -> list[i]) {
			rep[i] = i - table -> list[i] -> ele % len;
			if (rep[i] < 0) rep[i] += len;
		}
		i++;
	}

	// then calculate the dependencies
	i = 0;
	while (i < table -> len) {
		if (table -> list[i]) {
			// if the cell is not empty, trace to the larest index
			int far = rep[i];
			int cur = 1;
			while (cur <= far) {
				// scan all cells till reach the farest dependency
				int prob = i - cur;
				if (prob < 0) prob = prob + len;
				// make sure the index in in range
				if (cur + rep[prob] > far) far = cur + rep[prob];
				// lengthen the farest dependency length
				cur++;
			}
			// now the far value is the farest dependency length
			// translat it into a index
			table -> list[i] -> dep = i - far;
			if (table -> list[i] -> dep < 0) table -> list[i] -> dep += len;
		}
		i++;
	}

	return;
}	// find the dependency of each element in the table

int dependent(TabElement e1, TabElement e2) {
	int rt = 0;
	if (!e1 || !e2) rt = 0; // not relevent if any of elements is NULL
	else if ((e2 -> ind > e2 -> dep && (e1 -> ind < e2 -> ind && e1 -> ind >= e2 -> dep)) ||
		(e2 -> ind < e2 -> dep && !(e1 -> ind > e2 -> dep && e1 -> ind < e2 -> dep)))
		rt = 1;
	return rt;
}	// check if e2 is denpendent on e1

int greater(TabElement e1, TabElement e2) {
	int rt;

	if (!e1) rt = 0;
	else if (!e2) rt = 1;
	// test if any of the element is removed
	else {
		if (e1 -> ele < e2 -> ele) rt = 1;
		else rt = 0;
	}

	return rt;
}	// compareing two elements, return 1 if element 1 is 'greater'

void depsort(HashTab table) {
	long i, j;
	i = 1;
	while (i < (table -> len)) {
		int rel = 0; // relavent mark
		int dorel = 0; // do relavent exist mark
		j = 0;
		while (j < i) {
			if (dependent(table -> list[j], table -> list[i])) {
				rel = j;
				dorel = 1;
			}
			// if find any dependency, mark the latest
			j++;
		}

		if (dorel) j = rel + 1; // start from last relavent element
		else j = 0; // otherwise start form head
		while (j < i) {
			if (greater(table -> list[i], table -> list[j])) {
				break;
			}
			j++;
		}
		// find the first greater number j
		TabElement temp = table -> list[i];
		int from = i;
		while (from > j) {
			table -> list[from] = table -> list[from - 1];
			from--;
			// replace one by one
		}
		table -> list[from] = temp;

		if (DEBUG) {j = 0;
			while (j < table -> len) {
				if (table -> list[j])
					printf("[ %ld\t%ld\t%ld ]\n", (table -> list[j]) -> ind, (table -> list[j]) -> ele, (table -> list[j]) -> dep);
				else printf("[ /\t/\t/ ]\n");
				if (i == j) printf("--------------\n");
				j++;
			}
			printf("\n");
		}

		i++;
	}
	return;
}	// dependent sort, based on insertion sort
/////////////////////////////////////////////////////////////////////////////////////// END

/////////////////////////////////////////////////////////////////////////////////////// BODY
int main(void){
	long i, n, sub;
	HashTab table;

	scanf("%ld", &n);

	table = (HashTab)malloc(sizeof(struct hashtab));

	// read the table
	i = 0;
	while (i < n) {
		table -> list[i] = (TabElement)malloc(sizeof(struct tabelement));
		scanf("%ld", &((table -> list[i]) -> ele));
		(table -> list[i]) -> ind = i;
		i++;
	}
	table -> len = n;

	// clear the minus values
	i = 0;
	sub = 0; // count the removal number
	while (i < n) {
		if (table -> list[i] -> ele < 0) {
			free(table -> list[i]);
			table -> list[i] = NULL;
			sub++; // increse removal number
		}
		i++;
	}

	// calculate the farest relevent dependency
	find_dependency(table);

	// dependency sort
	depsort(table);

	// print the result
	i = 0;
	while (i < n - sub) {
		printf("%ld", (table -> list[i]) -> ele);
		i++;
		if (i < n - sub) printf(" ");
	}
	printf("\n");

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////// END