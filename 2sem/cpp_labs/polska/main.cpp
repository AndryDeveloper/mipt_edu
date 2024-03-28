#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "structures.h"

using namespace std;
map<string, int> priority{
    {"*", 3},
    {"/", 3},
    {"+", 2},
    {"-", 2},
    {"(", 1},
    {")", 0}};


bool isnum(char symb)
{
  return ((symb >= '0' && symb <= '9') || symb == '.');
}

bool isnum(string str)
{
  if (str == "")
  {
    return false;
  }
  if (str.length() == 1){
    return isnum(str[0]);
  }

  bool result = true;
  for (int i = 0; i < static_cast<int>(str.length()); i++){
    result = result && isnum(str[i]);
  }
  return result;
}

string get_string(char x)
{
  string s(1, x);
  return s;
}

vector<string> parse(string str)
{
  vector<string> result;
  for (int i = 0; i < static_cast<int>(str.length()); i++)
  {
    if (str[i] != ' ')
    {
      if (isnum(str[i]))
      {
        if (static_cast<int>(result.size()) == 0)
        {
          result.push_back(get_string(str[i]));
        }
        else if (!isnum(result.back()))
        {
          result.push_back(get_string(str[i]));
        }
        else
        {
          result.back() += get_string(str[i]);
        }
      }
      else
      {
        result.push_back(get_string(str[i]));
      }
    }
  }
  return result;
}

vector<string> str2notation(vector<string> str)
{
  Stack<string> stack;
  vector<string> output;
  for (int i = 0; i < static_cast<int>(str.size()); i++)
  {
    if (isnum(str[i]))
    {
      output.push_back(str[i]);
    }
    else
    {
      if (util::is_empty(stack) || priority[str[i]] == 1)
      {
        push(stack, str[i]);
      }
      else if (priority[str[i]] == 0)
      {
        // Обработка закрывающей скобки
        while (priority[read(stack)] != 1)
        {
          output.push_back(pop(stack));
        }
        pop(stack);
      }
      else
      {
        // Знаки кроме скобок
        while (!util::is_empty(stack) && priority[read(stack)] >= priority[str[i]])
        {
          output.push_back(pop(stack));
        }
        push(stack, str[i]);
      }
    }
  }

  while (!util::is_empty(stack))
  {
    output.push_back(pop(stack));
  }

  return output;
}

double calculate(vector<string> output)
{
  Stack<double> stack;
  for (int i = 0; i < static_cast<int>(output.size()); i++)
  {
    if (isnum(output[i]))
    {
      push(stack, stod(output[i]));
    }
    else
    {
      if (output[i] == "+")
        push(stack, pop(stack) + pop(stack));
      if (output[i] == "-")
        push(stack, -(pop(stack) - pop(stack)));
      if (output[i] == "/")
        push(stack, 1 / (pop(stack) / pop(stack)));
      if (output[i] == "*")
        push(stack, pop(stack) * pop(stack));
    }
  }
  return pop(stack);
}

int main()
{
  string input;
  getline(cin, input);

  // старт
  vector<string> output = str2notation(parse(input));

  cout << calculate(output) << endl;
  return 0;
}