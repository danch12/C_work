#ifndef _common_bk_h
#define _common_bk_h

typedef enum colours {empty,red, green,yellow,blue,magenta,cyan,white,black} colours;
typedef enum bool {false, true} bool;

int len_shelf_full(int row,const nodeptr bk_container);

void* safe_calloc(size_t nitems, size_t size);

char* num_to_letter(colours c);

void bookcase_to_str(const nodeptr bk_container,char* str);

bool legal_move(int start_row,int target_row, \
            const nodeptr bk_container);

bool is_happy(int start_row,const nodeptr bk_container);

bool duplicate_colour_rows(const colours* array,int array_size);

void print_lineage(nodeptr happy_bookcase,bool verbose);

int num_of_gens(nodeptr happy_bookcase);

colours letter_to_num(char c);

bool get_sizes(FILE *fp,int* num_rows,int* num_cols);

nodeptr fill_from_file(char* filename);

bool empties_before_colour(const nodeptr bk_container);

int count_num_colours(const nodeptr bk_container);

int count_colour(const nodeptr bk_container,colours colour);

bool colour_impossibility(const nodeptr bk_container);



#endif
