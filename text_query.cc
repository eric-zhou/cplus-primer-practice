#include <iostream>
#include <string>
#include <map>
#include <fstream>

class TextQuery 
{
public:
    TextQuery(const char *file = NULL);
    ~TextQuery() { };
    void show();
    bool search(std::string &expression);

private:
    typedef bool (TextQuery::*process)(std::map<int, std::string>::const_iterator it, 
        void *arg);
    void traverse(TextQuery::process func, void *arg);
    std::map<int, std::string> text;
    void build(std::ifstream &in);
    bool printItem(std::map<int, std::string>::const_iterator it, void *arg);
    bool match(std::map<int, std::string>::const_iterator it, void *arg);
};

TextQuery::TextQuery(const char *file)
{
    if (NULL == file) 
        return;

    std::ifstream infile(file);
    if (!infile) {
        std::cerr << "error: unable to open input file:" << file << std::endl;
        return;
    }

    build(infile);
}

void TextQuery::build(std::ifstream &in)
{
    std::string line;
    int number = 0;

    while (std::getline(in, line)) {
        number++;
        text[number] = line;
    }
}

void TextQuery::traverse(TextQuery::process func, void *arg)
{    
    std::map<int, std::string>::const_iterator it = text.begin();
    bool ret;

    while(ret && (it != text.end())) {
        ret = (this->*func)(it, arg);
        ++it;
    }
}

bool TextQuery::printItem(std::map<int, std::string>::const_iterator it, void *arg) 
{
    std::cout << it->first << ":" << it->second << std::endl;
    return true;
}

void TextQuery::show() 
{
    traverse(&TextQuery::printItem, NULL);
}

bool ask(std::string &expression) 
{
    std::cout << "Executed Query for:" << std::endl;
    std::cin >> expression;

    if (expression == "q")
        return false;

    return true;
}

bool TextQuery::search(std::string &expression)
{ 
    traverse(&TextQuery::match, &expression); 
}

bool TextQuery::match(std::map<int, std::string>::const_iterator it, void *arg)
{
    if (it->second.find((*(std::string *)arg)) != std::string::npos)
        printItem(it, NULL);

    return true;
}

int main(int argc, const char *argv[])
{
    std::string exp;
    TextQuery query("Alice_story");

    while (ask(exp)) {
        query.search(exp);
    }

    return 0;
}
