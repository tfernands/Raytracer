#ifndef PROGRESSBAR_PROGRESSBAR_HPP
#define PROGRESSBAR_PROGRESSBAR_HPP

#include <chrono>
#include <iostream>
#include <stdlib.h>

class ProgressBar {
private:
    unsigned int ticks = 0;
    unsigned int buffer_size;
    char* buffer;
    const unsigned int total_ticks;
    const unsigned int bar_width;
    const char complete_char = '=';
    const char incomplete_char = ' ';
    const std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

    void update() const {
        float progress = (float) ticks / total_ticks;
        unsigned int pos = (unsigned int) (bar_width * progress);
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now-start_time).count();
        buffer[0] = '[';
        unsigned int i = 1;
        for (i = 1; i < bar_width; ++i) {
            if (i < pos) buffer[i] = complete_char;
            else if (i == pos) buffer[i] = '>';
            else buffer[i] = incomplete_char;
        }
        buffer[i] = '\0';
        sprintf(buffer, "%s] %d%% %.2f %s",buffer,int(progress * 100.0), float(time_elapsed)/1000.0,"s");
    }

public:
    ProgressBar(unsigned int total, unsigned int width, char complete, char incomplete) :
            total_ticks {total}, bar_width {width}, complete_char {complete}, incomplete_char {incomplete} {
                buffer_size = bar_width+20;
                buffer = (char*)malloc(buffer_size*sizeof(char));
                update();
            }

    ~ProgressBar(){
        delete []buffer;
    }

    void set_index(unsigned int index){
        ticks = index;
    }

    unsigned int operator++(){
        ++ticks;
        update();
        return ticks;
    }

    char* get_buffer() const{
        return buffer;
    }
};

#endif //PROGRESSBAR_PROGRESSBAR_HPP
