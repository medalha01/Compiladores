#include <fstream>
#include <vector>
#include <memory>

#include "constants.h"
#include "ident_diagram.h"
#include "int_const_diagram.h"
#include "float_const_diagram.h"
#include "string_const_diagram.h"

using namespace std;

int main()
{
    vector<unique_ptr<Diagram>> diagrams;
    diagrams.push_back(make_unique<IdentDiagram>());
    diagrams.push_back(make_unique<IntConstDiagram>());
    diagrams.push_back(make_unique<FloatConstDiagram>());

    ifstream inputFile(INPUT_FILE);
    ofstream outputFile(OUTPUT_FILE);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Error opening files!" << endl;
        return 1;
    }

    // Process input file.
    char c;
    string current_lexem = "";
    string current_token = "";
    char character_to_backtrack = '\0';
    pair<DiagramProcessing, pair<string, string>> result;
    while (inputFile.get(c)) {  // PS, necessário tratar o EOF? --> DIAGRAMas rodando podem não terminar.
        for (auto& diagram : diagrams) {
            result = diagram->parse(c);
           
            switch (result.first)
            {
                case IN_PROGRESS:
                    break;
                
                case FINISHED:
                    if (result.second.second.length() > current_lexem.length()) {
                        current_token = result.second.first;
                        current_lexem = result.second.second;
                        character_to_backtrack = '\0';
                    }
                    break;

                case FINISHED_AND_BACKTRACK:
                    if (result.second.second.length() > current_lexem.length()) {
                        current_token = result.second.first;
                        current_lexem = result.second.second;
                        character_to_backtrack = c;
                    }
                    break;

                case FAILED:
                    if (current_token == "") {
                        current_token = result.second.first;
                    }
                    break;

                default:
                    assert(false);  // Should'nt run this.
                    break;
            }
        }
    }

    inputFile.close();
    outputFile.close();

    return 0;
}