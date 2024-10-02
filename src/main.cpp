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
    bool is_processing = false;
    size_t number_diagrams = diagrams.size();
    size_t completed_process_diagrams;
    while (inputFile.get(c)) {  // PS, necessário tratar o EOF? --> DIAGRAMas rodando podem não terminar.

        if (!is_processing and isspace(c)) {
            continue;
        }

        if (completed_process_diagrams == number_diagrams) {
            is_processing = false;
            completed_process_diagrams = 0;
            // Here saves token and/or lexems.
            outputFile << current_token << " ";
            current_lexem = "";
            current_token = "";
            character_to_backtrack = '\0';     // Still not handling when we have backtrack!

            // reset diagrams
            for (auto& diagram : diagrams) {
                diagram.reset();
            }


            if (character_to_backtrack == '\0') {
                continue;
            } else {
                c = character_to_backtrack;
                character_to_backtrack == '\0';    // And execution continues using the caracter in backtrack.
            }
        }

        completed_process_diagrams = 0;
        is_processing = true;

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

                    cout << "TOKEN FOUND: " << current_token << endl; // DEBUG DELETEME

                    completed_process_diagrams++;
                    break;

                case FINISHED_AND_BACKTRACK:
                    if (result.second.second.length() > current_lexem.length()) {
                        current_token = result.second.first;
                        current_lexem = result.second.second;
                        character_to_backtrack = c;
                    }
                    
                    cout << "TOKEN FOUND: " << current_token << endl; // DEBUG DELETEME

                    completed_process_diagrams++;
                    break;

                case FAILED:
                    if (current_token == "") {
                        current_token = result.second.first;
                    }

                    cout << "TOKEN FOUND: " << current_token << endl; // DEBUG DELETEME

                    completed_process_diagrams++;
                    break;

                default:
                    assert(false);  // Shouldn't run this.
                    break;
            }
        }
    }

    inputFile.close();
    outputFile.close();

    return 0;
}