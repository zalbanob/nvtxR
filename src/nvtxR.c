#include <stdint.h>
#include <R.h>
#include <Rinternals.h>
#include "nvToolsExt.h"

static const uint32_t colors[] = {
    0xff00ff00, 0xff0000ff, 0xffffff00,
    0xffff00ff, 0xff00ffff, 0xffff0000, 0xffffffff
};
static const int num_colors = sizeof(colors) / sizeof(uint32_t);

SEXP R_nvtx_push_range(SEXP nameSEXP, SEXP cidSEXP) {
    if (!Rf_isString(nameSEXP) || Rf_length(nameSEXP) != 1)
        Rf_error("name must be a string");
    if (!Rf_isInteger(cidSEXP) || Rf_length(cidSEXP) != 1)
        Rf_error("cid must be an integer");

    const char *label = CHAR(STRING_ELT(nameSEXP, 0));
    int cid = INTEGER(cidSEXP)[0];
    int color_id = (cid % num_colors + num_colors) % num_colors;

    nvtxEventAttributes_t eventAttrib = {0};
    eventAttrib.version = NVTX_VERSION;
    eventAttrib.size = NVTX_EVENT_ATTRIB_STRUCT_SIZE;
    eventAttrib.colorType = NVTX_COLOR_ARGB;
    eventAttrib.color = colors[color_id];
    eventAttrib.messageType = NVTX_MESSAGE_TYPE_ASCII;
    eventAttrib.message.ascii = label;

    nvtxRangePushEx(&eventAttrib);
    return R_NilValue;
}

SEXP R_nvtx_pop_range(void) {
    nvtxRangePop();
    return R_NilValue;
}

#include <R_ext/Rdynload.h>
static const R_CallMethodDef CallEntries[] = {
    {"R_nvtx_push_range", (DL_FUNC) &R_nvtx_push_range, 2},
    {"R_nvtx_pop_range",  (DL_FUNC) &R_nvtx_pop_range,  0},
    {NULL, NULL, 0}
};

void R_init_nvtxR(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
