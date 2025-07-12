#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "calc.hpp"

std::ostream& Calc::operator<<(std::ostream& out, const Calc::Token& t) {
  out << "{'" << t.type << "', " << t.val << '}';
  return out;
}

bool Calc::operator==(const Calc::Token& a, const Calc::Token& b) {
  if (a.type != b.type) {
    return false;
  }
  if (a.type == 'n') {
    return a.val == b.val;
  }
  return true;
}

// This is the function for you to write
std::vector<Calc::Token> Calc::infixToPostfix(const std::vector<Token>& input) {
  std::vector<Token> output;
  std::vector<Token> operatorStack;

  //Defining operator precedence with a lambda function
  auto precedence =[](char op){
    if (op == '+'|| op == '-') return 1;
    if (op == '*'|| op == '/') return 2;
    //For non operators:
    return 0;
  };
  
  //Iterate through the tokens in the input
  for(const Token& t : input){
    if(t.type == 'n'){
      //Numbers directly go to the output
      output.push_back(t);
    }  
    else if(t.type == '('){
      //The '(' goes back into the operatorstack
        operatorStack.push_back(t);
    }    
    else if(t.type == ')'){
      //Pop from stack to output until ( is found
      while(!operatorStack.empty() && operatorStack.back().type != '('){
        output.push_back(operatorStack.back());
        operatorStack.pop_back();
      }
      //Discard the (
      if(!operatorStack.empty() && operatorStack.back().type == '('){
        operatorStack.pop_back();
      }
    }
    else{
      //Operator: Pop higher or equal precedence operators to the outputs
      while(!operatorStack.empty() && operatorStack.back().type != '(' && 
      precedence(operatorStack.back().type) >= precedence(t.type)){
        output.push_back(operatorStack.back());
        operatorStack.pop_back();
      }
      //Push the current operastor to the stack }
      operatorStack.push_back(t);
    }
  }

  //Add any remaining operators
  while (!operatorStack.empty()){
    output.push_back(operatorStack.back());
    operatorStack.pop_back();
  }

  return output;
}

// evalPostfix evaluates a vector of tokens in postfix notation
// This function was done in tutorial Week 10
int Calc::evalPostfix(const std::vector<Token>& tokens) {
  if (tokens.empty()) {
    return 0;
  }
  std::vector<int> stack;
  for (Token t : tokens) {
    if (t.type == 'n') {
      stack.push_back(t.val);
    } else {
      int val = 0;
      if (t.type == '+') {
        val = stack.back() + *(stack.end()-2);
      } else if (t.type == '*') {
        val = stack.back() * *(stack.end()-2);
      } else if (t.type == '-') {
        val = *(stack.end()-2) - stack.back();
      } else if (t.type == '/') {
        if (stack.back() == 0) {
          throw std::runtime_error("divide by zero");
        }
        val = *(stack.end()-2) / stack.back();
      } else {
          std::cout << "invalid token\n";
      }
      stack.pop_back();
      stack.pop_back();
      stack.push_back(val);
    }
  }
  return stack.back();
}

// tokenise takes a string and parses it into a vector of tokens
std::vector<Calc::Token> Calc::tokenise(const std::string& expression) {
  const std::vector<char> symbols = {'+', '-', '*', '/', '(', ')'};
  std::vector<Token> tokens {};
  for (std::size_t i =0; i < expression.size(); ++i) {
    const char c = expression[i];
    // check if c is one of '+', '-', '*', '/', '(', ')'
    if (std::find(symbols.begin(), symbols.end(), c) != symbols.end()) {
      tokens.push_back({c});
    } else if (isdigit(c)) {
      // process multiple digit integers
      std::string num {c};
      while (i + 1 < expression.size() && isdigit(expression[i + 1])) {
        ++i;
        num.push_back(expression[i]);
      }
      tokens.push_back({'n', std::stoi(num)});
    }
  }
  return tokens;
}

// eval puts the pieces together to take a string with an
// arithmetic expression and output its evaluation
int Calc::eval(const std::string& expression) {
  std::vector<Token> tokens = tokenise(expression);
  std::vector<Token> postfix = infixToPostfix(tokens);
  return evalPostfix(postfix);
}
