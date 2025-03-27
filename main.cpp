#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

const string HOME_DIR = string(getenv("HOME"));
const string INPUT_DIR = HOME_DIR + "/Documents/PdfCompresser/input";
const string OUTPUT_DIR = HOME_DIR + "/Documents/PdfCompresser/output";
const string ARCHIVE_DIR = HOME_DIR + "/Documents/PdfCompresser/Archive";
const string ROOT_DIR = HOME_DIR + "/Documents/PdfCompresser";

void makeDir() {
    string command = "mkdir -p " + ROOT_DIR + " " + INPUT_DIR + " " + OUTPUT_DIR + " " + ARCHIVE_DIR;
    system(command.c_str());
}

void moveFile(const string& filePath) {
    string command = "mv \"" + filePath + "\" " + ARCHIVE_DIR;
    cout << "Moving File to Archive" << endl;
    system(command.c_str());
}

void compress(string mode) {
    for (const auto& entry : fs::directory_iterator(INPUT_DIR)) {
        if (entry.path().extension() == ".pdf") {
            string inputFile = entry.path();
            string fileName = entry.path().filename();
            string outputFile = OUTPUT_DIR + "/" + fileName;

            string command = "gs -sDEVICE=pdfwrite -dCompatibilityLevel=1.4 "
                              "-dPDFSETTINGS=/" + mode + " -dNOPAUSE -dBATCH "
                              "-dEmbedAllFonts=false -dSubsetFonts=true -dCompressFonts=true "
                              "-sOutputFile=\"" + outputFile + "\" \"" + inputFile + "\"";

            int result = system(command.c_str());

            if (result == 0) {
                cout << "Succed! " << fileName << " Your file is in: " << outputFile << endl;
                moveFile(inputFile); 
            } else {
                cerr << "Failed Prossesing " << fileName << "!" << endl;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if( !fs::exists(ROOT_DIR)){
        makeDir();
        cout << "Dir Created\n"<<endl;
    }

    if (argc < 2) {
        cout << "pdfcomp <mode>" << endl;
        cout << "   - Mode : --screen(-s), --ebook(-e), --printer(-p), --prepress(-pr), --default(-d)" << endl;
        cout << "How to Use: " << endl;
        cout << "   - Import your Files you want to compress to "+ INPUT_DIR << endl;
        cout << "   - Run the program"<< endl;
        cout << "   - Your Compressed file is in "+ OUTPUT_DIR<< endl;
        cout << "   - And your original wil be move Files in " + ARCHIVE_DIR<< endl;
        return 1;
    }

    string command = argv[1];

    if (command == "-s" || command == "--screen") {
        compress("screen");
    } else if (command == "-e" || command == "--ebook") {
        compress("ebook");
    } else if (command == "-p" || command == "--printer") {
        compress("printer");
    } else if (command == "-pr" || command == "--prepress") {
        compress("prepress");
    } else if (command == "-d" || command == "--default") {
        compress("default");
    } else {
        cerr << "Command :"+ command + " Not Found" << endl;
        return 1;
    }

    return 0;
}
