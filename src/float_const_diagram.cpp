#include "float_const_diagram.h"

/*
    See diagram.h 
*/
std::pair<DiagramProcessing,
 std::pair<std::string, std::string>> FloatConstDiagram::parse(const char entry) {

    std::pair<std::string, std::string> token_and_lexem;
    std::pair<DiagramProcessing, std::pair<std::string, std::string>> return_pair;

    switch (_current_state)
    {
        case 0:
            if (isdigit(entry)) {
                _current_state = 1;
                _current_lexem += entry;

                token_and_lexem = {"", ""};
                return_pair.first = IN_PROGRESS;
                return_pair.second = token_and_lexem;
            } else {
                 token_and_lexem = {GENERIC_TOKEN, ""};
                return_pair.first = FAILED;
                return_pair.second = token_and_lexem;
            }
            break;
        
        case 1:
            if (isdigit(entry)) {
                // _current_state = 1;
                _current_lexem += entry; 

                token_and_lexem = {"", ""};
                return_pair.first = IN_PROGRESS;
                return_pair.second = token_and_lexem;

            } else if (entry == '.') {
                 _current_state = 2;
                 _current_lexem += entry;

                token_and_lexem = {"", ""};
                return_pair.first = IN_PROGRESS;
                return_pair.second = token_and_lexem;
            } else {
                // TODO: revisar transição do diagrama.
            }
            break;
        
        case 2:
            if (isdigit(entry)) {
                // _current_state = 2;
                _current_lexem += entry;

                token_and_lexem = {"", ""};
                return_pair.first = IN_PROGRESS;
                return_pair.second = token_and_lexem;
            } else {
                // _current_state = 3;
                token_and_lexem = {FLOAT_TOKEN, _current_lexem};
                return_pair.first = FINISHED_AND_BACKTRACK;
                return_pair.second = token_and_lexem;
            }
            break;
            
        default:
            assert(false);  // Should not get here.
            break;
    }

    // Gets ready to process a new token.
    if (return_pair.first != IN_PROGRESS) {
        reset();
    }

    return return_pair;
}