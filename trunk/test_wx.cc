#include <fstream>
#include <wx/wx.h>

class deTest: public wxApp
{
    public:
        deTest()
        :wxApp()
        {
            std::ofstream* f = new std::ofstream("test_wx.log");
            
            (*f) << "hello" << std::endl;
            (*f) << "this is just a test" << std::endl;
            (*f) << "we write to log file from wxWidgets app" << std::endl;

            f->close();

            delete f;
        }

        ~deTest()
        {
        }

    	virtual bool OnInit()
        {
            wxExit();
        }
};

IMPLEMENT_APP(deTest)

