#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <pthread.h>
#include <openssl/sha.h>

void sha256(SHA256_CTX *c,uint8_t *s,uint32_t slen, uint8_t *buf) { // No INIT
	SHA256_Init(c);
	SHA256_Update(c,s,slen);
	SHA256_Final(buf,c);
}

void *thread(void *arg)
{
	uint8_t *arg2 = (uint8_t *)arg;
	uint8_t buf[SHA_DIGEST_LENGTH];
	SHA256_CTX c; //define only once at internal
	char *a = "1";
	sha256(&c,a,1,buf);
	for(int i =0; i < SHA256_DIGEST_LENGTH; i++) {
		printf("%02X",buf[i]);
	}
	printf("thread %d\n", *arg2);
	pthread_exit(arg2);
}


char *get_data_nodes(bool isNew, size_t *size)
{
	FILE *file;
	if(isNew) file = fopen("nodes/lfcs_new.dat");
	else file = fopen("nodes/lfcs.dat");
	//FILE *file = fopen(isNew ? "nodes/lfcs_new.dat", "nodes/lfcs.dat");
	if(file == NULL)
	{
		printf("No lfcs_new.dat/lfcs.dat found");
		return "ERROR";
	}
	fseek(file, 0L, SEEK_END);
	size_t sz = ftell(file);
	rewind(file);
	char *tmpbuf = malloc(sizeof(char*) * sz);
	if(tmpbuf == NULL) return "ERROR";
	fread(tmpbuf, sz, 1, file);
	fclose(file);
	
	*size = sz;
	return tmpbuf;
}

int main(int argc, char **argv) 
{
	if(argc != 3)
	{
		printf("Wrong usage. seedminer <ID0> <LFCS>");
		exit(1);
	}
	
	uint64_t ID0 = strtoul(argv[1], NULL, 16);
	uint64_t LFCS = strtoul(argv[1], NULL, 16);
	
	uint8_t *_LFCS = u64_to_u8(LFCS);
	
	size_t nodes_size = 0;
	char *nodes = get_data_nodes(_LFCS[4] == 2, &nodes_size); // check new-3ds flag
	if(strcmp(nodes, "ERROR") == 0) exit(2);
	
	char **nodes_info = parse_data_nodes(nodes, nodes_size);
	
	uint8_t hashword[4];
	sscanf(ID0, "%08X%08X%08X%08X", hashword[0], hashword[1], hashword[2], hashword[3]);
	
	pthread_t thread_t[8];
	void *ret;
	for(int i = 0; i < 8; i++)
	{
		if(pthread_create(&thread_t[i], NULL, thread, &i) != 0)
		{
			exit(1);
		}
		if(pthread_join(thread_t[i], &ret)!= 0)
		{	 
			exit(2);
		}
		
		printf("Thread %d returned %d\n", i, i);
	}
	return 0;
}
