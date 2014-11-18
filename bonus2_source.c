/* Author: LI YUTZE;  ID: 3120000185;  No.02 */
// Data Structure Bonus Program 2 QQ Account Management

# include <stdio.h>
# include <stdlib.h>

/////////////////////////////////////////////////////// DEFINE DATA TYPES
typedef struct tabelement * TabElement;
struct tabelement {
	long long acc;
	char pw[17];
};
typedef struct hashtable * HashTable;
struct hashtable {
	TabElement * list;
	long len;
	long fill;
};
/////////////////////////////////////////////////////// END

/////////////////////////////////////////////////////// FUNCTION DECLEARATION
HashTable 	newtable(long prime);
long 		nextprime(long num);
long		hash(long long acc, long mod);
long		check(HashTable table, long long acc, char pw[17]);
void		insert(HashTable table, long long acc, char pw[17]);
void		login(HashTable table, long long acc, char pw[17]);
HashTable	expand(HashTable table);
/////////////////////////////////////////////////////// END

/////////////////////////////////////////////////////// FUNCTIONS
HashTable newtable(long prime) {
	HashTable rt;
	rt = (HashTable)malloc(sizeof(struct hashtable));
	rt -> list = (TabElement)malloc(sizeof(struct tabelement) * prime);
	rt -> len = prime;
	rt -> fill = 0;
	return rt;
} // Initialize a new hash table with given prime

long nextprime(long num) {
	long primelist[9000]; // up to 19999 there are ~8000 primes list length 9000 is ok
	long cur = 2; // current prime
	int len;
	int isprime;

	len = 1;
	primelist[0] = cur;
	do {
		cur++;
		isprime = 1;
		int i = 0;
		while (i < len) {
			if (cur % primelist[i] == 0)
				isprime = 0;
			i++;
		}
		if (isprime) {
			primelist[len] = cur;
			len++;
		}
		if (cur > num && isprime) break;
	} while (1);

	return cur;
} // Find the next prime to the given number, using a primelist

long hash(long long acc, long mod) {
	long rt;
	rt = acc % mod;
	return rt;
} // Diteral defined hash function

long check(HashTable table, long long acc, char pw[17]) {
	long index = hash(acc, table -> len);
	while (table -> list[index]) {
		if (table -> list[index] -> acc == acc) break;
		index++;
		if (index == table -> len) index = 0;
	}
	return index;
} // Check if same element exists, return the last found index, point to NULL if not found

void insert(HashTable table, long long acc, char pw[17]) {
	long index = check(table, acc, pw);

	if (table -> list[index]) {
		printf("ERROR: Account Number Already Exists\n");
	} // if found same element in the table, print arror
	else {
		TabElement new;
		new = (TabElement)malloc(sizeof(struct tabelement));
		new -> acc = acc; // save account no.
		int i = 0;
		while (pw[i]) {
			new -> pw[i] = pw[i];
			i++;
		}
		new -> pw[i] = 0; // save password
		table -> list[index] = new;

		table -> fill++;
		printf("Register Successful\n");
	} // if found no same element in the table, register new account

	return;
} // Register the account, if exists print error

void login(HashTable table, long long acc, char pw[17]) {
	long index = check(table, acc, pw);

	if (table -> list[index]) {
		int i = 0;
		int same = 1;
		while (pw[i] || table -> list[index] -> pw[i]) {
			if (pw[i] != table -> list[index] -> pw[i]) {
				same = 0;
				break;
			}
			i++;
		}

		if (same) printf("Log in Successful\n");
		else printf("ERROR: Wrong Password\n");
	} // if found same element in the table, check passwor
	else {
		printf("ERROR: Account Not Exist\n");
	} // if not found same element, print error
} // Try login account

HashTable expand(HashTable table) {
	HashTable new = newtable(nextprime(2 * (table -> len)));
	new -> fill = table -> fill;
	
	long i = 0;
	int index;
	while (i < table -> len) {
		if (table -> list[i]) {
			index = check(new, table -> list[i] -> acc, table -> list[i] -> pw);
			new -> list[index] = table -> list[i];
		}
		i++;
	}
	printf("EXPAND [%ld -> %ld] [%ld -> %ld]\n", table -> len, new -> len, table -> fill, new -> fill);
	free(table);
	return new;
} // Expand the hash table
/////////////////////////////////////////////////////// END

/////////////////////////////////////////////////////// BODY
int main(void)
{
	long n;
	scanf("%ld\n", &n);

	HashTable table;
	table = newtable(1999);

	char cmd;
	long long acc;
	char pw[17];
	while (n) {
		scanf("%c %lld %s\n", &cmd, &acc, pw);
		printf("[%c, %lld, %s]\n", cmd, acc, pw);
		switch (cmd) {
			case 'R':
				insert(table, acc, pw);
				break;
			case 'L':
				login(table, acc, pw);
				break;
		}
		if (table -> fill > (table -> len / 2))
			table = expand(table);
		n--;
	}
	return 0;
}