#ifndef BASE64_H
#define BASE64_H

#include <string>

class Base64 {
public:
  static std::string encode(const std::string &input) {
    static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                            "abcdefghijklmnopqrstuvwxyz"
                                            "0123456789+/";

    std::string out;
    int val = 0;
    int val_b = -6;

    for (unsigned char c : input) {
      val = (val << 8) + c;
      val_b += 8;
      while (val_b >= 0) {
        out.push_back(base64_chars[(val >> val_b) & 0x3F]);
        val_b -= 6;
      }
    }

    if (val_b > -6) {
      out.push_back(base64_chars[((val << 8) >> (val_b + 8)) & 0x3F]);
    }

    while (out.size() % 4) {
      out.push_back('=');
    }

    return out;
  }
};

#endif
