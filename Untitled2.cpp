#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CACHE_SIZE 100

// Structure representing a file
typedef struct {
    char path[100];
    char contents[1000];
} File;

// Structure representing the file system
typedef struct {
    File files[CACHE_SIZE];
    int cache_index;
} FileSystem;

// Function to simulate disk read operation
void disk_read(const char *file_path, char *file_contents) {
    // Simulate disk read operation
    usleep(100000); // Simulate delay in microseconds (100 milliseconds)
    sprintf(file_contents, "Contents of file '%s'", file_path);
}

// Function to read a file from the file system with caching
char *read_file(FileSystem *fs, const char *file_path) {
    int i;
    char *file_contents = NULL;

    // Check if file is in cache
    for (i = 0; i < fs->cache_index; ++i) {
        if (strcmp(fs->files[i].path, file_path) == 0) {
            file_contents = fs->files[i].contents;
            break;
        }
    }

    // If file not found in cache, read from disk
    if (file_contents == NULL) {
        disk_read(file_path, fs->files[fs->cache_index].contents);
        strncpy(fs->files[fs->cache_index].path, file_path, sizeof(fs->files[fs->cache_index].path) - 1);
        fs->files[fs->cache_index].path[sizeof(fs->files[fs->cache_index].path) - 1] = '\0';
        file_contents = fs->files[fs->cache_index].contents;
        fs->cache_index++;
    }

    return file_contents;
}

// Function to clear the cache
void clear_cache(FileSystem *fs) {
    fs->cache_index = 0;
}

int main() {
    FileSystem fs;
    fs.cache_index = 0;

    // Example usage
    char *content1 = read_file(&fs, "/path/to/file1");
    printf("File 1 Contents: %s\n", content1);

    char *content2 = read_file(&fs, "/path/to/file2");
    printf("File 2 Contents: %s\n", content2);

    char *content3 = read_file(&fs, "/path/to/file1");
    printf("File 1 Contents (cached): %s\n", content3);

    // Clear cache
    clear_cache(&fs);

    return 0;
}
