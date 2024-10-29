#include <unistd.h>
#include <ctype.h>

int main() {
    char buffer[256];
    int bytes_read;
    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == ' ') {
                buffer[i] = '_';
            }
        }
        write(STDOUT_FILENO, buffer, bytes_read);
    }
    return 0;
}
