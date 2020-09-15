#include "fluxion.h"
#include "internals/debug.h"
#include "internals/Parser.h"

void fluxion::interpret(const char *source) {
    Parser parser {source};
    parser.parse();
    debug::printTokens(parser.getTokens());
}
