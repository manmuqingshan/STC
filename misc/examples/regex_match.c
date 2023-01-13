#define i_extern
#include <stc/cregex.h>
#include <stc/csview.h>
#define i_val float
#include <stc/cstack.h>

int main()
{
    // Lets find the first sequence of digits in a string
    const char *str = "Hello numeric world, there are 24 hours in a day, 3600 seconds in an hour."
                      " Around 365.25 days a year, and 52 weeks in a year."
                      " Boltzmann const: 1.38064852E-23, is very small."
                      " Bohrradius is 5.29177210903e-11, and Avogadros number is 6.02214076e23.";

    c_AUTO (cregex, re)
    c_AUTO (cstack_float, vec)
    c_AUTO (cstr, nums)
    {
        const char* pattern = "[+-]?([0-9]*\\.)?\\d+([Ee][+-]?\\d+)?";
        int res = cregex_compile(&re, pattern, CREG_DEFAULT);
        printf("%d: %s\n", res, pattern);

        // extract and convert all numbers in str to floats
        c_FORMATCH (i, &re, str)
            cstack_float_push(&vec, atof(i.match[0].str));

        c_FOREACH (i, cstack_float, vec)
            printf("  %g\n", *i.ref);

        // extracts the numbers only to a comma separated string.
        nums = cregex_replace_sv(&re, csview_from(str), " $0,", 0, NULL, CREG_R_STRIP);
        printf("\n%s\n", cstr_str(&nums));
    }
}