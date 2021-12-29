/* MIT License
 *
 * Copyright (c) 2021 Tyge Løvset, NORCE, www.norceresearch.no
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef CSVIEW_H_INCLUDED
#define CSVIEW_H_INCLUDED

#include "cstr.h"

typedef                 struct csview { const char* str; size_t size; } csview;
typedef                 struct csview_iter { const char *ref; } csview_iter;
typedef                 char csview_value;

#define                 csview_null  c_make(csview){"", 0}
#define                 csview_npos  cstr_npos
#define                 c_svfmt      "%.*s"
#define                 c_svarg(sv)  (int)(sv).size, (sv).str
#define                 c_sv(literal) csview_new(literal)
#define                 cstr_sv(s) csview_from_s(s)

STC_API csview          csview_substr(csview sv, intptr_t pos, size_t n);
STC_API csview          csview_slice(csview sv, intptr_t p1, intptr_t p2);
STC_API csview          csview_token(csview sv, csview sep, size_t* start);

#define                 csview_new(literal) \
                            c_make(csview){literal, sizeof c_make(c_strlit){literal} - 1}
STC_INLINE csview       csview_from(const char* str)
                            { return c_make(csview){str, strlen(str)}; }
STC_INLINE csview       csview_from_n(const char* str, size_t n)
                            { return c_make(csview){str, n}; }
STC_INLINE csview       csview_from_s(cstr s)
                            { return c_make(csview){s.str, _cstr_rep(&s)->size}; }
STC_INLINE size_t       csview_size(csview sv) { return sv.size; }
STC_INLINE size_t       csview_length(csview sv) { return sv.size; }
STC_INLINE bool         csview_empty(csview sv) { return sv.size == 0; }
STC_INLINE char         csview_front(csview sv) { return sv.str[0]; }
STC_INLINE char         csview_back(csview sv) { return sv.str[sv.size - 1]; }

STC_INLINE void         csview_clear(csview* self) { *self = csview_null; }

STC_INLINE bool         csview_equals(csview sv, csview sv2)
                            { return sv.size == sv2.size && !memcmp(sv.str, sv2.str, sv.size); }
STC_INLINE size_t       csview_find(csview sv, csview needle)
                            { char* res = c_strnstrn(sv.str, needle.str, sv.size, needle.size);
                              return res ? res - sv.str : cstr_npos; }
STC_INLINE bool         csview_contains(csview sv, csview needle)
                            { return c_strnstrn(sv.str, needle.str, sv.size, needle.size) != NULL; }
STC_INLINE bool         csview_starts_with(csview sv, csview sub)
                            { if (sub.size > sv.size) return false;
                              return !memcmp(sv.str, sub.str, sub.size); }
STC_INLINE bool         csview_ends_with(csview sv, csview sub)
                            { if (sub.size > sv.size) return false;
                              return !memcmp(sv.str + sv.size - sub.size, sub.str, sub.size); }
STC_INLINE csview_iter  csview_begin(const csview* self)
                            { return c_make(csview_iter){self->str}; }
STC_INLINE csview_iter  csview_end(const csview* self)
                            { return c_make(csview_iter){self->str + self->size}; }
STC_INLINE void         csview_next(csview_iter* it) { ++it->ref; }


/* cstr interaction with csview: */

STC_INLINE cstr         cstr_from_v(csview sv)
                            { return cstr_from_n(sv.str, sv.size); }
STC_INLINE cstr         cstr_from_replace_all_v(csview sv, csview find, csview repl)
                            { return cstr_from_replace_all(sv.str, sv.size, find.str, find.size,
                                                           repl.str, repl.size); }
STC_INLINE csview       cstr_to_v(const cstr* self)
                            { return c_make(csview){self->str, _cstr_rep(self)->size}; }
STC_INLINE csview       cstr_substr(cstr s, intptr_t pos, size_t n)
                            { return csview_substr(csview_from_s(s), pos, n); }
STC_INLINE csview       cstr_slice(cstr s, intptr_t p1, intptr_t p2)
                            { return csview_slice(csview_from_s(s), p1, p2); }
STC_INLINE cstr*        cstr_assign_v(cstr* self, csview sv)
                            { return cstr_assign_n(self, sv.str, sv.size); }
STC_INLINE cstr*        cstr_append_v(cstr* self, csview sv)
                            { return cstr_append_n(self, sv.str, sv.size); }
STC_INLINE void         cstr_insert_v(cstr* self, size_t pos, csview sv)
                            { cstr_replace_n(self, pos, 0, sv.str, sv.size); }
STC_INLINE void         cstr_replace_v(cstr* self, size_t pos, size_t len, csview sv)
                            { cstr_replace_n(self, pos, len, sv.str, sv.size); }
STC_INLINE bool         cstr_equals_v(cstr s, csview sv)
                            { return sv.size == cstr_size(s) && !memcmp(s.str, sv.str, sv.size); }
STC_INLINE size_t       cstr_find_v(cstr s, csview needle)
                            { char* res = c_strnstrn(s.str, needle.str, cstr_size(s), needle.size);
                              return res ? res - s.str : cstr_npos; }
STC_INLINE bool         cstr_contains_v(cstr s, csview needle)
                            { return c_strnstrn(s.str, needle.str, cstr_size(s), needle.size) != NULL; }
STC_INLINE bool         cstr_starts_with_v(cstr s, csview sub)
                            { if (sub.size > cstr_size(s)) return false;
                              return !memcmp(s.str, sub.str, sub.size); }
STC_INLINE bool         cstr_ends_with_v(cstr s, csview sub)
                            { if (sub.size > cstr_size(s)) return false;
                              return !memcmp(s.str + cstr_size(s) - sub.size, sub.str, sub.size); }

/* ---- Container helper functions ---- */

STC_INLINE int          csview_cmp(const csview* x, const csview* y) { 
                            const size_t m = x->size < y->size ? x->size : y->size; 
                            const int c = memcmp(x->str, y->str, m);
                            return c ? c : x->size - y->size;
                        }
#define                 csview_hash(xp, dummy) c_strhash((xp)->str)
#define                 csview_eq(xp, yp) (!csview_cmp(xp, yp))

/* -------------------------- IMPLEMENTATION ------------------------- */

#if !defined(STC_HEADER) || defined(STC_IMPLEMENTATION)

STC_DEF csview
csview_substr(csview sv, intptr_t pos, size_t n) {
    if (pos < 0) { pos += sv.size; if (pos < 0) pos = 0; }
    if (pos > sv.size) pos = sv.size; if (pos + n > sv.size) n = sv.size - pos;
    sv.str += pos, sv.size = n; return sv;
}

STC_DEF csview
csview_slice(csview sv, intptr_t p1, intptr_t p2) {
    if (p1 < 0) { p1 += sv.size; if (p1 < 0) p1 = 0; }
    if (p2 < 0) p2 += sv.size; if (p2 > sv.size) p2 = sv.size;
    sv.str += p1, sv.size = p2 > p1 ? p2 - p1 : 0; return sv;
}

STC_DEF csview
csview_token(csview sv, csview sep, size_t* start) {
    csview slice = {sv.str + *start, sv.size - *start};
    const char* res = c_strnstrn(slice.str, sep.str, slice.size, sep.size);
    csview tok = {slice.str, (res ? res - slice.str : slice.size)};
    *start += tok.size + (res ? sep.size : 0);
    return tok;
}

#endif
#endif