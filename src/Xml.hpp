#pragma once

#include <pugixml.hpp>
#include <string_view>


auto parseXmlString(std::string_view text) -> pugi::xml_document;
