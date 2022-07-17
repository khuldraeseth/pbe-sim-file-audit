#include "Tidy.hpp"

#include <iostream>


namespace htmlTidy {
#include <tidy.h>
#include <tidybuffio.h>
#include <tidyenum.h>
}


auto tidy(std::string_view text) -> std::string {
    htmlTidy::TidyBuffer out { nullptr, nullptr, 0, 0, 0 };
    htmlTidy::TidyBuffer err { nullptr, nullptr, 0, 0, 0 };
    htmlTidy::TidyDoc tdoc = htmlTidy::tidyCreate();

    // printf("Tidying:\t%s\n", text.data());
    // std::cout << "Tidying:\t" << text << std::endl;

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

    if (rc >= 0) {
        if (rc > 0) {
            // printf("\nDiagnostics:\n\n%s", err.bp);
            std::cerr << "\nDiagnostics:\n\n" << err.bp << std::endl;
        }
        // printf("\nAnd here is the result:\n\n%s", out.bp);
        // std::cout << "\nAnd here is the result:\n\n" << out.bp << std::endl;
    } else {
        // printf("A severe error (%d) occurred.\n", rc);
        std::cerr << "A severe error (" << rc << ") occurred." << std::endl;
    }

    std::string result { reinterpret_cast<char*>(out.bp) };

    htmlTidy::tidyBufFree(&out);
    htmlTidy::tidyBufFree(&err);
    htmlTidy::tidyRelease(tdoc);

    return result;
}
