#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <string>

int main (int argc, char* argv[]) {
    cxxopts::Options options("linefile", "Make sure a line is in a file");
    options.add_options()
//        ("m,match", "Allow any line matching regex", cxxopts::value<std::string>())
        ("n,newline", "First append a new line")
        ("positional", "File Line", cxxopts::value<std::vector<std::string>>())
        ("h,help", "Print help")
        ;
    options.parse_positional({"positional"});
    auto opts = options.parse(argc, argv);

    if (opts.count("help")) {
        std::cerr << options.help() << std::endl;
        return 1;
    }

    if ( (! opts.count("positional")) || (opts["positional"].as<std::vector<std::string>>().size() != 2) ) {
        std::cerr << "Error: Must specify file and line" << '\n';
        std::cerr << options.help() << std::endl;
        return 1;
    }
    std::string fileopt = opts["positional"].as<std::vector<std::string>>()[0];
    std::string lineopt = opts["positional"].as<std::vector<std::string>>()[1];

    std::string match_str;
    if (opts.count("match")) {
        match_str = opts["match"].as<std::string>();
    } else {
        match_str = lineopt;
    }

    std::fstream file;
    file.open(fileopt, std::fstream::in | std::fstream::app);
    std::string buf;
    while (std::getline(file, buf)) {
        if (buf == match_str) {
            return 0;
        }
    }

    file.clear();
    if (opts.count("newline")) {
        file << '\n';
    }
    file << lineopt << std::endl;
    return 0;
}
