#ifndef DUMMY_HPP
#define DUMMY_HPP


#define DEFAULT_CONFIG_PATH   "./config_example.config"

#define S_DEFAULT   1
#define S_SERVER    2
#define S_LOCATION  4

#define SERVER_BLOCK_OPEN     "server {"
#define SERVER_BLOCK_CLOSE    "}"
#define LOCATION_BLOCK_OPEN   "\tlocation"
#define LOCATION_BLOCK_CLOSE  "\t}"

#define SERVER_BLOCK_TAP      "\t"
#define LOCATION_BLOCK_TAP    "\t\t"

#define CRLF                  "\r\n"
#define CRLF_RETURN           "\r\n\r\n"

#define WSV_STR_EMPTY         ""

#define CLEAR_VECTOR(A)       (A).clear();
#define SWAP_VECTOR(A)        std::vector<std::string>().swap(A);
#define CLEAR_VECTOR_COMPLETLY(A) {CLEAR_VECTOR(A)}; {SWAP_VECTOR(A)}

#define TO_SERVER(A) (A & S_SERVER) ? A >>= 1 : A;

#endif
