#include <fcntl.h>
#include <unistd.h>
#include <bitset>
#include <fstream>
#include <iostream>

struct js_event {
    uint32_t time;
    int32_t value;
    uint8_t type;
    uint8_t number;
};

int main() {
    int fd = open("/dev/input/js0", O_RDONLY);

    struct js_event e;

    while (true) {
        read(fd, &e, sizeof(e));
        std::cout << std::bitset<11>(e.time) << "\t" << e.value << "\t"
                  << (int)e.type << std::endl;
        usleep(10 * 1000);
    }

    return 0;
}