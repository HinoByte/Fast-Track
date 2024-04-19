#include <iostream>
#include <unordered_map>
#include <variant>
#include <vector>

struct OpeningBracket {
  friend std::ostream& operator<<(std::ostream& os, const OpeningBracket&) {
    return os << "OpeningBracket";
  }
};

struct ClosingBracket {
  friend std::ostream& operator<<(std::ostream& os, const ClosingBracket&) {
    return os << "ClosingBracket";
  }
};

struct Number {
  int value;
  friend std::ostream& operator<<(std::ostream& os, const Number& token) {
    return os << "Number: " << token.value;
  }
};

struct UnknownToken {
  std::string value;
  friend std::ostream& operator<<(std::ostream& os, const UnknownToken& token) {
    return os << "UnknownToken: " << token.value;
  }
};

struct MinToken {
  friend std::ostream& operator<<(std::ostream& os, const MinToken&) {
    return os << "MinToken";
  }
};

struct MaxToken {
  friend std::ostream& operator<<(std::ostream& os, const MaxToken&) {
    return os << "MaxToken";
  }
};

struct SqrToken {
  friend std::ostream& operator<<(std::ostream& os, const SqrToken&) {
    return os << "SqrToken";
  }
};

struct AbsToken {
  friend std::ostream& operator<<(std::ostream& os, const AbsToken&) {
    return os << "AbsToken";
  }
};

struct Plus {
  friend std::ostream& operator<<(std::ostream& os, const Plus&) {
    return os << "Plus";
  }
};

struct Minus {
  friend std::ostream& operator<<(std::ostream& os, const Minus&) {
    return os << "Minus";
  }
};

struct Multiply {
  friend std::ostream& operator<<(std::ostream& os, const Multiply&) {
    return os << "Multiply";
  }
};

struct Modulo {
  friend std::ostream& operator<<(std::ostream& os, const Modulo&) {
    return os << "Modulo";
  }
};

struct Divide {
  friend std::ostream& operator<<(std::ostream& os, const Divide&) {
    return os << "Divide";
  }
};

using Token = std::variant<OpeningBracket, ClosingBracket, Number, UnknownToken,
                           MinToken, MaxToken, SqrToken, AbsToken, Plus, Minus,
                           Multiply, Modulo, Divide>;

                           
std::ostream& operator<<(std::ostream& os, const Token& token) {
  std::visit([&os](const auto& t) { os << t; }, token);
  return os;
}

const std::unordered_map<unsigned char, Token> kSymbol2Token{
    {'+', Plus{}},          {'-', Minus{}},  {'*', Multiply{}},
    {'/', Divide{}},        {'%', Modulo{}}, {'(', OpeningBracket{}},
    {')', ClosingBracket{}}};

const std::unordered_map<std::string, Token> kName2Token{{"min", MinToken{}},
                                                         {"max", MaxToken{}},
                                                         {"sqr", SqrToken{}},
                                                         {"abs", AbsToken{}}};

int ToDigit(unsigned char symbol) { return symbol - '0'; }

Number ParseNumber(const std::string& input, size_t& pos) {
  int value = 0;
  auto symbol = static_cast<unsigned char>(input[pos]);
  while (std::isdigit(symbol)) {
    value = value * 10 + ToDigit(symbol);
    if (pos == input.size()) {
      break;
    }
    symbol = static_cast<unsigned char>(input[++pos]);
  }
  return Number{value};
}

Token ParseName(const std::string& input, size_t& pos) {
  std::string name;
  auto symbol = static_cast<unsigned char>(input[pos]);
  while (std::isalpha(symbol)) {
    name += symbol;
    if (pos == input.size()) {
      break;
    }
    symbol = static_cast<unsigned char>(input[++pos]);
  }
  if (auto it = kName2Token.find(name); it != kName2Token.end()) {
    return it->second;
  }
  return UnknownToken{name};
}

std::vector<Token> Tokenize(const std::string& input) {
  std::vector<Token> tokens;
  const size_t size = input.size();
  size_t pos = 0;
  while (pos < size) {
    const auto symbol = static_cast<unsigned char>(input[pos]);
    if (std::isspace(symbol)) {
      ++pos;
    } else if (std::isdigit(symbol)) {
      tokens.emplace_back(ParseNumber(input, pos));
    } else if (auto it = kSymbol2Token.find(symbol);
               it != kSymbol2Token.end()) {
      tokens.emplace_back(it->second);
      ++pos;
    } else if (std::isalpha(symbol)) {
      tokens.emplace_back(ParseName(input, pos));
    }
  }
  return tokens;
}

int main() {
  const std::string input = "(1 + 2) * 3 / 4 + 5 * (6 - 7)";
  std::cout << "Tokens:" << std::endl;
  std::vector<Token> tokens = Tokenize(input);
  for (const auto& token : tokens) {
    std::cout << token << std::endl;
  }
  return 0;
}