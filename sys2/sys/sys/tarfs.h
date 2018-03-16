#ifndef _TARFS_H
#define _TARFS_H
#include<sys/defs.h>
#include<sys/process.h>
extern char _binary_tarfs_start;
extern char _binary_tarfs_end;

struct posix_header_ustar {
  char name[100];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char checksum[8];
  char typeflag[1];
  char linkname[100];
  char magic[6];
  char version[2];
  char uname[32];
  char gname[32];
  char devmajor[8];
  char devminor[8];
  char prefix[155];
  char pad[12];
};

#define EI_NIDENT 16

typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t      e_type;
    uint16_t      e_machine;
    uint32_t      e_version;
    uint64_t      e_entry;
    uint64_t      e_phoff;
    uint64_t      e_shoff;
    uint32_t      e_flags;
    uint16_t      e_ehsize;
    uint16_t      e_phentsize;
    uint16_t      e_phnum;
    uint16_t      e_shentsize;
    uint16_t      e_shnum;
    uint16_t      e_shstrndx;
} Elf_hdr;

typedef struct {
    uint32_t   p_type;
    uint32_t   p_flags;
    uint64_t   p_offset;
    uint64_t   p_vaddr;
    uint64_t   p_paddr;
    uint64_t   p_filesz;
    uint64_t   p_memsz;
    uint64_t   p_align;
} Elf64_Phdr;

/*
typedef struct {
    int inode_num;
    char filename[100];
    int perm;
    int size;
    char type;
    int sector_loc[10];
    int offset;
    uint64_t address;
    int is_fs;
}file;

file *fd[50];
*/

void findheaders(char *_binary_tarfs_start,char *_binary_tarfs_end);
int writeto(int first,char *result,int third);
void check(char *filename);
uint64_t is_exists(char *);
void tarfsinit();
uint64_t fun1(char *file);
int open(char *file1,int flags);
int close (int i);
int read(int, char *buffer, int limit);
uint64_t opendir(char *dirname);
uint64_t closedir(uint64_t address);
uint64_t readdir(uint64_t address);
void loadUser(struct task_struct *,char * ,char **);


#endif
