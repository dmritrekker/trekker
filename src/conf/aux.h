#pragma once

#include "config.h"

std::string center_text(std::string input, std::size_t width);
std::string wrap_text(const std::string &text, std::size_t first_line_indent, std::size_t subsequent_indent, std::size_t width);

// Custom help formatter for CLI11 to improve the display of options and groups
class CustomHelpFormatter : public CLI::Formatter {
public:
    CustomHelpFormatter() : CLI::Formatter() {
        size_t optionWidth = 50;
        column_width(optionWidth);
    }

    std::string make_option(const CLI::Option *opt, bool is_positional) const override {

        size_t descriptionWidth = 50;

        std::ostringstream out;
        // Get the description and the default formatted option
        std::string description = opt->get_description();
        std::string formatted_option = CLI::Formatter::make_option(opt, is_positional);

        // Calculate the position where the description starts
        size_t description_start = formatted_option.find(description);
        size_t first_line_indent = description_start;
        size_t subsequent_indent = description_start;

        // Adjust the description to wrap text and align properly
        std::string wrapped_description = wrap_text(description, first_line_indent, subsequent_indent+2, descriptionWidth);

        // Replace the original description with the wrapped description
        formatted_option.replace(description_start, description.length(), wrapped_description);

        out << "  " << formatted_option;
        return out.str();
    }

};