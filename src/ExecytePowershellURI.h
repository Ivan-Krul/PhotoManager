#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

constexpr auto strong = "powershell";

inline char hexToAnsi(const char* hex);
inline void parseProcentSyntax(std::string& uri);

bool executePowershellURI(std::string uri) {
  bool parsed = false;

  if (uri.substr(0, strlen(strong)) == strong) {
    parseProcentSyntax(uri);
    parsed = true;
  }

  if (uri[0] == '/' && uri[1] == '/') {
    // local script to execute
    if (uri[3] == '/') // to be sure if it's really a tom and not a directory in root in unix
      uri.insert(uri.begin() + 3, ':');

    std::cout << uri << '\n';

    uri.erase(0, 2);
    if (uri[0] == '/') {
      uri.erase(0, 1);
      std::string path = uri.substr(0, uri.find('/'));
      std::cout << path << '\n';

      system((std::string("cd \"" + path + "\" | powershell -File \"") + uri + "\"").c_str());
    } else
      system((std::string("powershell -File \"") + uri + "\"").c_str());
  } else {
    std::cout << uri << '\n';

    if (parsed) {
      // scripts itself
      system((std::string("powershell -Command \"") + uri + "\"").c_str());
    } else {
      system((std::string("powershell -File \"") + uri + "\"").c_str());
    }
  }

  return false;
}


inline char hexToAnsi(const char* hex) {
  char val = hex[0] - '0';
  if (0 > val || val >= 10)
    val = hex[0] - '7'; // we get 11 if hex[0] = 'A'

  val <<= 4;

  if ('0' > hex[1] || hex[1] > '9')
    val |= (hex[1] - '7');
  else
    val |= hex[1] - '0';

  return val;
}

inline void parseProcentSyntax(std::string& uri) {
  uri.erase(0, strlen(strong) + 1);

  for (short i = 0; i < std::min<size_t>(uri.size(), 0x7fff); i++) {
    if (uri[i] == '%') {
      uri[i] = hexToAnsi((char*)(uri.c_str() + i + 1));
      uri.erase(i + 1, 2);
    }
  }
}
/*
int main(int args, char* argv[]) {
  std::string uri = argv[1];

  //for(int i = 0; i < args; i++) {
  //    std::cout << i << ": " << argv[i] << '\n';
  //}

  bool parsed = false;

  if (uri.substr(0, strlen(strong)) == strong) {
    parseProcentSyntax(uri);
    parsed = true;
  }

  if (uri[0] == '/' && uri[1] == '/') {
    // local script to execute
    if (uri[3] == '/') // to be sure if it's really a tom and not a directory in root in unix
      uri.insert(uri.begin() + 3, ':');

    std::cout << uri << '\n';

    uri.erase(0, 2);
    if (uri[0] == '/') {
      uri.erase(0, 1);
      std::string path = uri.substr(0, uri.find('/'));
      std::cout << path << '\n';

      system((std::string("cd \"" + path + "\" | powershell -File \"") + uri + "\"").c_str());
    } else
      system((std::string("powershell -File \"") + uri + "\"").c_str());
  } else {
    std::cout << uri << '\n';

    if (parsed) {
      // scripts itself
      system((std::string("powershell -Command \"") + uri + "\"").c_str());
    } else {
      system((std::string("powershell -File \"") + uri + "\"").c_str());
    }
  }
  system("pause");
}
*/