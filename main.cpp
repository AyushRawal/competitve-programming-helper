#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

void usage_msg()
{
    std::cerr << "[USAGE]: "
              << "ch [CONTEST-NAME] [PROBLEM-COUNT]\n"
              << "   [OR]: "
              << "ch [FILE-NAME]\n";
    exit(-1);
}

void copy(const std::string &source, const std::string &destination)
{
    try
    {
        if (std::ifstream(destination))
        {
            std::cerr << "[ERROR] File already exits\n";
            throw "[ERROR]";
        }
        std::ifstream src(source);
        std::ofstream dest(destination);
        dest << src.rdbuf();
        if (src.is_open())
            src.close();
        if (dest.is_open())
            dest.close();
    }
    catch (...)
    {
        std::cerr << "[ERROR] Unable to read / write file\n";
        exit(-2);
    }
}

std::string get_extension(const std::string &path)
{
    size_t index = path.find_last_of(".");
    if (index == std::string::npos)
        return "";
    return path.substr(index);
}

std::string get_template_path()
{
    char *tmp = getenv("TEMPLATE_PATH");
    if (tmp == NULL)
    {
        std::cerr << "[ERROR] Template path is not set. See README to learn more.\n";
        exit(-1);
    }
    return std::string(tmp);
}

int get_problem_count(const std::string &str)
{
    int value;
    try
    {
        value = stoi(str);
        if (value < 0)
            throw "[ERROR]";
    }
    catch (...)
    {
        std::cerr << "[ERROR] Invalid input\n"
                  << "[ERROR] Problem count must be a non-negative integer\n";
        usage_msg();
    }
    return value;
}

void create_directory(const char *name)
{
    if (mkdir(name, 0777) == -1)
    {
        std::cerr << "[ERROR] Unable to create directory " << name << "\n";
        exit(-2);
    }
}

int main(int argc, char *argv[])
{
    std::string TEMPLATE_PATH = get_template_path();
    std::string ext = get_extension(TEMPLATE_PATH);
    int problem_count = 0;
    std::string filename;
    switch (argc)
    {
    case 2:
        filename = std::string(argv[1]) + ext;
        copy(TEMPLATE_PATH, filename);
        std::cout << "[STATUS] Created file " << filename << "\n";
        break;

    case 3:
        problem_count = get_problem_count(std::string(argv[2]));
        create_directory(argv[1]);
        std::cout << "[STATUS] Created directory " << argv[1] << "\n";
        for (int i = 0; i < problem_count; i++)
        {
            filename = std::to_string(i + 1) + ext;
            copy(TEMPLATE_PATH, std::string(argv[1]) + "/" + filename);
            std::cout << "[STATUS] Created file " << filename << "\n";
        }
        break;

    default:
        usage_msg();
    }
    return 0;
}
