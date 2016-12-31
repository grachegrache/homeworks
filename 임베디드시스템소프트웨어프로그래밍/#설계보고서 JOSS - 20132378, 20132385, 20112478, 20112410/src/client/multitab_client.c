/*
 * multitab client.c
 *
 *  Created on: 2016. 5. 25.
 *      Author: panda
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct string {
		char* ptr;
		size_t len;
};

char* ip_address = "192.168.123.2";
char* port = "8080";

void init_string(struct string*);
ssize_t write_func(void*, size_t, size_t, struct string*);
void set_url(char**, char*, int, int);
int get_relay_value(int);
void set_relay_value(int, int);

//int main() {
//	int i, value;

//	set_relay_value(0, 1);
//	for(i = 0; i < 4; i++){
//		value = get_relay_value(i);
//		printf("%d value: %d\n", i, value);
//	}

//	return 0;
//}

int get_relay_value(int index) {
	curl_global_init (CURL_GLOBAL_ALL);
	CURL* ctx = curl_easy_init();
	
	if(ctx == NULL){
		printf("ctx null\n");
		return -1;
	}
	
	char* url;
	struct string s;
	set_url(&url, "get", index, 0);
	init_string(&s);
	printf("url: %s\n", url);
	
	curl_easy_setopt(ctx, CURLOPT_URL, url);
	curl_easy_setopt(ctx, CURLOPT_WRITEFUNCTION, write_func);
	curl_easy_setopt(ctx, CURLOPT_WRITEDATA, &s);
	
	const CURLcode rc = curl_easy_perform(ctx);
	
	if(rc != CURLE_OK){
		printf("error from curl\n");
		return -1;
	}
	
	int value = atoi(s.ptr);
	
	free(s.ptr);
	free(url);
	curl_easy_cleanup(ctx);
	curl_global_cleanup();
	
	return value;
}

void set_relay_value(int index, int value) {
	curl_global_init (CURL_GLOBAL_ALL);
	CURL* ctx = curl_easy_init();
	
	if(ctx == NULL){
		printf("ctx null\n");
		return;
	}
	
	char* url;
	set_url(&url, "set", index, value);
	printf("url:%s\n",url);

	curl_easy_setopt(ctx, CURLOPT_URL, url);

	const CURLcode rc = curl_easy_perform(ctx);

	if(rc != CURLE_OK){
		printf("error from curl\n");
		return;
	}
	
	free(url);
	curl_easy_cleanup(ctx);
	curl_global_cleanup();
}

void init_string(struct string* s) {
	s->len = 0;
	s->ptr = malloc(s->len + 1);
	
	if(s->ptr == NULL){
		printf("malloc fail\n");
		exit (EXIT_FAILURE);
	}
	
	s->ptr[0] = '\0';
	
}

void set_url(char** url, char* mode, int index, int value) {
	int total_len;
	char* serv_path = "/multitab_server/main.jsp?mode=";
	total_len = strlen(ip_address)+strlen(port)+strlen(serv_path)+strlen(mode)+2;

	if(strcmp(mode, "get") == 0){
		total_len += 8;
		*url = malloc(total_len);
		sprintf(*url, "%s%s%s%s%s%s%d", ip_address, ":", port, serv_path, mode, "&index=", index);
		
	}else{
		total_len += 16;
		*url = malloc(total_len);
		sprintf(*url, "%s%s%s%s%s%s%d%s%d", ip_address, ":", port, serv_path, mode, "&index=", index,
				"&value=", value);
	}
//	*url[total_len - 1] = '\0';
}

ssize_t write_func(void* ptr, size_t size, size_t nmemb, struct string* s) {
	size_t new_len = s->len + size * nmemb;
	s->ptr = realloc(s->ptr, new_len + 1);
	
	if(s->ptr == NULL){
		printf("realloc fail\n");
		exit (EXIT_FAILURE);
	}
	
	memcpy(s->ptr + s->len, ptr, size * nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;
	
	return size * nmemb;
}
