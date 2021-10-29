//
// Created by alex on 10/28/21.
//
#include <boost/program_options.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <string.h>

#define LOG(x) BOOST_LOG_TRIVIAL(x)

namespace po = boost::program_options;

int main(int argc, char **argv){
    // Declare the supported options.
    po::options_description
    desc("Allowed options");
    desc.add_options()
            ("help,h", "produce help message")
            ("parents,p", po::bool_switch()->default_value(false), "no error if existing, make parent directories as needed")
            ("verbose,v", po::bool_switch()->default_value(false), "print a message for each created directory")
            ("version",   po::bool_switch()->default_value(false), "output version information and exit")
            ;

    const po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    const std::vector<std::string> unrecognized = collect_unrecognized(parsed.options, po::include_positional);
    po::variables_map vm;
    po::store(parsed, vm);
    po::notify(vm);

    logging::add_console_log(std::cout,keywords::format = "%Message%");

    const auto log_level =  vm["verbose"].as<bool>() ? logging::trivial::info : logging::trivial::error;
    logging::core::get()->set_filter(logging::trivial::severity >= log_level);
    logging::add_common_attributes();


    for(const auto & opt : unrecognized){
        if(opt[0] == '-'){
            LOG(fatal) << "Unknown option '" << opt << '\'';
            return 1;
        }
    }

    if (vm.count("help") || argc <= 1) {
        LOG(fatal) << desc;
        return 1;
    }
    if (vm["version"].as<bool>()) {
        LOG(fatal) << "version 1.0";
        return 1;
    }
    std::error_code ec;
    for(const auto & dir_name : unrecognized) {
        auto path = std::filesystem::absolute(std::filesystem::path(dir_name));
            if(vm["parents"].as<bool>()) std::filesystem::create_directories(path, ec);
            else                         std::filesystem::create_directory(path, ec);
            if(errno){
                LOG(fatal) << "Cannot create directory '" << dir_name << "': " << strerror(errno);
                errno = 0;
            } else {
                LOG(info) << "Created directory '" << dir_name << '\'';
            }
    }
    return 0;
}
