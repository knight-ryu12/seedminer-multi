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
int main() {
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
