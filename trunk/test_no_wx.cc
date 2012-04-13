#include <fstream>

int main()
{
    std::ofstream* f = new std::ofstream("test_no_wx.log");
    
    (*f) << "hello" << std::endl;
    (*f) << "this is just a test" << std::endl;
    (*f) << "we write to log file from main function" << std::endl;

    f->close();

    delete f;

    return 0;
}            
