#include "aux.h"

// Function to center a string within a given width, filling with '=' characters
std::string center_text(std::string input, std::size_t width) {

    input = " " + input + " ";

    if (input.length() >= width) {
        return input;  // If the input string is longer than the width, return the input string as it is
    }


    std::size_t padding = width - input.length();
    std::size_t padding_left = padding / 2;
    std::size_t padding_right = padding - padding_left;

    std::string result = std::string(padding_left, '=') + input + std::string(padding_right, '=');
    return result;
}

// Wrap text for better displaying option descriptions
std::string wrap_text(const std::string &text, std::size_t first_line_indent, std::size_t subsequent_indent, std::size_t width) {
    std::istringstream words(text);
    std::ostringstream wrapped;
    std::string word;
    std::string line;
    std::size_t current_width = first_line_indent;

    while (words >> word) {
        if (line.length() + word.length() + 1 > width) {
            wrapped << line << "\n" << std::string(subsequent_indent, ' ');
            line.clear();
            current_width = subsequent_indent;
        } else if (!line.empty()) {
            line += " ";
            current_width += 1;
        }
        line += word;
        current_width += word.length();
    }
    if (!line.empty()) {
        wrapped << line;
    }

    return wrapped.str();
}