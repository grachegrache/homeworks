/*
 * mysort.c
 *
 *  Created on: 2015. 11. 8.
 *      Author: panda
 */

#include "headers/mysort.h"

void mysort(char* arg){
	if(strcmp(arg, "--help") == 0){
		print_man("mysort");
		return;
	}
	int res;
	record* rec = (record*) malloc(sizeof(record));

	res = init_file(arg, rec);
	if(res == -1){
		printf("error:can not open the file!\n");
		print_man("mysort");
		return;

	}else if(res == -2){
		printf("error:can not read any character!\n");
		return;
	}

	merge_sort(rec, res);
	rewrite(arg, rec);
	free_rec(rec, 1);
}

int init_file(char* filename, record* rec){
	int fd = open(filename, O_RDONLY);
	if(fd == -1)
		return -1;

	int cnt, is_enter;
	char buf[1];
	record* tmp1 = rec;
	str* tmp2;
	tmp1->next = 0;
	cnt = 0;
	is_enter = 0;

	//first read
	if(read(fd, buf, 1) > 0){
		tmp1->id = (str*) malloc(sizeof(str));
		tmp2 = tmp1->id;
		tmp2->ch = buf[0];
		tmp2->next = 0;

		cnt++;
	}else{
		close(fd);
		return -2;
	}

	while(read(fd, buf, 1) > 0){
		if(buf[0] == '\n'){
			is_enter = 1;
			tmp2->next = (str*) malloc(sizeof(str));
			tmp2 = tmp2->next;
			tmp2->ch = buf[0];
			tmp2->next = 0;

		}else if(is_enter){
			//first character after the enter
			is_enter = 0;
			tmp1->next = (record*) malloc(sizeof(record));
			tmp1 = tmp1->next;
			tmp1->id = (str*) malloc(sizeof(str));
			tmp2 = tmp1->id;
			tmp2->ch = buf[0];
			tmp2->next = 0;
			tmp1->next = 0;

			cnt++;
		}else{
			tmp2->next = (str*) malloc(sizeof(str));
			tmp2 = tmp2->next;
			tmp2->ch = buf[0];
			tmp2->next = 0;
		}

	}

	close(fd);
	return cnt;
}

void merge_sort(record* rec, int n){
	int i,j,half;
	record* tmp = rec;
	if(tmp == 0 || n == 1)
		return;

	half = n/2;

	merge_sort(rec, half);
	merge_sort(search(rec, half), n-half);

	record* s;
	record* s_tmp;
	record* tmp_i;
	record* tmp_j;
	s = (record*) malloc(sizeof(record));
	s->next = 0;
	s_tmp = s;
	i=0;
	j=half;

	tmp_i = search(tmp,i);
	tmp_j = search(tmp,j);
	if(compare(tmp_i, tmp_j) < 0){
		//duplicate tmp_i to s
		s->id = tmp_i->id;
		i++;
	}else{
		s->id = tmp_j->id;
		j++;
	}

	for(;i<half && j<n;){
		tmp_i = search(tmp,i);
		tmp_j = search(tmp,j);
//		if(tmp_i != 0 && tmp_j != 0){
			s->next = (record*) malloc(sizeof(record));
			s = s->next;
			s->next = 0;

			if(compare(tmp_i, tmp_j) < 0){
				//duplicate tmp_i to s
				s->id = tmp_i->id;
				i++;
			}else{
				//duplicate tmp_j to s
				s->id = tmp_j->id;
				j++;
			}
//		}
	}

	//duplicate remains
	while(i<half){
		s->next = (record*) malloc(sizeof(record));
		s = s->next;
		s->next = 0;
		s->id = search(tmp, i)->id;
		i++;
	}
	while(j<n){
		s->next = (record*) malloc(sizeof(record));
		s = s->next;
		s->next = 0;
		s->id = search(tmp, j)->id;
		j++;
	}

	for(i=0;i<n;i++){
		tmp_i = search(tmp, i);
		tmp_j = search(s_tmp, i);
		tmp_i->id = tmp_j->id;
	}

	free_rec(s_tmp, 0);
}

void rewrite(char* filename, record* rec){
	record* tmp1 = rec;
	str* tmp2 = rec->id;
	int fd = open(filename, O_WRONLY | O_TRUNC);
	if(fd == -1){
		printf("error:can not write the file!\n");
		return;
	}

	while(tmp1 != 0){
		tmp2 = tmp1->id;

		while(tmp2 != 0){
			write(fd, &tmp2->ch, 1);
			tmp2 = tmp2->next;
		}

		tmp1 = tmp1->next;
	}

	close(fd);
}

//return record located n
record* search(record* rec, int n){
	record* tmp = rec;
	while(n-- > 0 && tmp != 0)
		tmp = tmp->next;

	return tmp;
}

int compare(record* rec1, record* rec2){
	if(rec1 == 0 && rec2 != 0)
		return -1;
	else if(rec1 != 0 && rec2 == 0)
		return 1;
	else if(rec1 == 0 && rec2 == 0)
		return 0;

	str* str1 = rec1->id;
	str* str2 = rec2->id;
	while(str1 != 0 && str2 != 0){
		if(str1->ch < str2->ch)
			return -1;
		else if(str1->ch > str2->ch)
			return 1;
		else if(str1->ch == str2->ch){
			str1 = str1->next;
			str2 = str2->next;
		}
	}

	if(str1 == 0 && str2 != 0)
		return -1;
	else if(str1 != 0 && str2 == 0)
		return 1;
	else if(str1 == 0 && str2 == 0)
		return 0;

	return 0;
}

void free_rec(record* rec, int opt){
	if(rec->next != 0)
		free_rec(rec->next, opt);
	if(rec != 0){
		if(opt)
			free_str(rec->id);
		free(rec);
	}
}

void free_str(str* str){
	if(str->next != 0)
		free_str(str->next);
	if(str != 0)
		free(str);
}
