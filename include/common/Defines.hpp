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
#define D_CRLF                "\r\n\r\n"

#define WSV_STR_EMPTY         ""
#define WSV_NUM_ZERO          0
#define WSV_NUM_MINUS         -1


#define CLEAR_VECTOR(A)       (A).clear();
#define SWAP_VECTOR(A)        std::vector<std::string>().swap(A);
#define CLEAR_VECTOR_COMPLETLY(A) {CLEAR_VECTOR(A)}; {SWAP_VECTOR(A)}

#endif
