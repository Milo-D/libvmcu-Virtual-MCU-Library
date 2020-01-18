/* Prompt Header */

#ifndef PROMPT_HPP
#define PROMPT_HPP

class Prompt {

public:
    Prompt(int height, int width, int y, int x);
    ~Prompt(void);

    void init(int height, int width, int y, int x);

    std::string read(void);
    void write(const std::string& data);
    void update(void);

    void destroy(void);

private:
    WINDOW *win;
};

#endif