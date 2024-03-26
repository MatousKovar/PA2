/**
 * @author Matouš Kovář, <kovarm46@fit.cvut.cz>
 * @date 6.5.2023
 */


#include "CApplication.h"
#include  <iostream>

int main(int argc, char **argv) {
    std::string filename;

    if (argc == 2) {
        filename = argv[1];
    }

    CApplication app(filename);
    try {
        app.Run();
        return 0;
    }
    catch (std::invalid_argument & err) {
        std::cerr << err.what() << std::endl;
    }
    catch (std::runtime_error & err) {
        std::cerr << err.what() << std::endl;
    }
    catch (std::logic_error & err) {
        std::cerr << err.what() << std::endl;
    }
}
