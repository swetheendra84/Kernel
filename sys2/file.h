#define DIRECTORY 5
#define FILE_TYPE 0

typedef struct {
    char fs_type[10];
    int size;
    int magic_no;
    int inode_start_sector;
    int data_start_sector;
    int free_inode_block[4];
    int free_data_block[1272];
} superblock;

typedef struct {
    int inode_num;
    char filename[100];
    int perm;
    int size;
    char type;
    int sector_loc[10];
    int written;
} inode;

typedef struct {
    char filename[100];
    int inode_num;
}

typedef struct {
    char inode_num;
    char name[100];
    int size;
    int typeflag;
    int par_ind;
} itable


