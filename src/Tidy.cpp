#include "Tidy.hpp"

#include <iostream>
#include <stdexcept>

#include <fmt/core.h>


namespace htmlTidy {
#if __has_include(<tidy/tidy.h>)
#include <tidy/tidy.h>
#include <tidy/tidybuffio.h>
#include <tidy/tidyenum.h>
#else
#include <tidy.h>
#include <tidybuffio.h>
#include <tidyenum.h>
#endif
}   // namespace htmlTidy


auto tidy(std::string_view text) -> std::string {
    htmlTidy::TidyBuffer out { nullptr, nullptr, 0, 0, 0 };
    htmlTidy::TidyBuffer err { nullptr, nullptr, 0, 0, 0 };
    htmlTidy::TidyDoc tdoc = htmlTidy::tidyCreate();

    int rc = -1;
    bool ok = htmlTidy::tidyOptSetBool(tdoc, htmlTidy::TidyXhtmlOut, htmlTidy::yes) != 0U   // Convert to XHTML
           && htmlTidy::tidyOptSetInt(tdoc, htmlTidy::TidyWrapLen, 0) != 0U                 // No line wrapping
           && htmlTidy::tidyOptSetBool(tdoc, htmlTidy::TidyMakeBare, htmlTidy::yes) != 0U   // Make bare
           && htmlTidy::tidyOptSetValue(tdoc, htmlTidy::TidyVertSpace, "auto") != 0U;       // No vertical space
    if (ok) {
        rc = htmlTidy::tidySetErrorBuffer(tdoc, &err);   // Capture diagnostics
    }
    if (rc >= 0) {
        rc = htmlTidy::tidyParseString(tdoc, text.data());   // Parse the input
    }
    if (rc >= 0) {
        rc = htmlTidy::tidyCleanAndRepair(tdoc);   // Tidy it up!
    }
    if (rc >= 0) {
        rc = htmlTidy::tidyRunDiagnostics(tdoc);   // Kvetch
    }
    if (rc > 1) {   // If error, force output.
        rc = (htmlTidy::tidyOptSetBool(tdoc, htmlTidy::TidyForceOutput, htmlTidy::yes) != 0U ? rc : -1);
    }
    if (rc >= 0) {
        rc = htmlTidy::tidySaveBuffer(tdoc, &out);   // Pretty Print
    }

    if (rc < 0) {
        std::cerr << "Tidy failed: " << err.bp << std::endl;
        // TODO: write RAII wrapper
        htmlTidy::tidyBufFree(&out);
        htmlTidy::tidyBufFree(&err);
        htmlTidy::tidyRelease(tdoc);
        throw std::runtime_error { fmt::format("A severe error occurred in tidy: {}", rc) };
    }

    std::string result { reinterpret_cast<char*>(out.bp) };

    htmlTidy::tidyBufFree(&out);
    htmlTidy::tidyBufFree(&err);
    htmlTidy::tidyRelease(tdoc);

    return result;
}
