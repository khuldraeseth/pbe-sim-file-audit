#include "Xml.hpp"

#include <pugixml.hpp>
#include <stdexcept>
#include <string_view>

#include <fmt/core.h>


auto parseXmlString(std::string_view text) -> pugi::xml_document {
    using namespace std::literals;

    pugi::xml_document doc;
    auto const result = doc.load_string(text.data());
    if (!result) {
        throw std::runtime_error { fmt::format("Failed to parse XML: {}"sv, result.description()) };
    }
    return doc;
}
