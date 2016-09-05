/* 
 * my_assembler 함수를 위한 변수 선언 및 매크로를 담고 있는 헤더 파일이다. 
 * 
 */
#define MAX_INST 256
#define MAX_LINES 5000

#define MAX_COLUMNS 4
#define MAX_OPERAND 3

/* 
 * 기계어 목록 파일로 부터 정보를 받아와 생성하는 기계어 변환 테이블이다.
 * 해당 기계어의 정보를 토큰으로 나눠 기록하고 있다. 
 */
char *inst[MAX_INST][MAX_COLUMNS];
int inst_index;

/*
 * 어셈블리 할 소스코드를 토큰 단위로 관리하는 테이블이다. 
 * 관리 정보는 소스 라인 단위로 관리되어진다.
 */
char *input_data[MAX_LINES];
static int line_num;

int label_num;

struct token_unit{
		char *label;char *operator;
		char *operand[MAX_OPERAND];
		char *comment;
};

typedef struct token_unit token;
token *token_table[MAX_LINES];

static int token_line;

/*
 * 심볼을 관리하는 구조체이다.
 * 심볼 테이블은 심볼 이름, 심볼의 위치로 구성된다.
 */
struct symbol_unit{
		char symbol[10];
		int addr;
};

typedef struct symbol_unit symbol;
symbol sym_table[MAX_LINES];

static int locctr;
//--------------
#define N 2
#define I 1
#define op_X 8
#define op_B 4
#define P 2
#define E 1
#define A 0
#define reg_X 1
#define reg_B 3
#define S 4
#define T 5
#define L 2

int program_count;
int* program_length;
int last_ctr;
int len;

char* object_code[MAX_LINES];
static int code_index;

char* extdef[MAX_LINES];
char* extref[MAX_LINES];
static int* def_index;
static int* ref_index;

typedef struct{
		int program_index;
		int len;
		char op;
		char* ref;
		int addr;
} modify;

modify m_table[MAX_LINES];
static int m_index;

static char *input_file = "input.txt";
static char *output_file = "output.txt";

int init_my_assembler(void);
static int assem_pass1(void);
static int assem_pass2(void);
int init_inst_file(char *inst_file);
int init_input_file(char *input_file);
int search_opcode(char *str);
void make_objectcode(char *file_name);

void make_intercode(token* token, int* program_count);
void print_intercode();
int add_literal();
void print_literal();
int print_equal(token* token, symbol* symbol);
int search_symbol(char* symbol);
int search_extref(char* str, int program_index);
int get_reg_num(char* operand);
int get_inst_size(token* inst);
int get_byte_size(char* operand);

int is_digit(char* str);
