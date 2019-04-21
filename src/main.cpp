/*
 *  cfly
 *  Baragouine
 *  04/21/2019 at 12:10
 *  Open source
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

#define VERSION_MAJ (0)
#define VERSION_MIN (1)
#define VERSION_REV (0)
#define VERSION_STR "0.1.0"

//  Print help
void            print_help();

//  convert argc_, argv to vector<string>
vector<string>  pack_arg(int argc_, char** argv_);

//  Return true if program arguments is valid
bool            is_valid_arg(vector<string> const& args_);

//  Convert file to array
int             convert_file_to_array(vector<string> const& args_);

//  cfy <SRC> [-a <ARRAY_NAME>] [-o <FILE>]
int             main(int argc_, char** argv_)
{
    vector<string> const    arg = pack_arg(argc_, argv_);

    //  Verify arguments
    if (!is_valid_arg(arg))
    {
        cout << "Error: Invalid parameter" << endl;
        print_help();
        return (1);
    }

    //  exec action
    return (convert_file_to_array(arg));
}

void            print_help()
{
    cout
        << "cfy " << VERSION_STR << "\n"
        << "\n"
        << "COMMAND\n"
        << "    cfy <SRC> [-a <ARRAY_NAME>] [-o <FILE>]\n"
        << "\n"
        << "DESCRIPTION\n"
        << "    SRC         File to convert on array c langage\n"
        << "    ARRAY_NAME  Name of array\n"
        << "    FILE        Name of ouput file\n"
        << endl;
}

vector<string>  pack_arg(int argc_, char** argv_)
{
    vector<string>  res;

    for (int i = 0; i < argc_; ++i)
        res.push_back(string(argv_[i]));
    return (res);
}

bool    is_valid_arg(vector<string> const& args_)
{
    if (args_.size() == 2)
        return (true);
    if (args_.size() == 4 && (args_[2] == "-o" || args_[2] == "-a"))
        return (true);
    if (args_.size() == 6 && (
        (args_[2] == "-o" && args_[4] == "-a") ||
        (args_[2] == "-a" && args_[4] == "-o"))
    )
    {   return true;}

    return (false);
}

//  Try to deduct array name from parameter or generate random name
string          deduct_array_name(vector<string> const& args_);

//  Try to deduct output file name
string          deduct_output_file_name(vector<string> const& args_);

int            convert_file_to_array(vector<string> const& args_)
{
    string const    src = (args_.size() >= 2 ? args_[1] : string(""));
    string const    array_name = deduct_array_name(args_);
    string const    output_file_name = deduct_output_file_name(args_);
    ifstream        ifs(src);

    if (ifs.is_open())
    {
        ofstream        ofs(output_file_name);

        if (ofs.is_open())
        {
            long long   i = 0;

            ofs << "char unsigned const  "<< array_name << "[] = {\n";
            while (ifs)
            {
                int const   byte = +ifs.get();

                if (i != 0)
                    ofs << ",";

                if (i && (i % 16) == 0)
                    ofs << "\n";
                else if (i != 0)
                    ofs << " ";

                if (byte < 10)
                    ofs << "  ";
                else if (byte < 100)
                    ofs << " ";

                ofs << byte;
                ++i;
            }
            if (i % 16)
                ofs << "\n";
            ofs << "};\n";
            ofs.close();
        }
        else
            cout << "Error: Can't open " << output_file_name << endl;

        ifs.close();
    }
    else
        cout << "Error: Can't open " << src << endl;
    return (0);
}

string          deduct_array_name(vector<string> const& args_)
{
    if (args_.size() == 4 && args_[2] == "-a")
        return (args_[3]);
    if (args_.size() == 6 && args_[2] == "-a")
        return (args_[3]);
    if (args_.size() == 6 && args_[4] == "-a")
        return (args_[5]);

    return ("array_" + to_string(time(nullptr) * 100 + rand() % 100));
}

string          deduct_output_file_name(vector<string> const& args_)
{
    if (args_.size() == 4 && args_[2] == "-o")
        return (args_[3]);
    if (args_.size() == 6 && args_[2] == "-o")
        return (args_[3]);
    if (args_.size() == 6 && args_[4] == "-o")
        return (args_[5]);

    return ("array_" + to_string(time(nullptr) * 100 + rand() % 100) + ".c");    
}
